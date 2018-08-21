
#include "usb_conf.h"
#include "usb_devices.h"

__IO uint16_t wIstr;  /* ISTR register last read value */


void USB_McuUSBTask(void *pd)
{

#ifdef CONFIG_USBMCUFLASH
   USB_McuFlashTask();
#endif
#ifdef CONFIG_USBMOUSE
     USB_MouseTask();
#endif
#ifdef CONFIG_USBVCOM
    USB_VcomTask();
#endif
}

void USB_Istr(void)
{
	#ifdef CONFIG_USBMCUFLASH
		Mass_USB_Istr();
	#endif
	#ifdef CONFIG_USBMOUSE
		Mouse_USB_Istr();
	#endif
    #ifdef CONFIG_USBVCOM
		Vcom_USB_Istr();
	#endif
}

void USB_CTR_HP(void)
{
	#ifdef CONFIG_USBMCUFLASH
		CTR_HP();
	#endif
}
