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
#ifndef _MEIG_DEBUG_H
#define _MEIG_DEBUG_H

typedef enum _ftp_log_type_t
{
    FTPLOG_CON = 0,
    FTPLOG_LIB,
    FTPLOG_AT,
} ftp_log_type_t;

void FTPLOGI(ftp_log_type_t t, const char *format, ...);
void FSLOGI(const char *format, ...);

#endif


