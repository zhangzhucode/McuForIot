/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_conf.h
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Joystick Mouse demo configuration file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H

#include "UserappConfig.h"
#include <mass_usb_conf.h>


#ifdef CONFIG_USBMCUFLASH
#include <mass_usb_conf.h>
#endif

extern void USB_Istr(void);
extern void USB_CTR_HP(void);

#endif /*__USB_CONF_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

