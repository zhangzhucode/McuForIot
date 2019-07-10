#include <includes.h>	
#include "DriverConfig.h"
//#include "MPU6050.h"

void MPU6050Task(void *pd)
{	
	pd=pd;

	i2cInit();
	MPU6050_INIT();
	while(1)
	{  
	   MPU6050_READ();
	   MPU6050_Dataanl();

	   OSTimeDly(OS_TICKS_PER_SEC);
	}	
}

