/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MASS_HW_CONFIG_H
#define __MASS_HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <includes.h>	
#include "stm32f10x.h"
#include "platform_config.h"
#include "usb_lib.h"
#include "usb_core.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "usb_type.h"
#include "usb_init.h"

#include "mass_usb_conf.h"
#include "mass_mal.h"
#include "mass_usb_desc.h"
#include "mass_usb_pwr.h"
#include "mass_memory.h"
#include "mass_usb_scsi.h"
#include "mass_usb_bot.h"
#include "mass_usb_istr.h"
#include "mass_nand_if.h"
#include "mass_usb_prop.h"

#include "usb_conf.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define BULK_MAX_PACKET_SIZE  0x00000040

/* Exported functions ------------------------------------------------------- */
void Mass_Set_System(void);
void Mass_Set_USBClock(void);
void Mass_Enter_LowPowerMode(void);
void Mass_Leave_LowPowerMode(void);
void Mass_USB_Interrupts_Config(void);
void Led_Config(void);
void Led_RW_ON(void);
void Led_RW_OFF(void);
void USB_Configured_LED(void);
void USB_NotConfigured_LED(void);
void Mass_USB_Cable_Config (FunctionalState NewState);
void Mass_Get_SerialNum(void);
void MAL_Config(void);
void Mass_USB_Disconnect_Config(void);
/* External variables --------------------------------------------------------*/

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
