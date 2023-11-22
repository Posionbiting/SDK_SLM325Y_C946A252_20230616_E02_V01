#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sim.h"


#define LOG_TAG MAKE_LOG_TAG('E', 'S', 'I', 'M')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

static void prvThreadEntry(void *param)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mg_sim example entry line[%d]", __LINE__);

    s32 ret = 0;
    /** get sim card status*/
    Enum_SIM_Status state = SIM_STATUS_ABSENT;
    ret = MG_SIM_GetState(SIM0, &state);
    if (state == SIM_STATUS_NORMAL)
    {
        APP_DEBUG("SIM ready", __LINE__);
    }
    else if (ret == MG_RET_OK)
    {
        APP_DEBUG("No SIM card inserted line[%d]", __LINE__);
    }
    else
    {
        APP_DEBUG("get SIM status fail line[%d]", __LINE__);
    }
    MG_OS_ThreadSleep(1000);

    /** get sim IMEI*/
    u8 imei[SIM_IMEI_SIZE]={0};
    ret = MG_SIM_GetIMEI(SIM0,imei,SIM_IMEI_SIZE);
    if(ret > 0)
    {
        APP_DEBUG("SIM IMEI line[%d],IMEI = %s,IMEI_len = %d", __LINE__, imei, ret);
    }
    else
    {
        APP_DEBUG("get IMEI err line[%d]",__LINE__);
    }
    MG_OS_ThreadSleep(1000);

    /** get sim IMSI*/
    u8 imsi[SIM_IMSI_SIZE]={0};
    ret = MG_SIM_GetIMSI(SIM0,imsi,SIM_IMSI_SIZE);
    if(ret > 0)
    {
        APP_DEBUG("SIM IMSI line[%d],IMSI = %s,IMSI_len = %d", __LINE__, imsi, ret);
    }
    else
    {
        APP_DEBUG("get IMSI err line[%d]",__LINE__);
    }
    MG_OS_ThreadSleep(1000);

    /** get sim ICCID*/
    u8 iccid[SIM_ICCID_SIZE]={0};
    ret = MG_SIM_GetICCID(SIM0,iccid,SIM_ICCID_SIZE);
    if(ret > 0)
    {
        APP_DEBUG("SIM ICCID line[%d],ICCID = %s,ICCID_len = %d", __LINE__, iccid, ret);
    }
    else
    {
        APP_DEBUG("get ICCID err line[%d]",__LINE__);
    }
    MG_OS_ThreadSleep(1000);

    APP_DEBUG("mgsim example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgsim", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}

