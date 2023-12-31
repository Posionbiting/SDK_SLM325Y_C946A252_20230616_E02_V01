/*******************************************************************************
 *
 * Copyright (c) 2017 China Mobile and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bai Jie & Long Rong, China Mobile - initial API and implementation
 *    Baijie & Longrong, China Mobile - Please refer to git log
 *
 *******************************************************************************/

/************************************************************************/
/* nb-iot middle software of china mobile api                           */
/************************************************************************/

#include "cis_def.h"
#include "cis_config.h"
#include "FreeRTOS.h"
#include "time.h"

#ifndef _CIS_INTERFACE_SYS_H_
#define _CIS_INTERFACE_SYS_H_

#define NBSYS_IMEI_MAXLENGTH    (16)
#define NBSYS_IMSI_MAXLENGTH    (16)
#if CIS_ENABLE_PSK
#define NBSYS_PSK_MAXLENGTH     (17)
#endif

#if CIS_ENABLE_CMIOT_OTA

#define NBSYS_EID_MAXLENGTH                      (22)
#define NBSYS_DEVICENAME_MAXLENGTH               (16)
#define NBSYS_TOOLVERSION_MAXLENGTH              (16)

#define OTA_FINISH_COMMAND_SUCCESS_CODE         ('0')
#define OTA_FINISH_COMMAND_UNREGISTER_CODE      ('3')
#define OTA_FINISH_COMMAND_FAIL_CODE            ('2')

#define OTA_FINISH_AT_RETURN_SUCCESS              (0)
#define OTA_FINISH_AT_RETURN_REPEATED             (1)
#define OTA_FINISH_AT_RETURN_NO_REQUEST           (30)
#define OTA_FINISH_AT_RETURN_TIMEOUT_FIRST        (55)
#define OTA_FINISH_AT_RETURN_TIMEOUT_SECOND       (56)
#define OTA_FINISH_AT_RETURN_FAIL                 (57)

typedef enum{
	OTA_HISTORY_STATE_RAW,
	OTA_HISTORY_STATE_FINISHED,
}cis_ota_history_state;

extern uint8_t    g_cmiot_ota_at_return_code;//the return result defination of comand AT+OTASTART

#endif
#define cissys_assert( x )  configASSERT( x )

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	FOTA_STATE_IDIL,
	FOTA_STATE_DOWNLOADING,
	FOTA_STATE_DOWNLOADED,
	FOTA_STATE_UPDATING,
}cis_fw_state;

typedef enum
{
	FOTA_RESULT_INIT,                //0 init
	FOTA_RESULT_SUCCESS,             //1 success
	FOTA_RESULT_NOFREE,              //2 no space
	FOTA_RESULT_OVERFLOW,            //3 downloading overflow
	FOTA_RESULT_DISCONNECT,          //4 downloading disconnect
	FOTA_RESULT_CHECKFAIL,           //5 validate fail
	FOTA_RESULT_NOSUPPORT,           //6 unsupport package
	FOTA_RESULT_URIINVALID,          //7 invalid uri
	FOTA_RESULT_UPDATEFAIL,          //8 update fail
	FOTA_RESULT_PROTOCOLFAIL        //9 unsupport protocol
}cissys_fw_result_type_t;

typedef enum
{
	cissys_event_unknow = 0,
	cissys_event_fw_erase_success,         //for erase, handle /5/0/26501
	cissys_event_fw_erase_fail,  
	cissys_event_write_success,            //for write
	cissys_event_write_fail,
	cissys_event_fw_validate_success,       //for validate
	cissys_event_fw_validate_fail,                 
	cissys_event_fw_update_success,   //for update
	cissys_event_fw_update_fail,
}cissys_event_t;

#if CIS_ENABLE_CMIOT_OTA
typedef enum{
	CMIOT_OTA_STATE_IDIL,
	CMIOT_OTA_STATE_START,
	CMIOT_OTA_STATE_SUCCESS,
	CMIOT_OTA_STATE_FAIL,
	CMIOT_OTA_STATE_FINISH,	
}cis_cmiot_ota_state;

#endif
typedef cis_ret_t (*cissys_event_callback_t)(cissys_event_t id,void* param,void* userData,int *len);

typedef struct st_cissys_callback
{
	void* userData;
	cissys_event_callback_t onEvent;
}cissys_callback_t;

//extern cis_cfg_sys_t g_sysconfig;
//extern cissys_callback_t g_syscallback;

typedef struct cis_fw_context{
	uint8_t state;  // /5/0/3 
	uint8_t result;  // /5/0/5 
	int savebytes;
}cis_fw_context_t;


#define CIS_MAX_DL_BUFFER_NUM	8
#define CIS_MAX_DL_BUFFER_LEN	2048

typedef enum
{
	CIS_DL_BUFFER_INVALID = 0,
	CIS_DL_BUFFER_WRITE,
	CIS_DL_BUFFER_EXCUTE,
}cisDLBufType;


typedef struct cisDLBufferInfo_Tag
{
	void *context;
	cis_mid_t mid;
	uint8_t uri_flag;           // indicates which segments are set
	cis_oid_t uri_objectId;
	cis_iid_t uri_instanceId;
	cis_rid_t uri_resourceId;
	uint32_t len;
	void *data;
	cis_rescount_t resCount;
	bool valid;
	cisDLBufType type;
}cisDLBufferInfo;



