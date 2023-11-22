/**
  ******************************************************************************
  * @file    app.c
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    01-Feb-2023
  * @brief   EC618 at command demo entry source file.
  ******************************************************************************
  * @attention
  *
  *     Date            Version     Comments
  *     2023-02-01      V1.0.0      add SIM Ready is reported after startup.(BugID 40665).
  *     2023-02-06      V1.0.1      Modify SIM card hot swap configuration.(TaskID 30861).
  *     2023-02-06      V1.0.2      PSM--call slpManAONIOLatchEn before sleep.(TaskID 39826).
  *     2023-02-16      V1.0.3      modify simhotplug report(BugID 41254)
  * @note
  *
  * <h2><center>&copy; COPYRIGHT 2023 MEIG SMART</center></h2>
  ******************************************************************************
  */

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/

#include "string.h"
#include "bsp.h"
#include "bsp_custom.h"
#include "ostask.h"
#include DEBUG_LOG_HEADER_FILE
#include "plat_config.h"
#include "app.h"
#include "FreeRTOS.h"
#include "at_def.h"
#include "at_api.h"
//<!--[ODM][SLM332Y][BUGID 39826]liulei@2023-2-06 Call slpManAONIOLatchEn before sleep.
#include "slpman.h"
//end-->
#include "npi_config.h"
#if defined(FEATURE_CTCC_DM_ENABLE) || defined(FEATURE_CUCC_DM_ENABLE) || defined(FEATURE_CMCC_DM_ENABLE)
#include "dm_task.h"
#endif
#ifdef FEATURE_MBEDTLS_ENABLE
#include "at_ssl_task.h"
#endif
#ifdef FEATURE_HTTPC_ENABLE
#include "at_http_task.h"
#endif
#ifdef FEATURE_CTWING_CERTI_ENABLE
#include "ctw_task.h"
#endif
#include "version.h"
#ifdef CONFIG_MG_LED_SUPPORT
#include "led.h"
#endif
#include "meig_util.h"


//<!--[ODM][TaskID25305]fw@2022-11_15 add version and boot cause
#ifdef FEATURE_OPENCPU_ENABLE
#include "reset.h"
extern int MEIG_Util_GetSGSW_ID(char *outData, int outDataSize);
#endif
//end-->

#define USRAPP_TASK_STACK_SIZE        (1024)
static StaticTask_t             usrapp_task;
static uint8_t                  usrapp_task_stack[USRAPP_TASK_STACK_SIZE];
void usb_portmon_task_init(void);

//<!--[ODM][SLM332Y][BUGID 39826]liulei@2023-2-06 Call slpManAONIOLatchEn before sleep.
#if 0
//<!--[ODM][BugID36102]huqing@2022-09-20 light up NET STATUS led
static void appBeforeSleep(void *pdata, slpManLpState state)
{
    //<!--[ODM][SLM328Y][BUGID 38197]liulei@2022-11-08 close the lamp control.
    if (npiGetProcessStatusItemValue(NPI_PROCESS_STATUS_ITEM_RFCALI))
        return;
    //end-->

    // enable aonio latch
    slpManAONIOLatchEn(true);
    switch(state)
    {
        case SLPMAN_SLEEP1_STATE:
        case SLPMAN_SLEEP2_STATE:
        case SLPMAN_HIBERNATE_STATE:
#ifdef CONFIG_MG_LED_SUPPORT
            drvNetModeLedOff();
            drvNetStatusLedOff(0);
#endif
            break;
        default:
            break;
    }
}

