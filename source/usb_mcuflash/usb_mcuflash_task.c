#include "usb_mcuflash_task.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

void USB_McuFlashTask(void *pd)
{ 
    Set_System();
    
    Set_USBClock();
    
    USB_Interrupts_Config();
    
    USB_Init();
    
    while (bDeviceState != CONFIGURED);

    USB_Configured_LED();  
    while(1)
	{
        OSTimeDly(100);
	}
}
