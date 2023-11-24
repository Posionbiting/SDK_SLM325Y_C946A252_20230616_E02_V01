#include "app_include.h"
#include <math.h>
#include "mg_gpio.h"
#include "mg_gnss.h"
#include "mg_http.h"
#include "mg_network.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

#define GPS_ANT_PIN                                 PIN40
#define LBS_LONGTITUDE_LATITUDE_SIZE                20


static ST_nmeaINFO nmeaINFO;

#if DEBUG_GSNN_TEST_DATA
char *gnss_str = {"$GNRMC,104738.00,A,3107.86745,N,12121.36045,E,10.000,,160623,,,A,V*15\r\n"
            "$GNGGA,104738.00,3107.86745,N,12121.36045,E,1,12,3.28,55.4,M,,M,,*63\r\n"
            "$GNGLL,3107.86745,N,12121.36045,E,104738.00,A,A*76\r\n"
            "$GNGSA,A,3,05,20,24,15,13,23,35,36,,,,,5.12,3.28,3.94,1*05\r\n"
            "$GNGSA,A,3,01,03,06,12,,,,,,,,,5.12,3.28,3.94,4*02\r\n"
            "$GPGSV,2,1,08,05,38,088,46,13,38,038,34,15,64,004,28,20,18,107,44,0*62\r\n"
            "$GPGSV,2,2,08,23,27,310,36,24,52,175,47,35,53,169,41,36,40,175,48,0*6A\r\n"
            "$GBGSV,2,1,06,01,47,139,43,03,53,201,37,04,,,44,06,52,206,39,0*45\r\n"
            "$GBGSV,2,2,06,07,,,33,12,15,183,28,0*41\r\n"
            "$GNVTG,,T,,M,0.000,N,0.000,K,A*3D\r\n"};
#endif

int app_gps_openGpsANT(void)
{
    int iRet = MG_RET_OK;

    //MG_GPIO_Init(GPS_ANT_PIN, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_LOW, MG_GPIO_PULL_UP);
    iRet = MG_GPIO_SetLevel(GPS_ANT_PIN, MG_GPIO_LEVEL_HIGH);

    APP_DEBUG("[%d]GPS open gps ant, %d", __LINE__, iRet);
    return iRet;
}

int app_gps_closeGpsANT(void)
{
    int iRet = MG_RET_OK;

    iRet = MG_GPIO_SetLevel(GPS_ANT_PIN, MG_GPIO_LEVEL_LOW);

    APP_DEBUG("[%d]GPS close gps ant, %d", __LINE__, iRet);
    return iRet;
}

static char* app_gps_dms2d(double val)
{
    // original value: +/-[degree][min].[sec/60]
    val = val /100;
    double int_part;
    double fra_part;
    char* nval;
    nval = malloc(20);
    memset(nval, 0, 20);    
    fra_part = modf(val, &int_part);
    fra_part = ((fra_part * 100) / 60);
    sprintf(nval,"%d.%d",(int)int_part,(int)fra_part);
    return nval;
}

static char *dmTod(double num) {
    
    char *str;
    str = malloc(20);
    memset(str, 0, 20);
    int precision = 7;
    int intPart = (int) (num / 100); // 整数部分
    double decimalPart = (((num / 100) - intPart) * 100) / 60; // 小数部分

    // 将整数部分转换为字符串
    int i = 0;
    while (intPart > 0) {
        str[i++] = (intPart % 10) + '0';
        intPart /= 10;
    }

    // 反转整数部分字符串
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }

    // 如果有小数部分，则添加小数点和小数部分
    if (precision > 0) {
        str[i++] = '.';
        decimalPart *= 10; // 将小数部分扩大10倍，以便取出每一位
        for (int j = 0; j < precision; j++) {
            int digit = (int) decimalPart;
            str[i++] = digit + '0';
            decimalPart -= digit;
            decimalPart *= 10;
        }
    }

    // 添加字符串结束标志
    str[i] = '\0';

    return str;
}

