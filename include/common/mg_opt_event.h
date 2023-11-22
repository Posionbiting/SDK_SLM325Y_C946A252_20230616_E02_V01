/**
  ******************************************************************************
  * @file    mg_opt_event.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all event id.
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

#ifndef __MG_OPT_EVENT_H__
#define __MG_OPT_EVENT_H__

/**
 * NET event
 */
#define EV_OPEN_NET_BASE 0x10000000

#define EV_OPEN_SIM_CARD_INSERT         (EV_OPEN_NET_BASE + 1)  //Insert the sim card
#define EV_OPEN_SIM_CARD_PULL_OUT       (EV_OPEN_NET_BASE + 2)  //Pull out the sim card
#define EV_OPEN_CS_DOMAIN_REGISTER      (EV_OPEN_NET_BASE + 3)  //CS Domain regist
#define EV_OPEN_CS_DOMAIN_NO_REGISTER   (EV_OPEN_NET_BASE + 4)  //CS Domain unregist
#define EV_OPEN_PS_DOMAIN_REGISTER      (EV_OPEN_NET_BASE + 5)  //PS Domain regist
#define EV_OPEN_PS_DOMAIN_NO_REGISTER   (EV_OPEN_NET_BASE + 6)  //PS Domain unregist
#define EV_OPEN_PDP_AUTO_ACTIVATION     (EV_OPEN_NET_BASE + 7)  //PDP auto active
#define EV_OPEN_PDP_AUTO_DEACTIVATION   (EV_OPEN_NET_BASE + 8)  //PDP auto deactive
#define EV_OPEN_PDP_MANUAL_ACTIVATION   (EV_OPEN_NET_BASE + 9)  //PDP manual acitve
#define EV_OPEN_PDP_MANUAL_DEACTIVATION (EV_OPEN_NET_BASE + 10) //PDP manual deactive
#define EV_OPEN_NET_END 0x100000FF

/**
 * SMS event
 */
#define EV_OPEN_SMS_BASE 0x10000100

#define EV_OPEN_SMS_NEW (EV_OPEN_SMS_BASE + 1)                     //receive new SMS

#define EV_OPEN_SMS_END 0x100001FF

/**
 * User Event: 0x20000000 -- 0xA0000000
 */
#define EV_OPEN_EV_BASE_ 0x20000000
#define EV_OPEN_EV_END_ 0xA0000000


#endif  // End-of __MG_OPT_EVENT_H__