cis_ret_t cissys_init(void *context,const cis_cfg_sys_t* cfg,cissys_callback_t* event_cb);
cis_ret_t cissys_deinit(void *context);

//negative return value for error;return millisecond;
uint32_t    cissys_gettime(void);
void        cissys_sleepms(uint32_t ms);

UINT32		cissys_tick(void);

void        cissys_logwrite(uint8_t* buffer,uint32_t length);
void*       cissys_malloc(size_t length);
void        cissys_free(void* ptr);

void*       cissys_memset(void* s, int c, size_t n);
void*       cissys_memcpy(void* dst, const void* src, size_t n);
void*       cissys_memmove(void* dst, const void* src, size_t n);
int         cissys_memcmp(const void* s1, const void* s2, size_t n);
void        cissys_fault(uint16_t id);
//void        cissys_assert(bool flag);

uint32_t    cissys_rand(void);
uint32_t cissys_getCoapMinTransmitWaitTime(void);
void cissys_setCoapMinTransmitWaitTime(void);
uint32_t cissys_getCoapMaxTransmitWaitTime(void);
void cissys_setCoapMaxTransmitWaitTime(void);
uint8_t cissys_getCoapAckTimeout(void);
void cissys_setCoapAckTimeout(uint8_t celevel);
void cissys_setCoapAckTimeoutDirect(uint8_t timeout);






/*
#if CIS_ENABLE_PSK
uint8_t		cissys_getPSK(char* buffer, uint32_t maxlen);
#endif
*/
uint8_t     cissys_getIMEI(char* buffer,uint32_t maxlen);
uint8_t     cissys_getIMSI(char* buffer,uint32_t maxlen, bool usingReal);
#if CIS_ENABLE_CMIOT_OTA
uint8_t    cissys_getEID(char* buffer,uint32_t maxlen);
#endif
bool setIMEI(char* imei, int len);//EC add
bool setIMSI(char* imsi, int len);//EC add

void        cissys_lockcreate(void** mutex);
void        cissys_lockdestory(void* mutex);
cis_ret_t   cissys_lock(void* mutex,uint32_t ms);
void        cissys_unlock(void* mutex);

bool        cissys_save(uint8_t* buffer,uint32_t length);
bool        cissys_load(uint8_t* buffer,uint32_t length);

uint32_t    cissys_getFwVersion(uint8_t** version);

uint32_t    cissys_getCellId(void);
uint32_t    cissys_getRadioSignalStrength(void);

#if CIS_ENABLE_UPDATE

//update
uint32_t    cissys_getFwBatteryLevel(void);
uint32_t    cissys_getFwBatteryVoltage(void);
uint32_t    cissys_getFwAvailableMemory(void);

bool        cissys_checkFwValidation(cissys_callback_t *cb);
bool        cissys_eraseFwFlash (cissys_callback_t *cb);
void        cissys_ClearFwBytes(void);
uint32_t    cissys_writeFwBytes(uint32_t size,uint8_t* buffer,cissys_callback_t *cb);
void        cissys_savewritebypes(uint32_t size);
bool        cissys_updateFirmware(cissys_callback_t *cb);

bool		cissys_readContext(cis_fw_context_t * context);
//int         cissys_setFwSavedBytes(uint32_t length);
int         cissys_getFwSavedBytes(void);// -1 error  
bool        cissys_setFwState(uint8_t state);
bool        cissys_setFwUpdateResult(uint8_t result);
bool        cissys_checkLocalUpdateResult(void);
void        cis_SetCFUN(uint8_t fun);

#if CIS_ENABLE_UPDATE_MCU
bool        cissys_setSwState(bool ismcu);
bool        cissys_getSwState(void);

void  cissys_setSotaMemory(uint32_t size);


uint32_t  cissys_getSotaMemory(void);


uint32_t    cissys_getSotaVersion(uint8_t** version);


void  cissys_setSotaVersion(char* version);


#endif

void cis_reboot_timer(void);
void cis_loadVersion(void);
#endif

#if CIS_ENABLE_MONITER
int8_t      cissys_getRSRP(void);
int8_t      cissys_getSINR(void);
int8_t     cissys_getBearer(void);
int8_t     cissys_getUtilization(void);
int8_t     cissys_getCSQ(void);
int8_t     cissys_getECL(void);
int8_t     cissys_getSNR(void);
int16_t    cissys_getPCI(void);
int32_t    cissys_getECGI(void);
int32_t    cissys_getEARFCN(void);
int16_t    cissys_getPCIFrist(void);
int16_t    cissys_getPCISecond(void);
int8_t     cissys_getRSRPFrist(void);
int8_t     cissys_getRSRPSecond(void);
int8_t     cissys_getPLMN(void);
int8_t     cissys_getLatitude(void);
int8_t     cissys_getLongitude(void);
int8_t     cissys_getAltitude(void);
#endif//CIS_ENABLE_MONITER

#ifdef __cplusplus
};
#endif

#endif//_CIS_INTERFACE_SYS_H_
