
#include "UserappConfig.h"
#include "usb_lib.h"
#include "mass_hw_config.h"
#include "mass_usb_pwr.h"
#include "mass_usb_Istr.h"

#include "mouse_usb_Istr.h"
__IO uint16_t wIstr;  /* ISTR register last read value */

void USB_Istr(void)
{
	#ifdef CONFIG_USBMCUFLASH
		Mass_USB_Istr();
	#endif
	#ifdef CONFIG_USBMOUSE
		Mouse_USB_Istr();
	#endif
}

void USB_CTR_HP(void)
{
	#ifdef CONFIG_USBMCUFLASH
		CTR_HP();
	#endif
}
