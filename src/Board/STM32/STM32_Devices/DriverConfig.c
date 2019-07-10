

#include "DriverConfig.h" 



void DriverInt(void)
{
#if CONFIG_DEV_RS232
	Bsp_RS232_Initialise();
#endif 


}


