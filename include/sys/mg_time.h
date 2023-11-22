/**
  ******************************************************************************
  * @file    mg_time.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the time function APIs.
  ******************************************************************************
  *  @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2022 MEIG SMART</center></h2>
  ******************************************************************************
  */


#ifndef _MG_TIME_H_
#define _MG_TIME_H_
#include "mg_type.h"

typedef struct {
    s32 year;
    s32 month;
    s32 day;
    s32 hour;
    s32 minute;
    s32 second;
    s32 timezone;  //one digit expresses a quarter of an hour, range:-96~96
}ST_Time;

/*****************************************************************
* Function:     MG_TIME_Mktime
*
* Description:
*               This function gets total seconds elapsed in UTC time
*               since 1970.01.01 00:00:00.
*
* Parameters:
*               time:
*                       [in] Point to the ST_Time object
* Return:
*               >=0, the total seconds.
*               <0, error code.
* Note:
*       ST_Time time = {
*            .year = 2015,
*            .month = 10,
*            .day = 3,
*            .hour = 8,
*            .minute = 10,
*            .second = 23,
*            .timezone = 32;
*            }
*  The meaning of time definition is 2015-10-03T08:10:23+32Z
*  2015-10-03T08:10:23,  indicate the date is GMT time.
*  +32, indicate time zone in quarters of an hour, It's actually
*  the eighth East District.
*****************************************************************/
s64 MG_TIME_Mktime(ST_Time *time);

/*****************************************************************
* Function:     MG_TIME_Gmtime
*
* Description:
*               This function convert the seconds elapsed in UTC time
*               since 1970.01.01 00:00:00 to GMT time.
*
* Parameters:
*               seconds:
*                       [in] the seconds elapsed in UTC time since
*                            1970.01.01 00:00:00
*               time:
*                       [out] Point to the ST_Time object
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
* Note:
*       ST_Time time = {
*            .year = 2015,
*            .month = 10,
*            .day = 3,
*            .hour = 8,
*            .minute = 10,
*            .second = 23,
*            .timezone = 32;
*            }
*  The meaning of time definition is 2015-10-03T08:10:23+32Z
*  2015-10-03T08:10:23,  indicate the date is GMT time.
*  +32, indicate time zone in quarters of an hour, It's actually
*  the eighth East District.
*****************************************************************/
s32 MG_TIME_Gmtime(s64 seconds, ST_Time *time);

/******************************************************************************
* Function:     MG_TIME_SetLocalTime
*
* Description:
*               Set the current local time.
*
* Parameters:
*               time:
*                       [in] Point to the ST_Time object
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
* Note:
*       ST_Time time = {
*            .year = 2015,
*            .month = 10,
*            .day = 3,
*            .hour = 8,
*            .minute = 10,
*            .second = 23,
*            .timezone = 32;
*            }
*  The meaning of time definition is 2015-10-03T08:10:23+32Z
*  2015-10-03T08:10:23,  indicate the date is GMT time.
*  +32, indicate time zone in quarters of an hour, It's actually
*  the eighth East District.
******************************************************************************/
s32 MG_TIME_SetLocalTime(ST_Time *time);

/******************************************************************************
* Function:     MG_TIME_GetLocalTime
*
* Description:
*               Get the current local time.
*
* Parameters:
*               time:
*                       [out] Point to the ST_Time object
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
* Note:
*       ST_Time time = {
*            .year = 2015,
*            .month = 10,
*            .day = 3,
*            .hour = 8,
*            .minute = 10,
*            .second = 23,
*            .timezone = 32;
*            }
*  The meaning of time definition is 2015-10-03T08:10:23+32Z
*  2015-10-03T08:10:23,  indicate the date is GMT time.
*  +32, indicate time zone in quarters of an hour, It's actually
*  the eighth East District.
******************************************************************************/
s32 MG_TIME_GetLocalTime(ST_Time *time);

/******************************************************************************
* Function:     MG_TIME_SetTimeZone
*
* Description:
*               Set the current timezone.
*
* Parameters:
*               timezone:
*                       [in] timezone in quarters of an hour,
*                            range:-96 ~ 96.
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_TIME_SetTimeZone(s32 timezone);

/******************************************************************************
* Function:     MG_TIME_GetTimeZone
*
* Description:
*               Get the current timezone.
*
* Parameters:
*               timezone:
*                       [out] Point to the timezone
* Return:
*               MG_RET_OK, this function succeeds.
*               Other values, error code.
******************************************************************************/
s32 MG_TIME_GetTimeZone(s32 *timezone);

#endif // End-of _MG_TIME_H_

