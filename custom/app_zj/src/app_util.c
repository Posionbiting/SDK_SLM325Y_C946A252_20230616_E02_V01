#include "app_include.h"
#include <math.h>

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')


union TMP_UBUF
{
    unsigned char   ucData;
    unsigned short  usData;
    unsigned int    uiData;
    unsigned long   ulData;
    double          dData;
};

static void _app_util_dtostr(double data, int bit, int *x, int *y)
{
    double precious = 0.0;
    double inter = 0;
    int cout = 1;
    int i;
    int m_x = 0, m_y = 0;

    precious = modf(data, &inter);
    for (i = 0; i < bit; i++)
    {
        cout *= 10;
    }
    m_x = (int)inter;
    m_y = (int)(precious * cout);

    *x = m_x;
    *y = m_y < 0 ? 0 : m_y;
}

void app_util_jsonDataPack(bool bFirst, unsigned int *pOffset, unsigned char attr, const char *tag, const unsigned char *value)
{
    unsigned int mLen;
    union TMP_UBUF mBuff;

    mLen = *pOffset;

    if (bFirst != true && attr != ATTR_ARRAY)
    {
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, ",");
        mLen += 1;
        *pOffset += 1;
    }

    switch(attr)
    {
    case ATTR_ARRAY:
        if (bFirst == false)
        {
            snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "},\"%s\":%s", tag, "{");
        }
        else
        {
            sprintf((char *)&value[strlen((char *)value)], "}");
            snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%s", tag, "{");
        }
        break;
    case ATTR_BOOL:
        memcpy(&mBuff.ucData, value, sizeof(unsigned char));
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%s", tag, mBuff.ucData != 1 ? "false" : "true");
        break;
    case ATTR_HEX:
        memcpy(&mBuff.ucData, value, sizeof(unsigned char));
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%d", tag, mBuff.ucData);
        break;
    case ATTR_SHORT:
        memcpy(&mBuff.usData, value, sizeof(unsigned short));
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%d", tag, mBuff.usData);
        break;
    case ATTR_INT:
        memcpy(&mBuff.uiData, value, sizeof(unsigned int));
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%d", tag, mBuff.uiData);
        break;
    case ATTR_LONG:
        memcpy(&mBuff.ulData, value, sizeof(unsigned long));
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%d", tag, mBuff.ulData);
        break;
    case ATTR_DOUBLE:
    {
        int x = 0, y = 0;
        memcpy(&mBuff.dData, value, sizeof(double));

        if (!strcmp(tag, "LAT") || !strcmp(tag, "LON"))
        {
            _app_util_dtostr(mBuff.dData, 5, &x, &y);
        }
        else
        {
            _app_util_dtostr(mBuff.dData, 0, &x, &y);
        }

        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":%d.%d", tag, x, y);
        break;
    }
    default:
        snprintf((char *)(g_app_tmpBuff+mLen), sizeof(g_app_tmpBuff) - mLen, "\"%s\":\"%s\"", tag, value);
        break;
    }
    *pOffset += strlen((char *)(g_app_tmpBuff + mLen));
}

void app_util_threadSleep(s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        MG_OS_ThreadSleepLowPower(ms);
    else
        MG_OS_ThreadSleep(ms);
}

bool app_util_eventWait(ST_Thread *thread, ST_Event *event, s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_OS_EventWaitLowPower(thread, event);
    else
        return MG_OS_EventWait(thread, event, ms);
}

bool app_util_eventSend(ST_Thread *thread, ST_Event *event, s32 ms, bool bLowPowerEn)
{
    if (bLowPowerEn)
        return MG_OS_EventSendLowPower(thread, event);
    else
        return MG_OS_EventSend(thread, event, ms);
}

