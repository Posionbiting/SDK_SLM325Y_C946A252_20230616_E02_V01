#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_sys.h"
#include "mg_crypto.h"
#include "mg_mqtt.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'M', 'Q')

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif


#define MQTT_CONNECT_TIMEOUT_MS (100*1000)
#define MQTT_REQUEST_TIMEOUT_MS (10*1000)

static char *ca_crt ={  \
"-----BEGIN CERTIFICATE-----\r\n"\
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\r\n"\
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\r\n"\
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\r\n"\
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\r\n"\
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\r\n"\
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\r\n"\
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\r\n"\
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\r\n"\
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\r\n"\
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\r\n"\
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\r\n"\
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\r\n"\
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\r\n"\
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\r\n"\
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\r\n"\
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\r\n"\
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\r\n"\
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\r\n"\
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\r\n"\
"-----END CERTIFICATE-----\r\n"};

static char *client_pk = {  \
"-----BEGIN RSA PRIVATE KEY-----\r\n"\
"MIIEpAIBAAKCAQEAwtYJn4TBlavzw4uUEv0p+89684Ndk9oDEfQWyRp5z/+5fJuS\r\n"\
"p4MXlKCO685UXqq57EnsgGahgpA7JI5EN9ziSWZvjV7nschIxW8VFr2iTXvHwIFR\r\n"\
"FFuVJ7ZhA5dcD1rQGKgjvutQiUEogWKhQJ8aPYPn/f3FiYEcwp6yG3t2f/V8Z3PZ\r\n"\
"WMFa3VL3oihZ+2PCOxd830jREdTyqx1S18MKPXngSznHhKq/dVz3crWquDFo3gRL\r\n"\
"uSp9ExFUeLpElLnNwFltGh0umx544TVXNXGlgyIzR9eP9RZ5Gpyi2SKq6dDWdO1L\r\n"\
"vAZPneEtrbUY/HE3UX7vu85aYb3+MvNfFpkobQIDAQABAoIBAGRf5ldDCoQ0MQIF\r\n"\
"z4Q03sM2sa3weG4qO7itGmwCcs0Afqu+bkBUXfqY+OhsxLR8mgosT2Ya+YwNCBse\r\n"\
"G/D3n7CW0FNxmEXT3AGy55dunMX0RgVb2cx2HhJ2d+ZkvO3AKEL7pGbATh5X7iky\r\n"\
"CccD8H3Ojd8hNPpRBOMS5MkZwwPKK0sLEKmmPVvoC5wY8u4m0zZV6iMpfRgBxf8+\r\n"\
"hU+xu3dIi34cAeN4wOAEJio7lw9YuC9G81uybnp1ZQZOKCulZbXfUogRam5ELUxo\r\n"\
"rBb/JCci0UkCpYgNMuWhTx2NYRbIJnwm6bFp3GNxtB5DlAKxe08ucLO1MqwEP0zy\r\n"\
"HR3oZOECgYEA5zidgNcY+MYj9qgFdcI/hpYW48jQDcWCnBxzExZ4G4EYKsigIx6K\r\n"\
"+uGXSTTFsj5TWuUt20wpBoxJWZ/sRS+xYfvWfJAGCE+rxfIsQBTCr0bM1wK7cd9Y\r\n"\
"/2Eev0wWyxcEuOCt6FDKjq793tblEq4nW6DBPivJOsjA6WiApyftUykCgYEA17c5\r\n"\
"btiS8MHhS/60RBuWdQDOs/8k2RfoDMYtrvKENTCtqYpWfmC4fLq8vWF+Ez/fURkL\r\n"\
"zF766RxKCtkF75wg//3D0WdXYAstGNmfmDILikjthKOlYfqTLpfc45UUb/nplAT0\r\n"\
"eBvAfYJkgWfNju23kOD1YO6A3PSZHy9EL4b296UCgYA7xQHJ5VBaNnXjc8l9iEn9\r\n"\
"k1y1YxqNBMpfamdAvuUrTvKdVKE4ktOgSKJf1RlpmvzXGTkCYCyWubrYRSPBOrPi\r\n"\
"vUPTjTNSAK5Ji/FuWy98OZQH3Hfx1qdd1JArKRMXSor483NhA0lpGskbSneKnaQk\r\n"\
"5mgtlwn8fRqI+2+ORh7uCQKBgQCze5cE3Jw+SALuGUniFVY1metippsBKrv/zPD7\r\n"\
"zHsmSo9kLl9THaTC2xjpNfIRkCZ+Mqm3bRu107zjk7k1qaFnfbxtQl/X4NSwBXqd\r\n"\
"xp4FfDJsF0Em6beGoALBNSxhEv4/E3RZbzXU+CYp6bWGA9KlcBeahDeiIL46zTgd\r\n"\
"l11rpQKBgQDgLtMMOY8Yw4Jp2qrgoIH4FjZ89hEgBndXcJ2CmTDBV86RUT02JK0o\r\n"\
"5pwZM1jXnk3I/Iif9LglUPBa7tDxXYZOCFrplp2Vb/OkRdgN5fotdWIPxpTdoTMl\r\n"\
"+DTFBJZ3YY3tM6EoBsxzPEYqNLZEtQUBNc0w4tCGnCW+SjrJfWYrug==\r\n"\
"-----END RSA PRIVATE KEY-----\r\n"};

static char *client_crt = { \
"-----BEGIN CERTIFICATE-----\r\n"\
"MIIDiDCCAnCgAwIBAgIIAOAGw8oC0kQwDQYJKoZIhvcNAQELBQAwUzEoMCYGA1UE\r\n"\
"AwwfQWxpYmFiYSBDbG91ZCBJb1QgT3BlcmF0aW9uIENBMTEaMBgGA1UECgwRQWxp\r\n"\
"YmFiYSBDbG91ZCBJb1QxCzAJBgNVBAYTAkNOMCAXDTIxMTIyMzAyNDE0OVoYDzIx\r\n"\
"MjExMjIzMDI0MTQ5WjBRMSYwJAYDVQQDDB1BbGliYWJhIENsb3VkIElvVCBDZXJ0\r\n"\
"aWZpY2F0ZTEaMBgGA1UECgwRQWxpYmFiYSBDbG91ZCBJb1QxCzAJBgNVBAYTAkNO\r\n"\
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwtYJn4TBlavzw4uUEv0p\r\n"\
"+89684Ndk9oDEfQWyRp5z/+5fJuSp4MXlKCO685UXqq57EnsgGahgpA7JI5EN9zi\r\n"\
"SWZvjV7nschIxW8VFr2iTXvHwIFRFFuVJ7ZhA5dcD1rQGKgjvutQiUEogWKhQJ8a\r\n"\
"PYPn/f3FiYEcwp6yG3t2f/V8Z3PZWMFa3VL3oihZ+2PCOxd830jREdTyqx1S18MK\r\n"\
"PXngSznHhKq/dVz3crWquDFo3gRLuSp9ExFUeLpElLnNwFltGh0umx544TVXNXGl\r\n"\
"gyIzR9eP9RZ5Gpyi2SKq6dDWdO1LvAZPneEtrbUY/HE3UX7vu85aYb3+MvNfFpko\r\n"\
"bQIDAQABo2AwXjAfBgNVHSMEGDAWgBSKN5uocM3V+UjIl34hn1lvSY4kETAdBgNV\r\n"\
"HQ4EFgQUJ0J8bjJ6ZmiiI1yH+tj05CdRl3YwDgYDVR0PAQH/BAQDAgP4MAwGA1Ud\r\n"\
"EwEB/wQCMAAwDQYJKoZIhvcNAQELBQADggEBAHQzNdUdVDtvcrrxTjoTtqVK5+o8\r\n"\
"osbog6iKTdWbuVNCalTocJ72DK+hGOxgnz31HajU3uE9042V8hQubObLBfIPRYVs\r\n"\
"Tm0AzH8ehTLjDM60urQjBs/c8i4VpWUP4mW6pxt/rf9ewCycYC9MfUXighLjCjzc\r\n"\
"jFpXSAHh7T5Om+Z2F6jzfETYGu4qoxNOWFiut2Ls8xfoujYvQBjhYyd6lqEGm4zx\r\n"\
"DzJDwKQEacdxNt616c8UHcyzympfzBaDOndsprfZEK+a1MaGROXPSKxIZKMJlqOw\r\n"\
"DLOwE343+eHunCaNsPS5poH65lk77rOv2uhccWnIe493w2uool55RL5tc5I=\r\n"\
"-----END CERTIFICATE-----\r\n"};

/*
ProductKey ： a1t29S5DMq0
DeviceName : mgx509test
topic: /a1t29S5DMq0/mgx509test/user/TEST
*/
#define MQTT_CLIENT_ID  "mqttclientExample|securemode=2,signmethod=hmacsha1|"
#define MQTT_CLIENT_USERNAME  "mgx509test&a1t29S5DMq0"
#define MQTT_CLIENT_HOSTNAME  "x509.itls.cn-shanghai.aliyuncs.com"
#define MQTT_CLIENT_PORT  1883
#define MQTT_CLIENT_SUBSCRIBE_TOPIC  "/a1t29S5DMq0/mgx509test/user/TEST"
#define MQTT_CLIENT_PUBLISH_TOPIC  "/a1t29S5DMq0/mgx509test/user/TEST"
#define MQTT_CLIENT_PUBLISH_MSG  "This is test data, hello MQTT"


#define MQTT_BUFFER_MAX_LEN       1024
u8 g_buf[MQTT_BUFFER_MAX_LEN] = {0};

void MG_mqtt_mqtt_incoming_topic_and_data_cb(void *arg, const u8 *topic, u32 topic_len, const u8 *data, u32 date_len)
{
    MG_UTILS_Memset(g_buf, 0, MQTT_BUFFER_MAX_LEN);
    MG_UTILS_Memcpy(g_buf, topic, topic_len);
    APP_DEBUG("mqtt incoming topic line[%d]topic_len:%d, topic:%s", __LINE__, topic_len , g_buf);
    MG_UTILS_Memset(g_buf, 0, MQTT_BUFFER_MAX_LEN);
    MG_UTILS_Memcpy(g_buf, data, date_len);
    APP_DEBUG("mqtt incoming topic line[%d]date_len:%d, date:%s", __LINE__, date_len , g_buf);
}


// auto activate (cid = 1)
static BOOL _doAct(void)
{
    s32 iResult = 0;

    /*get csq*/
    u8 nSignalLevel = 99;
    iResult = MG_NW_GetSignalQuality(SIM0, &nSignalLevel);
    APP_DEBUG("get csq[%d] iResult:%d, nSignalLevel:%d", __LINE__, iResult, nSignalLevel);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, PDP_CID_1, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    return FALSE;
}

// Activate manually
static BOOL _doActEx(u8 cid)
{
    s32 iResult = 0;

    if ( cid == 1 )
    {
        APP_DEBUG("cid 1 is automatically activated[%d]", __LINE__);
        return FALSE;
    }

    /*get csq*/
    u8 nSignalLevel = 99;
    iResult = MG_NW_GetSignalQuality(SIM0, &nSignalLevel);
    APP_DEBUG("get csq[%d] iResult:%d, nSignalLevel:%d", __LINE__, iResult, nSignalLevel);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*get network state*/
    Enum_NwState nNwState = NW_STATE_NO_REGISTERED;
    iResult = MG_NW_GetNetWorkState(SIM0, cid, &nNwState);
    APP_DEBUG("get network state[%d] iResult:%d, nNwState:%d", __LINE__, iResult, nNwState);
    if (nNwState == NW_STATE_NO_REGISTERED)
    {
        return FALSE;
    }
    else if (nNwState == NW_STATE_LTE_ACTIVED)
    {
        return TRUE;
    }

    /*set default network cofig*/
    iResult = MG_NW_Config(SIM0, cid, NULL);
    APP_DEBUG("cid %d set default network cofig[%d] iResult:%d", cid, __LINE__, iResult);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    /*do act, timeout 10s*/
    iResult = MG_NW_Activate(SIM0, cid, 10*1000);
    APP_DEBUG("cid %d do act[%d] iResult:%d", cid, __LINE__, iResult);
    if (iResult != MG_RET_OK)
    {
        return FALSE;
    }
    MG_OS_ThreadSleep(100);

    return TRUE;
}


static void prvThreadEntry(void *param)
{
    s32 iResult = 0;
    BOOL isAct = FALSE;
    u8 nCurcid = 0;
    APP_DEBUG("mgmqtt example entry line[%d]", __LINE__);

    //MG_TRACE_SetLogLevel(MG_TRACE_LOG_LEVEL_VERBOSE);
    MG_OS_ThreadSleep(10000);

    /*do act 1 */
    isAct = FALSE;
    while (!isAct)
    {
        isAct = _doAct();
        APP_DEBUG("_doAct[%d] cid:1, isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }
    /*do act 2 */
    isAct = FALSE;
    while (!isAct)
    {
        isAct = _doActEx(PDP_CID_2);
        APP_DEBUG("_doAct[%d] cid:2, isAct:%d", __LINE__, isAct);
        MG_OS_ThreadSleep(1000);
    }

    /*select current cid*/
    nCurcid = PDP_CID_1; //PDP_CID_2


	/* create mqtt client*/
    ST_MqttClient * MqttClient = NULL;
    ST_MqttClientInfo MqttClientInfo = {0};
    MqttClientInfo.vsn = MQTT_TLS_VERSION_TLS1_2;
    MqttClientInfo.cacert = ca_crt;
    MqttClientInfo.clicert = client_crt;
    MqttClientInfo.pkey = client_pk;
    do
    {
        MqttClient = MG_MQTT_Create(&MqttClientInfo);
        APP_DEBUG("mqtt Create line[%d] MqttClient :%x", __LINE__, MqttClient);
        MG_OS_ThreadSleep(1000);
    }while(!MqttClient);

    iResult = MG_MQTT_SetOpt(MqttClient, MQTT_OPT_BIND_CID_SET, &nCurcid, sizeof(u8));
    APP_DEBUG("Set OPT cid %d line[%d] iResult :%d", nCurcid, __LINE__, iResult);
    MG_OS_ThreadSleep(1000);

    /* set mqtt option*/
    //enable tls
    u8 nTlsConnectEn = MQTT_OPT_CLIENT_ENABLE;
    iResult = MG_MQTT_SetOpt(MqttClient, MQTT_OPT_CLIENT_TLS, &nTlsConnectEn, sizeof(u8));
    APP_DEBUG("Set OPT TLS CONNECT line[%d] iResult :%d", __LINE__, iResult);
    MG_OS_ThreadSleep(1000);
    //enable verify required
    u8 nTlsVerify = MQTT_OPT_CLIENT_TLS_VERIFY_REQUIRED;
    iResult = MG_MQTT_SetOpt(MqttClient, MQTT_OPT_CLIENT_TLS_VERIFY, &nTlsVerify, sizeof(u8));
    APP_DEBUG("Set OPT CLIENT VERIFY line[%d] iResult :%d", __LINE__, iResult);
    MG_OS_ThreadSleep(1000);

    bool isErr = FALSE;
    bool isConnect = FALSE;
    u32 nPulishMsgCount = 0;
    ST_MqttConnectCtx MqttClientConnectCtx = {0};
    while(1)
    {
        MG_OS_ThreadSleep(1000);

        if(!isConnect)
        {
                    /* mqtt Register Subscribe callback */
            iResult = MG_MQTT_RegisterEx(MqttClient,
                        (mqtt_incoming_topic_and_data_cb)MG_mqtt_mqtt_incoming_topic_and_data_cb,
                        NULL);
            if(iResult != MG_RET_OK)
            {
                APP_DEBUG("mqtt Register fail line[%d] iResult:%d", __LINE__, iResult);
                isErr = TRUE;
            }

            /* mqtt connect */
            MG_UTILS_Memset(&MqttClientConnectCtx ,0, sizeof(ST_MqttConnectCtx));
            MqttClientConnectCtx.client_id = (char *)MQTT_CLIENT_ID;
            MqttClientConnectCtx.hostname = (char *)MQTT_CLIENT_HOSTNAME;
            MqttClientConnectCtx.port = (u16)MQTT_CLIENT_PORT;
            MqttClientConnectCtx.username = (char *)MQTT_CLIENT_USERNAME;
            MqttClientConnectCtx.password = NULL;
            MqttClientConnectCtx.keep_alive = 300;
            MqttClientConnectCtx.clean_session = 0,

            iResult = MG_MQTT_Connect(MqttClient, &MqttClientConnectCtx, MQTT_CONNECT_TIMEOUT_MS);
            if(iResult != MG_RET_OK)
            {
                APP_DEBUG("mqtt Connect fail line[%d] iResult:%d", __LINE__, iResult);
                continue;
            }
            isConnect = TRUE;

            /* mqtt Subscribe topic*/
            if(isConnect && !isErr)
            {
                iResult = MG_MQTT_Subscribe(MqttClient,
                            (char *)MQTT_CLIENT_SUBSCRIBE_TOPIC,
                            MQTT_QOS_0,
                            MQTT_REQUEST_TIMEOUT_MS);
                if(iResult != MG_RET_OK)
                {
                    APP_DEBUG("mqtt Subscribe fail line[%d] iResult:%d", __LINE__, iResult);
                    isErr = TRUE;
                }
            }
            nPulishMsgCount = 0;
        }

        /* ckeck mqtt connect state*/
        if(isConnect && !isErr)
        {
            if(!MG_MQTT_ClientIsConnected(MqttClient))
            {
                APP_DEBUG("mqtt is disconnected line[%d]", __LINE__);
                isConnect = FALSE;
            }
        }

        /* mqtt publish msg*/
        if(isConnect && !isErr)
        {
            iResult = MG_MQTT_Publish(MqttClient,
                        (char *)MQTT_CLIENT_PUBLISH_TOPIC,
                        (u8 *)MQTT_CLIENT_PUBLISH_MSG,
                        MG_UTILS_Strlen((char *)MQTT_CLIENT_PUBLISH_MSG),
                        MQTT_PUBLISH_DUP_0,
                        MQTT_QOS_0,
                        MQTT_PUBLISH_RETAIN_0,
                        MQTT_REQUEST_TIMEOUT_MS);
            if(iResult != MG_RET_OK)
            {
                APP_DEBUG("mqtt pulish fail line[%d] iResult:%d", __LINE__, iResult);
                isErr = TRUE;
            }
            nPulishMsgCount++;
        }

        /* mqtt Unsubscribe and reconnect when nPulishMsgCount Up to 5*/
        if(isConnect && !isErr)
        {
            if(nPulishMsgCount == 5)
            {
                iResult = MG_MQTT_Unsubscribe(MqttClient,
                            (char *)MQTT_CLIENT_SUBSCRIBE_TOPIC,
                            MQTT_REQUEST_TIMEOUT_MS);
                if(iResult != MG_RET_OK)
                {
                    APP_DEBUG("mqtt Unsubscribe fail line[%d] iResult:%d", __LINE__, iResult);
                    isErr = TRUE;
                }
                isConnect = FALSE;
            }
        }

        APP_DEBUG("mqtt pulish count line[%d] nPulishMsgCount:%d", __LINE__, nPulishMsgCount);

        /*Check whether to reconnect*/
        if(!isConnect || isErr)
        {
            MG_MQTT_Disconnect(MqttClient);
            isErr = FALSE;
            isConnect = FALSE;
        }
    }

    MG_MQTT_Destroy(MqttClient);

    APP_DEBUG("mg example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void proc_main_task(void)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgmqtt", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 4, 4);
}


