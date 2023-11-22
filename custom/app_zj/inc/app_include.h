#ifndef _APP_INCLUDEL_H
#define _APP_INCLUDEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mg_common.h"
#include "mg_opt_event.h"
#include "mg_trace.h"
#include "mg_os.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_adc.h"
#include "mg_time.h"

#include "app_global.h"
#include "app_err.h"

#define DEBUG_ENABLE                    1
#define DEBUG_DEF_DATA                  0
#define DEBUG_GSNN_TEST_DATA            1


#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)      MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define EX_TIMEOUT_MS   (1000)

typedef enum
{
    EV_ACT_NET,             //激活网络
    EV_MQTT_CONNECT,

} app_event_e;

typedef enum
{
    NET_NO_ACT,
    NET_ACTING,
    NET_ACTED,
} app_net_state_e;



/*----------------- UTIL ------------------------------*/
typedef enum
{
    ATTR_BOOL = 1,
    ATTR_HEX,
    ATTR_SHORT,
    ATTR_INT,
    ATTR_LONG,
    ATTR_DOUBLE,
    ATTR_TIME,
    ATTR_STRING,
    ATTR_ARRAY,
    ATTR_OBJECT
} app_json_attr;


void app_util_jsonDataPack(bool bFirst,
                                   unsigned int *pOffset,
                                   unsigned char attr,
                                   const char *tag,
                                   const unsigned char *value);

void app_util_threadSleep(s32 ms, bool bLowPowerEn);

bool app_util_eventWait(ST_Thread *thread, ST_Event *event, s32 ms, bool bLowPowerEn);
bool app_util_eventSend(ST_Thread *thread, ST_Event *event, s32 ms, bool bLowPowerEn);


/*----------------- NV ------------------------------*/
void app_nvmLoadConfig(void);
void app_nvmSaveConfig(void);

/*--------------------------------------------------*/
void app_fota_task(void);
void app_fota_start(void);


void app_net_doAct(int iTimeout, app_cfg_t *pApp);
void app_net_task(void);

void app_gps_init(void);
int app_gps_getData(app_cfg_t *pApp);

void app_mqtt_task(void);


#endif