static char* app_gps_evl2str(double val)
{
    double int_part;
    double fra_part;
    char *nstr;
    nstr = malloc(20);
    memset(nstr, 0, 20);
    fra_part = modf(val, &int_part);
    fra_part = (fra_part) * 10000;
    sprintf(nstr,"%d.%d",(int)int_part,(int)(abs(fra_part)));
    return nstr;
}

static char* app_gps_speed2str(double val)
{
    char *convert_str;
    convert_str = malloc(20);
    memset(convert_str, 0, 20);
    int precision = 7;
    double int_part;
    double fra_part;
    fra_part = modf(val, &int_part);
    int intPart = (int) (int_part); // 整数部分
    double decimalPart = fra_part; // 小数部分

    // 将整数部分转换为字符串
    int i = 0;
    while (intPart > 0) {
        convert_str[i++] = (intPart % 10) + '0';
        intPart /= 10;
    }

    // 反转整数部分字符串
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = convert_str[j];
        convert_str[j] = convert_str[len - j - 1];
        convert_str[len - j - 1] = temp;
    }

    // 如果有小数部分，则添加小数点和小数部分
    if (precision > 0) {
        convert_str[i++] = '.';
        decimalPart *= 10; // 将小数部分扩大10倍，以便取出每一位
        for (int j = 0; j < precision; j++) {
            int digit = (int) decimalPart;
            convert_str[i++] = digit + '0';
            decimalPart -= digit;
            decimalPart *= 10;
        }
    }

    // 添加字符串结束标志
    convert_str[i] = '\0';

    return convert_str;
}


