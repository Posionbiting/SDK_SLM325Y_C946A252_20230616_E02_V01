/****************************************************************************
 *
 * Copy right:   2017-, Copyrigths of EigenComm Ltd.
 * File name:    app.c
 * Description:  EC618 at command demo entry source file
 * History:      Rev1.0   2018-07-12
 *
 ****************************************************************************/
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
#include "slpman.h"
#include "ps_lib_api.h"

// at cmd task static stack and control block
#define USRAPP_TASK_STACK_SIZE        (1024)

#define SLPMAN_EXAMPLE_1    1
#define SLPMAN_EXAMPLE_2    2
#define SLPMAN_EXAMPLE_3    3
#define SLPMAN_EXAMPLE_4    4


static StaticTask_t             usrapp_task;
static uint8_t                  usrapp_task_stack[USRAPP_TASK_STACK_SIZE];
static uint8_t exampleNum;

#define EXAMPLE_NUM_SET(a)  (exampleNum = a)


static uint8_t slpmanSlp2Handler           = 0xff;
static uint8_t slpmanSlp1Handler           = 0xff;


#define SLPMAN_EXAMPLE_INDEX  SLPMAN_EXAMPLE_2

static void appBeforeSleep(void *pdata, slpManLpState state)
{
    // enable aonio latch
    slpManAONIOLatchEn(true);

    switch(state)
    {
        case SLPMAN_SLEEP1_STATE:
            ECPLAT_PRINTF(UNILOG_PLA_APP, appBeforeSleep_1, P_SIG, "Before Sleep1");
            break;
        case SLPMAN_SLEEP2_STATE:
            ECPLAT_PRINTF(UNILOG_PLA_APP, appBeforeSleep_2, P_SIG, "Before Sleep2");
            break;
        case SLPMAN_HIBERNATE_STATE:
            ECPLAT_PRINTF(UNILOG_PLA_APP, appBeforeSleep_3, P_SIG, "Before Hibernate");
            break;
        default:
            break;
    }
}

static void appAfterSleep(void *pdata, slpManLpState state)
{
    switch(state)
    {
        case SLPMAN_SLEEP1_STATE:
            ECPLAT_PRINTF(UNILOG_PLA_APP, appAfterSleep_1, P_SIG, "After Sleep1");
            break;
        case SLPMAN_SLEEP2_STATE:
            ECPLAT_PRINTF(UNILOG_PLA_APP, appAfterSleep_2, P_SIG, "After Sleep2");
            break;
        case SLPMAN_HIBERNATE_STATE:
            ECPLAT_PRINTF(UNILOG_PLA_APP, appAfterSleep_3, P_SIG, "After Hibernate");
            break;
        default:
            break;
    }
}

static void UserAppTask(void *arg)
{
    uint32_t cnt;
    slpManRet_t ret;
    slpManWakeSrc_e wakeupSrc;
    

    // choose a test case:
    EXAMPLE_NUM_SET(SLPMAN_EXAMPLE_INDEX);

    slpManSetPmuSleepMode(true,SLP_HIB_STATE,false);

    slpManApplyPlatVoteHandle("SLP1Vote",&slpmanSlp1Handler);            // apply an app layer vote handle for sleep1
    slpManApplyPlatVoteHandle("SLP2Vote",&slpmanSlp2Handler);            // apply an app layer vote handle for sleep2

    slpManRegisterUsrdefinedBackupCb(appBeforeSleep,NULL);
    slpManRegisterUsrdefinedRestoreCb(appAfterSleep,NULL);

    slpManSlpState_t slpstate = slpManGetLastSlpState();

    if(slpstate == SLP_ACTIVE_STATE)        // from power on
    {
        ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_0, P_SIG, 1, "Wakeup from Power On, and Set CFUN=0");
        appSetCFUN(0);
    }
    else if((slpstate == SLP_SLP2_STATE) || (slpstate == SLP_HIB_STATE))     // wakeup from sleep
    {
        ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_1, P_SIG, 1, "Wakeup From state = %u", slpstate);

        if(exampleNum == SLPMAN_EXAMPLE_2)
        {
            wakeupSrc = slpManGetWakeupSrc();
            ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_80, P_SIG, 1, "Wakeup Source is = %e<slpManWakeSrc_e>", wakeupSrc);
        }
    }

    while(1)
    {
        switch(exampleNum)
        {
            case SLPMAN_EXAMPLE_1:          // test for sleep vote
            {
                slpManSlpState_t State;
                uint8_t vote_cnt;

                ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_2, P_SIG, 0, "Example Step1: Disable Sleep1/2 to keep in IDLE");
                ret = slpManPlatVoteDisableSleep(slpmanSlp1Handler, SLP_SLP1_STATE);
                EC_ASSERT(ret == RET_TRUE, ret, 0, 0);
                ret = slpManPlatVoteDisableSleep(slpmanSlp2Handler, SLP_SLP2_STATE);  // sleep deeper than sleep2(include) is prohibited
                EC_ASSERT(ret == RET_TRUE, ret, 0, 0);

                cnt = 0;
                while(cnt<2)
                {
                    cnt++;
                    osDelay(3000);
                }
                ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_3, P_SIG, 0, "Example 1: Enable sleep1 to enter sleep1");
                ret = slpManPlatVoteEnableSleep(slpmanSlp1Handler, SLP_SLP1_STATE);
                EC_ASSERT(ret == RET_TRUE, ret, 0, 0);

                cnt = 0;
                while(cnt<5)
                {
                    cnt++;
                    osDelay(3000);
                }

                ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_4, P_SIG, 0, "Example 1: We can check vote state and vote counter");

                if(slpManCheckVoteState(slpmanSlp2Handler, &State, &vote_cnt)==RET_TRUE)
                {
                    ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_5, P_SIG, 2, "Check Vote State, state=%u, cnt=%u",State,vote_cnt);
                }

                slpManPlatVoteEnableSleep(slpmanSlp2Handler, SLP_SLP2_STATE);  // cancel the prohibition of sleep2

                if(slpManCheckVoteState(slpmanSlp2Handler, &State, &vote_cnt)==RET_TRUE)
                {
                    ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_6, P_SIG, 2, "Check Vote State Again, state=%u, cnt=%u",State,vote_cnt);
                }

                ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_7, P_SIG, 0, "Example 1: Now we can enter Hibernate");

                while(1)
                {
                    osDelay(3000);
                }
                break;
            }
            case SLPMAN_EXAMPLE_2:          // test for external wakeup
            {
                ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_81, P_SIG, 0, "Example 2: Enter Hibernate and test for wakeup");
                void WakeupPadInit(void);
                WakeupPadInit();
                if(slpManGetLastSlpState() == SLP_ACTIVE_STATE)         // create timer when first power on
                {
                    slpManDeepSlpTimerStart(DEEPSLP_TIMER_ID0, 15000);
                    slpManDeepSlpTimerStart(DEEPSLP_TIMER_ID3, 40000);
                    slpManDeepSlpTimerStart(DEEPSLP_TIMER_ID6, 300000);     
                }
                while(1)
                {
                    osDelay(3000);
                }
                break;
            }
            case SLPMAN_EXAMPLE_4:
            {
                ECOMM_TRACE(UNILOG_PLA_APP, UserAppTask_9, P_SIG, 1, "Example 4: Empty");
                
                while(1)
                {
                    osDelay(3000);
                }
                break;
            }
        }
    }
}

void app_gps_init(void);
void app_test_init(void *arg)
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

    app_gps_init();
    osThreadNew(UserAppTask, NULL, &task_attr);
}