static void appAfterSleep(void *pdata, slpManLpState state)
{
    //<!--[ODM][SLM328Y][BUGID 38197]liulei@2022-11-08 close the lamp control.
    if (npiGetProcessStatusItemValue(NPI_PROCESS_STATUS_ITEM_RFCALI))
        return;
    //end-->

    switch(state)
    {
        case SLPMAN_SLEEP1_STATE:
        case SLPMAN_SLEEP2_STATE:
        case SLPMAN_HIBERNATE_STATE:
#ifdef CONFIG_MG_LED_SUPPORT
            drvNetModeLedOn();
            drvNetStatusLedOn(0, NET_STATUS_SEARCH);
#endif
            break;
        default:
            break;
    }
}
#else
static void appBeforeSleep(void *pdata, slpManLpState state)
{
    // enable aonio latch
    slpManAONIOLatchEn(true);

//<!--[ODM][ANT][Task33295]wj@2023-03-15 ant pin init
#ifdef CONFIG_MG_CUSTOMER_YS
    extern UINT8 gps_ant_pin_init(char *pin_name, INT32 level);
    // gps ant pin init
    gps_ant_pin_init("P63", 0);
#endif
//end-->
}

static void appAfterSleep(void *pdata, slpManLpState state)
{

}
#endif
//end-->

//<!--[ODM][TaskID25305]fw@2022-11_15 add version and boot cause
#ifdef FEATURE_OPENCPU_ENABLE
static void showBootInfo(void)
{
    LastResetState_e apRstState = LAST_RESET_UNKNOWN;
    LastResetState_e cpRstState = LAST_RESET_UNKNOWN;
    char pVerBuf[33] = {0};
    ResetStateGet(&apRstState, &cpRstState);
    MEIG_Util_GetSGSW_ID(pVerBuf, 32);
    ECPLAT_PRINTF(UNILOG_MCI, showBootInfo_mci_1, P_INFO, "version:%s", pVerBuf);
	ECPLAT_PRINTF(UNILOG_MCI, showBootInfo_mci_2, P_INFO, "boot cause: AP:%d, CP:%d", apRstState, cpRstState);
}
#endif
//end-->

//<!--[ODM][BugID 38202]wbh@2022-11-11 Add task to monitor the status of SIM card.
#include "ps_lib_api.h"
#include "semphr.h"

osSemaphoreId_t simDetectSem = NULL;
extern void atProcSimHotSwapInd_BSP(BOOL bSimPlugIn);

static void UserAppTask(void *arg)
{
#ifdef CONFIG_MG_SIM_HOTPLUG_SUPPORT
    BOOL bSimPlugIn = false;
#endif

//<!--[ODM][BUGID 38814]liulei@2022-11-25 enter the PSM,device restarts.
//<!--[ODM][SLM332Y_DL][BUGID 36613]liulei@2023-2-07 Call sleepslpManAONIOLatchEn before sleep.
//#ifdef CONFIG_MG_LED_SUPPORT
    //<!--[ODM][SLM332Y][BUGID 39826]liulei@2023-2-6 call slpManAONIOLatchEn before sleep
    slpManRegisterUsrdefinedBackupCb(appBeforeSleep,NULL);
    //slpManRegisterUsrdefinedRestoreCb(appAfterSleep,NULL);
    //end-->
//#endif
//end-->
//end-->

    simDetectSem = osSemaphoreNew(3, 0, NULL);

    // Add delay here to wait USB AT port ready, and 500ms is reliable.
    osDelay(MILLISECONDS_TO_TICKS(500));

//<!--[ODM][TaskID25305]fw@2022-11_15 add version and boot cause
#ifdef FEATURE_OPENCPU_ENABLE
    showBootInfo();
#endif
//end-->

    //When hot swap is enabled, wakeup2 is initialized again upon startup
    if (MEIG_Util_SimHotplugIsEnable())
    {
        reConfig();
    }

    while (1)
    {
        //<!--[ODM][BUGID 40230]ldw@2023-01-04 add SIM card hot pulg API
        #ifdef CONFIG_MG_SIM_HOTPLUG_SUPPORT
        //<!--[ODM][TaskID 28909]liulei@2022-11-25 add AT+MGSYS
        //<!--[ODM][BUGID 39654]liulei@2022-12-08 Modify mark of judgment
        if (osOK == osSemaphoreAcquire(simDetectSem, 0xFFFFFFFFU))
        //end-->
        {
            if (MEIG_Util_SimHotplugIsEnable())
            {
                bSimPlugIn = BSP_SimGetSimStatus();
                appSetSIMHotSwapNotify(bSimPlugIn);
                atProcSimHotSwapInd_BSP(bSimPlugIn);

                if (bSimPlugIn)
                    ECPLAT_PRINTF(UNILOG_PLA_APP, ApUserAppTask_0, P_VALUE, "SIM Plug In");
                else
                    ECPLAT_PRINTF(UNILOG_PLA_APP, ApUserAppTask_1, P_VALUE, "SIM Plug Out");
            }
        }
        #endif
        //end-->
        //end-->
        osDelay(MILLISECONDS_TO_TICKS(200));
    }
}
//end-->
//end-->

