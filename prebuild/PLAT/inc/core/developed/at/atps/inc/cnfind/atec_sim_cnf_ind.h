/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: atec_sim_cnf_ind.h
*
*  Description: The header file of atec_sim_cnf_ind
*
*  History:
*
*  Notes:
*
******************************************************************************/
#ifndef __ATEC_SIM_CNF_IND_H__
#define __ATEC_SIM_CNF_IND_H__

#include <stdio.h>
#include "cmsis_os2.h"
#include "at_util.h"
//#include "atec_controller.h"
#include "cmisim.h"
#include "atec_sim.h"
#include "ps_sim_if.h"


void atSimProcCmiCnf(CamCmiCnf *pCmiCnf);
void atSimProcCmiInd(CamCmiInd *pCmiInd);

//<!--[ODM][BugID 38202]cxz@2022-11-09 add
void atProcSimHotSwapInd(BOOL bSimPlugIn);
//end -->

//<!--[ODM][TaskID 34278]ldw@2023-03-28 Modify message is blocked before the sim ready reports
void atProcSimState(void *arg);
//end [TaskID 34278]-->


#endif
