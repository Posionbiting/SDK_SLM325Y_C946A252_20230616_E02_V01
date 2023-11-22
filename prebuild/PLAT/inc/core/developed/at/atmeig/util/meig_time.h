/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: meig_time.h
*
*  Description:
*
*  History:
*
*  Notes:
*
******************************************************************************/
#ifndef _MEIG_TIME_H
#define _MEIG_TIME_H

typedef long long               INT64;

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int timezone;  //one digit expresses a quarter of an hour, range:-96~96
}ST_Time;


INT64 G_TIME_Mktime(ST_Time *time);

#endif


