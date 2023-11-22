#include "app_include.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

#if DEBUG_DEF_DATA
static void app_load_debug_data(void)
{
    sprintf((char *)g_app_cfg.nvConfig.sn, "1430-100001");
    sprintf((char *)g_app_cfg.fotaFileDownloadUrl, "https://tankmiles-dei-sz.oss-cn-shenzhen.aliyuncs.com/mqtt/ZJ_115_116.par");
}
#endif

static void app_init(void)
{
    memset(&g_app_cfg, 0, sizeof(g_app_cfg));
    app_nvmLoadConfig();
    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);
    MG_ADC_channelInit(MG_ADC_CHANNEL_VBAT);

#if DEBUG_DEF_DATA
    app_load_debug_data();
#endif

    MG_SYSTEM_GetModuleInfo(g_app_cfg.devInfo.sysVer, sizeof(g_app_cfg.devInfo.sysVer));

    // network
    g_app_cfg.netInfo.netType           = NET_TYPE_LTE;
    g_app_cfg.netInfo.simId             = SIM0;
    g_app_cfg.netInfo.cid               = PDP_CID_1;

    // fota
    g_app_cfg.isSysUpdate               = false;
    g_app_cfg.bLowPowerModeEn           = false;
}

int app_check(void)
{
    // 无SN情况下不进行业务操作
    if (strlen((char *)g_app_cfg.nvConfig.sn) <= 0)
        return false;
    // 无KEY情况下不进行业务操作
    if (strlen((char *)g_app_cfg.nvConfig.key) <= 0)
        return false;
    // 无SECRET情况下不进行业务操作
    if (strlen((char *)g_app_cfg.nvConfig.secret) <= 0)
        return false;
    return true;
}

void app_main(void)
{
    app_init();         // init param
    if (app_check())
    {
        app_net_task();     // network
        app_fota_task();    // fota
        app_mqtt_task();    // mqtt
    }
}

