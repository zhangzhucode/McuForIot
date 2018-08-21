#include "mass_hw_config.h"


void USB_McuFlashTask(void )
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