static int app_gps_getGnssData(int iTimeout, app_cfg_t *pApp)
{
    int iRet = MG_RET_ERR;
    int i = iTimeout/(1000);
    APP_DEBUG("[%d]GPS get gnss data.", __LINE__);

    //开GPS天线
    iRet = app_gps_openGpsANT();
    //Open the GNSS system.
    if (!MG_GNSS_Open()){
        APP_DEBUG("[%d]Filed to open the GNSS system.", __LINE__);
    } else {
        APP_DEBUG("[%d]Successfully opened the GNSS system.", __LINE__);
    }
    app_util_threadSleep(1000, pApp->bLowPowerModeEn);
    do
    {
        iRet = MG_RET_ERR;
        if (pApp->netState != NET_ACTED)
            break;

        if (MG_GNSS_GetStatus() != true)
        {
            APP_DEBUG("[%d]GNSS system is closed.", __LINE__);
            //MG_GNSS_DebugConfig(GNSS_DBG_USB_NMEA);
            // if (!MG_GNSS_Open()){
            //     APP_DEBUG("[%d]Filed to open the GNSS system.", __LINE__);
            // } else {
            //     APP_DEBUG("[%d]Successfully opened the GNSS system.", __LINE__);
            // }
        }
        else
        {
            APP_DEBUG("[%d]GPS get gnss data.", __LINE__);
            memset(&nmeaINFO, 0, sizeof(nmeaINFO));
            memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));

            MG_GNSS_GetNmeaInfo((u8 *)g_app_tmpBuff, sizeof(g_app_tmpBuff));
#if DEBUG_GSNN_TEST_DATA
            if (MG_GNSS_NmeaParse((u8 *)gnss_str, &nmeaINFO) == true)
#else
            if (MG_GNSS_NmeaParse((u8 *)g_app_tmpBuff, &nmeaINFO) == true)
#endif
            {
                if (nmeaINFO.sig)
                {
                    ST_Time m_time;

                    APP_DEBUG("[%d]GPS FIX STATUS:%d.", __LINE__, nmeaINFO.sig);
                    APP_DEBUG("[%d]GPS get gnss data, lat:%d, lon:%d", __LINE__, (int)(nmeaINFO.lat*100000), (int)(nmeaINFO.lon*100000));
                    APP_DEBUG("[%d]GPS get gnss data, lat:%s, lon:%s", __LINE__, dmTod(nmeaINFO.lat), dmTod(nmeaINFO.lon));
                    sprintf(pApp->gnssData.lat,"%s", dmTod(nmeaINFO.lat));
                    sprintf(pApp->gnssData.lon,"%s", dmTod(nmeaINFO.lon));
                    APP_DEBUG("[%d]GPS get gnss data, altitude:%d", __LINE__, (int)(nmeaINFO.elv * 10000));
                    sprintf(pApp->gnssData.elv,"%s", app_gps_evl2str(nmeaINFO.elv));
                    pApp->gnssData.view = nmeaINFO.satinfo.inview;
                    APP_DEBUG("[%d]GPS get gnss data, speed:%d", __LINE__, (int)(nmeaINFO.speed * 1000000));
                    sprintf(pApp->gnssData.speed,"%s", app_gps_speed2str(nmeaINFO.speed));
                    sprintf(pApp->gnssData.mode,"%s", "GPS");
                    // pApp->gnssData.lat = nmeaINFO.lat;
                    // pApp->gnssData.lon = nmeaINFO.lon;
                    // pApp->gnssData.elv = nmeaINFO.elv;
                    // pApp->gnssData.view = nmeaINFO.satinfo.inview;
                    // pApp->gnssData.speed = nmeaINFO.speed;

                    //UTC: "2023-01-05 03:22:22"
                    memset(&m_time, 0, sizeof(m_time));
                    MG_TIME_GetLocalTime(&m_time);
                    memset((char *)pApp->gnssData.utc, 0, sizeof(pApp->gnssData.utc));
                    // sprintf((char *)pApp->gnssData.utc, "%04d-%02d-%02d %02d:%02d:%02d%02d",
                    //         m_time.year, m_time.month, m_time.day,
                    //         m_time.hour, m_time.minute, m_time.second, m_time.timezone);
                    sprintf((char *)pApp->gnssData.utc, "%04d-%02d-%02d %02d:%02d:%02d",
                        m_time.year, m_time.month, m_time.day,
                        m_time.hour, m_time.minute, m_time.second);
                            

                    iRet = MG_RET_OK;
                    break;
                }
            }
        }
        app_util_threadSleep(1000, pApp->bLowPowerModeEn);
    }while(--i > 0);

    if (MG_GNSS_GetStatus() == true){
        if (!MG_GNSS_Close()){
            APP_DEBUG("[%d]Filed to cloess the GNSS system.", __LINE__);
        } else {
            APP_DEBUG("[%d]Successfully cloessed the GNSS system.", __LINE__);
        }
    }
    //关GPS天线
    app_gps_closeGpsANT();

    return iRet;
}

static s16 _app_lbs_searchStr(char *pBuf, char *pStr)
{
    s16 iBufLen;
    s16 iStrLen;
    s16 iBufPos;
    s16 iStrPos;
    s16 iMatchCnt;

    iBufLen = iStrLen = 0;
    while (pBuf[iBufLen] != '\0')
    {
        iBufLen++;
    }
    while (pStr[iStrLen] != '\0')
    {
        iStrLen++;
    }

    if (iBufLen < iStrLen)
        return -1;

    iMatchCnt = iBufPos = iStrPos = 0;
    while (iBufPos < iBufLen)
    {
        if (pBuf[iBufPos] == pStr[iStrPos])
        {
            iMatchCnt++;
            if (iMatchCnt == iStrLen)
            {
                return (iBufPos - iStrLen + 1);
            }
            iBufPos++;
            iStrPos++;
        }
        else
        {
            iBufPos++;
            iStrPos = 0;
            iMatchCnt = 0;
        }
    }
    return -1;
}

