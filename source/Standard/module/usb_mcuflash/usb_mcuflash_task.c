#include "usb_mcuflash_task.h"
#include "usb_lib.h"
#include "mass_hw_config.h"
#include "mass_usb_pwr.h"

void USB_McuFlashTask(void *pd)
{ 
    Mass_Set_System();
    
    Mass_Set_USBClock();
    
    Mass_USB_Interrupts_Config();
    
    USB_Init();
    
    while (Mass_bDeviceState != CONFIGURED);

    USB_Configured_LED();  
    while(1)
	{
        OSTimeDly(100);
	}
}
