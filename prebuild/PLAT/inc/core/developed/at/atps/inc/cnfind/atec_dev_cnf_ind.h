/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: atec_dev_cnf_ind.h
*
*  Description: Process dev related AT commands
*
*  History:
*
*  Notes:
*
******************************************************************************/
#ifndef __ATEC_DEV_CNF_IND_H__
#define __ATEC_DEV_CNF_IND_H__

#include <stdio.h>
#include "cmsis_os2.h"
#include "at_util.h"
#include "cmidev.h"
#include "atec_dev.h"
#include "ps_dev_if.h"


void atDevProcCmiCnf(CamCmiCnf *pCmiCnf);
void atDevProcCmiInd(CamCmiInd *pCmiInd);


#endif