static bool _app_lbs_getPos(char *pData, char *latitude, char *longtitude)
{
    char *content = pData;
    char *lat;
    char *lng;
    s16 pos_content = 0;
    char lat_str[LBS_LONGTITUDE_LATITUDE_SIZE] = {0};
    char lng_str[LBS_LONGTITUDE_LATITUDE_SIZE] = {0};
    u32 size;

    if((NULL == pData) || (NULL == latitude) || (NULL == longtitude))
    {
        return false;
    }

    pos_content = _app_lbs_searchStr(content, "location");
    if (pos_content < 0)
    {
        return false;
    }

    content = content + pos_content + 11;

    lng = content;
    size = 0;
    while (*content++ != ',')
    {
        size++;
    }
    memcpy(lng_str, lng, size);
    //Get the latitude
    lat = content;
    size = 0;
    while (*content++ != '\"')
    {
        size++;
    }
    memcpy(lat_str, lat, size);

    strncpy(latitude, lat_str, LBS_LONGTITUDE_LATITUDE_SIZE);
    strncpy(longtitude, lng_str, LBS_LONGTITUDE_LATITUDE_SIZE);

    return true;
}

static double _app_lbs_degree2ndeg(double val)
{
    double int_part;
    double fra_part;
    fra_part = modf(val, &int_part);
    val = int_part * 100 + fra_part * 60;
    return val;
}

static int _app_lbs_httpGet(app_cfg_t *pApp, const char *url, u32 timeout_s)
{
    int iRet = MG_RET_OK;
    ST_HttpInfo httpinfo = {0};
    ST_HttpRequestCtx *request = NULL;
    ST_HttpResponseInfo *response = NULL;
    char latitude[LBS_LONGTITUDE_LATITUDE_SIZE] = {0};
    char longtitude[LBS_LONGTITUDE_LATITUDE_SIZE] = {0};

    httpinfo.cid = pApp->netInfo.cid;
    if (strstr((char*)url, "https://"))
    {
        httpinfo.vsn = HTTP_TLS_VERSION_TLS1_2;
        httpinfo.verify = HTTP_TLS_VERIFY_NONE;
        httpinfo.cacert = NULL;
    }

    request = MG_HTTP_Create(&httpinfo);
    if (NULL == request)
        iRet = MG_RET_ERR;

    // new response context
    if (MG_RET_OK == iRet)
    {
        response = MG_HTTP_ResponseNew();
        if (NULL == response)
            iRet = MG_RET_ERR;
    }

    // request get
    if (MG_RET_OK == iRet)
        iRet = MG_HTTP_Get(request, url, response, timeout_s*1000);

    // get response content size
    if (MG_RET_OK == iRet)
    {
        if (MG_HTTP_GetResponseInfoSize(response, HTTP_RESPONSE_CONTENT) <= 0)
            iRet = MG_RET_ERR;
    }

    // get response content value
    if (MG_RET_OK == iRet)
    {
        memset(g_app_tmpBuff, 0, sizeof(g_app_tmpBuff));

        MG_HTTP_GetResponseInfoValue(response, HTTP_RESPONSE_CONTENT, (u8 *)g_app_tmpBuff, sizeof(g_app_tmpBuff));
        if (true == _app_lbs_getPos(g_app_tmpBuff, latitude, longtitude))
        {
            ST_Time m_time;
            // char *ptr = NULL;
            char evl_null[] = "0.0";
            char speed_null[] = "0.0";

            APP_DEBUG("[%d]LBS get gnss data, latitude:%s, longtitude:%s", __LINE__, latitude, longtitude);
            memset(&pApp->gnssData, 0, sizeof(pApp->gnssData));

            // pApp->gnssData.lat = _app_lbs_degree2ndeg(strtod(latitude, &ptr));
            // pApp->gnssData.lon = _app_lbs_degree2ndeg(strtod(longtitude, &ptr));
            sprintf((char *)pApp->gnssData.lat, "%s",latitude);
            sprintf((char *)pApp->gnssData.lon, "%s",longtitude);
            // pApp->gnssData.lat = latitude;
            // pApp->gnssData.lon = longtitude;
            APP_DEBUG("[%d]LBS modify to string, lat:%s, lon:%s", __LINE__, pApp->gnssData.lat,pApp->gnssData.lon);
            sprintf(pApp->gnssData.elv,"%s", evl_null);
            // pApp->gnssData.elv = 0;
            pApp->gnssData.view = 0;
            sprintf(pApp->gnssData.speed,"%s", speed_null);
            // pApp->gnssData.speed = 0;
            sprintf(pApp->gnssData.mode,"%s", "LBS");
            //UTC: "2023-01-05 03:22:22"
            memset(&m_time, 0, sizeof(m_time));
            MG_TIME_GetLocalTime(&m_time);
            memset((char *)pApp->gnssData.utc, 0, sizeof(pApp->gnssData.utc));

            if ((m_time.timezone) >= 0)
            {
                // sprintf((char *)pApp->gnssData.utc, "%04d-%02d-%02d %02d:%02d:%02d+%02d",
                //     m_time.year, m_time.month, m_time.day,
                //     m_time.hour, m_time.minute, m_time.second, m_time.timezone);
                sprintf((char *)pApp->gnssData.utc, "%04d-%02d-%02d %02d:%02d:%02d",
                    m_time.year, m_time.month, m_time.day,
                    m_time.hour, m_time.minute, m_time.second);
            }
            else
            {
                sprintf((char *)pApp->gnssData.utc, "%04d-%02d-%02d %02d:%02d:%02d-%02d",
                    m_time.year, m_time.month, m_time.day,
                    m_time.hour, m_time.minute, m_time.second, m_time.timezone);
            }
            iRet = MG_RET_OK;
        }
    }

    // close request
    MG_HTTP_Close(request);
    // delete response context
    MG_HTTP_ResponseDelete(response);
    // destoty request context
    MG_HTTP_Destroy(request);

    return iRet;
}

