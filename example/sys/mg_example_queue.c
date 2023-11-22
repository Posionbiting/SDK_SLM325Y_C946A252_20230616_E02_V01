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
#include "mg_queue.h"
#include "mg_trace.h"
#include "mg_utils.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'Q', 'E')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

const char *writebuf = "abcdefghijklmn1234567890";
u8 readbuf[48] = {0};

static void prvThreadEntry(void *param)
{
	s32 iResult;
	MG_OS_ThreadSleep(5000);
	APP_DEBUG("mg_queue example entry line[%d]", __LINE__);

	ST_queue * queue = NULL;

//create queue
	do
	{
		queue = MG_QUEUE_Create(4, 100);
		APP_DEBUG("mg_queue example create line[%d] queue %x", __LINE__,queue);
		MG_OS_ThreadSleep(1000);
	}while(!queue);

//write data to queue
	iResult = MG_QUEUE_Write(queue, (const u8 *)writebuf, MG_UTILS_Strlen(writebuf));
	APP_DEBUG("mg_queue example Write line[%d] iResult %x", __LINE__,iResult);
	MG_OS_ThreadSleep(1000);

//read data from queue
	iResult = MG_QUEUE_Read(queue, readbuf, 48);
	APP_DEBUG("mg_queue example Read line[%d] iResult %x buf %s", __LINE__, iResult, readbuf);
	MG_OS_ThreadSleep(1000);

//delete queue
	MG_QUEUE_Delete(queue);

	APP_DEBUG("mg_queue example exit line[%d]", __LINE__);
	MG_OS_ThreadSleep(1000);
	MG_OS_ThreadExit();
}

void proc_main_task(void *param)
{
	APP_DEBUG("mg example enter line[%d]", __LINE__);
	MG_OS_ThreadCreate("mgtime", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024*2, 4);
}

