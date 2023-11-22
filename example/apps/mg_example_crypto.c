/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_crypto.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'C', 'R')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

char *msg = "clientIdmqttclientExampledeviceNameMgNameproductKeya1A6f5k6fLm";
char *key = "569c0ad14f20c07064e4ee8d4a5f5416";

char digestMD5[HMAC_MD5_DIGEST_STRING_LEN] = {0};
char digestSHA1[HMAC_SHA1_DIGEST_STRING_LEN] = {0};
char digestSHA256[HMAC_SHA256_DIGEST_STRING_LEN] = {0};

static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgcrypto example entry line[%d]", __LINE__);

    u32 msgLen = MG_UTILS_Strlen(msg);
    u32 keyLen = MG_UTILS_Strlen(key);
    //HmacMD5
    iResult = MG_CRYPTO_HmacMD5(msg, msgLen, digestMD5, HMAC_MD5_DIGEST_STRING_LEN, key, keyLen);
    if(iResult == MG_RET_OK)
    {
        APP_DEBUG("CRYPTO line[%d] MD5 %s", __LINE__, digestMD5);
    }
    MG_OS_ThreadSleep(1000);
    //HmacSHA1
    iResult = MG_CRYPTO_HmacSHA1(msg, msgLen, digestSHA1, HMAC_SHA1_DIGEST_STRING_LEN, key, keyLen);
    if(iResult == MG_RET_OK)
    {
        APP_DEBUG("CRYPTO line[%d] SHA1 %s", __LINE__, digestSHA1);
    }
    MG_OS_ThreadSleep(1000);
    //HmacSHA256
    iResult = MG_CRYPTO_HmacSHA256(msg, msgLen, digestSHA256, HMAC_SHA256_DIGEST_STRING_LEN, key, keyLen);
    if(iResult == MG_RET_OK)
    {
        APP_DEBUG("CRYPTO line[%d] SHA256 %s", __LINE__, digestSHA256);
    }
    MG_OS_ThreadSleep(1000);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgntp", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}