void daemonTaskInit(void)
{
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    memset(usrapp_task_stack, 0xA5,USRAPP_TASK_STACK_SIZE);
    task_attr.name = "usrapp";
    task_attr.stack_mem = usrapp_task_stack;
    task_attr.stack_size = USRAPP_TASK_STACK_SIZE;
    task_attr.priority = osPriorityNormal;
    task_attr.cb_mem = &usrapp_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block

    osThreadNew(UserAppTask, NULL, &task_attr);
}
//<!--[ODM][TaskID30898]hs@2023-03_28 add LowpowerMode
#ifdef FEATURE_LOWPOWER_AT_ENABLE
extern void startLowpowerMode(void);
#endif
//end-->

static void appInit(void *arg)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, EC_CHIP_VERSION_1, P_INFO, "%s", EC_CHIP_VERSION);
#if defined(FEATURE_CTCC_DM_ENABLE) || defined(FEATURE_CUCC_DM_ENABLE) || defined(FEATURE_CMCC_DM_ENABLE)
    ecAutoRegisterInit();
#endif

#ifdef FEATURE_CTWING_CERTI_ENABLE
    ecCtwAutoRegisterInit();
#endif

#ifdef FEATURE_MBEDTLS_ENABLE
    sslEngineInit();
#endif
#ifdef FEATURE_HTTPC_ENABLE
    httpEngineInit();
#endif

    if (BSP_UsbGetVBUSMode()==1)
    {
        usb_portmon_task_init() ;
    }

#ifdef CONFIG_MG_SHOWMAC_ESIM_SUPPORT
    {
        extern void init_physical_tcp(void);
        init_physical_tcp();
    }
#endif

#ifdef FEATURE_MEIG_AT_ENABLE
    extern void MG_YY_CfgInit(void);
    MG_YY_CfgInit();
#endif

//<!--[ODM][TaskID25305]fw@2022-09-14 add proc_main_task
#ifdef FEATURE_OPENCPU_ENABLE
    extern void MG_OPEN_NetInit(void);
    MG_OPEN_NetInit();

#ifdef FEATURE_OPENCPU_AT_ENABLE
    extern void mg_openAt_EngineCreate(void);
    mg_openAt_EngineCreate();
#endif

    extern void proc_main_task(void);
    proc_main_task();
#endif
//end-->

//<!--[ODM][BugID36102]huqing@2022-09-13 light up NET STATUS led
#ifdef CONFIG_MG_LED_SUPPORT
    drvNetModeLedOn();
    drvNetStatusLedOn(0, NET_STATUS_SEARCH);
#endif
//end-->

//<!--[ODM][BUGID 40230]ldw@2023-01-04 add SIM card hot pulg API
//#ifdef CONFIG_MG_SIM_HOTPLUG_SUPPORT
//<!--[ODM][BugID 38202]wbh@2022-11-11 Add task to monitor the status of SIM card.
    daemonTaskInit();
//end-->
//#endif
//end-->
//<!--[ODM][TaskID30898]hs@2023-03_28 add LowpowerMode
#ifdef FEATURE_LOWPOWER_AT_ENABLE
    startLowpowerMode();
#endif
//end-->
}

/**
  \fn          int main_entry(void)
  \brief       main entry function.
  \return
*/
void main_entry(void)
{
    BSP_CommonInit();
    osKernelInitialize();
    registerAppEntry(appInit, NULL);
    if (osKernelGetState() == osKernelReady)
    {
        osKernelStart();
    }
    while(1);

}
