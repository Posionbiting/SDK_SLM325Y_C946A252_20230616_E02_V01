#ifndef _CTW_HTTP_H_
#define _CTW_HTTP_H_

#include <stdlib.h>
#include <string.h>
#include "HTTPClient.h"
#include "ctw_common.h"
#include "cJSON.h"

typedef enum
{
    HTTP_INIT_STATE,
    HTTP_IPREADY_STATE,
    HTTP_REG_STATE,
    HTTP_KEEP_ONLINE_STATE
} ctwHttpStateMachine_t;
    
typedef struct{
    char host[48];
    uint32_t timestamp;
    char signature[65];
    char iccid[CTW_CCID_LEN+1];
    char imsi[CTW_IMSI_LEN+1];
    char imei[CTW_IMEI_LEN+1];
    INT16 rsrp;
    INT8  snr;
    INT8  txPower;
    uint16_t phyCellId;
}ctwHttpRegParam_t;

void ctwHttpCalcSign(char* outSign, uint32_t* outTimestamp, char* masterKey);
uint8_t ctwHttpAuthReg(HttpClientContext* pHttpClient, MWNvmCfgCtwHttpParam* pCtwHttpParam, ctwHttpRegParam_t* pRegParam);
uint8_t ctwHttpSendData(HttpClientContext* pHttpClient, MWNvmCfgCtwHttpParam* pCtwHttpParam, ctwHttpRegParam_t* pRegParam, char* topic, char* data);
void ctwHttpRegisterInit(MWNvmCfgCtwHttpParam* pCtwHttpParam);

#endif//_CTW_HTTP_H_

