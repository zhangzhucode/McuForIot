
#ifndef __APPCREATE_H
#define __APPCREATE_H 

#include <includes.h>
#include "DriverConfig.h" 
#include "5110Task.h"
#include "DebugTask.h"
#include "MPU6050Task.h"
#include "ledstask.h"
#include "usb_mcuflash_task.h"
#include "usb_mouse_task.h"

#define	    CONFIG_DEBUG       1    
#define	    CONFIG_LEDS        1    
#define	    CONFIG_LCD5110     0   
#define	    CONFIG_MPU6050     0   // 1 Ìí¼Ó 0È¡Ïû  
//#define     CONFIG_USBMCUFLASH 1 
#define     CONFIG_USBMOUSE    1 

extern void AppCreate(void);

#endif