#define LBS_DEFAULT_KEY "f61857c3cf1c7e892776a58383799c9f"

int app_gps_getLBSData(int iTimeout, app_cfg_t *pApp)
{
    int iRet = MG_RET_OK;
    char url[256] = {0};

    ST_NwUeExtStatusInfo ueExtStatusInfo = {0};
    iRet = MG_NW_GetUeStatusInfo(pApp->netInfo.simId, &ueExtStatusInfo);
    if (iRet == MG_RET_OK)
    {
        snprintf(url, sizeof(url), "http://apilocate.amap.com/position?accesstype=0"
                 "&bts=%x,%x,%d,%d,%d"
                 "&key=%s",
                 ueExtStatusInfo.rrcStatus_plmn_mcc,
                 (ueExtStatusInfo.rrcStatus_plmn_mncWithAddInfo) & 0xFFF,
                 ueExtStatusInfo.rrcStatus_tac,
                 ueExtStatusInfo.rrcStatus_cellId,
                 ueExtStatusInfo.phyStatus_sRsrp / 100,
                 LBS_DEFAULT_KEY);

        APP_DEBUG("[%d]LBS URL: %s", __LINE__, url);
        _app_lbs_httpGet(pApp, url, 30);
    }

    return iRet;
}

int app_gps_getData(app_cfg_t *pApp)
{
    int iRet = MG_RET_OK;

    //获取GPS定位数据
    iRet = app_gps_getGnssData(45*1000, pApp);

    //如果获取不到定位数据,获取LBS
    if (iRet != MG_RET_OK)
    {
        iRet = app_gps_getLBSData(60*1000, pApp);
    }

    return iRet;
}

void app_gps_init(void)
{
    bool bRet = MG_GPIO_Init(GPS_ANT_PIN, MG_GPIO_DIR_OUT, MG_GPIO_LEVEL_LOW, MG_GPIO_PULL_UP);
    APP_DEBUG("[%d]GPS init gps ant, %d", __LINE__, bRet);
}

