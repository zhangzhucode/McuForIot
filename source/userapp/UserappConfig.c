/****************************************************************************
创建时间：	20120712
创建人：	张柱
版本：		stm32 v3.5
****************************************************************************/
#include <includes.h>

#include "UserappConfig.h"

/*******************TASK_STK**************************/
#define APP_TASK_STK_SIZE       256

#if CONFIG_DEBUG
#define DebugTask_PRI		 	17 
static  INT32U STK_DebugTask[APP_TASK_STK_SIZE];
#endif

#if CONFIG_LEDS
#define LedsTask_PRI		 	18 
static  INT32U STK_LedsTask[APP_TASK_STK_SIZE];
#endif

#if CONFIG_LCD5110
#define LCD5110Task_PRI		 	18 
static  INT32U STK_LCD5110Task[APP_TASK_STK_SIZE];
#endif

#if CONFIG_MPU6050
#define MPU6050Task_PRI		 	18 	
static  INT32U STK_MPU6050Task[APP_TASK_STK_SIZE];
#endif

#if CONFIG_USBMCUFLASH
#define USB_McuFlashTask_PRI		 	19 	
static  INT32U STK_USB_McuFlashTask[APP_TASK_STK_SIZE];
#endif
/*****************************************************/

/***************************************************************************************
** 函数名称: AppCreate
** 功能描述:
** 参    数: None
** 返 回 值: None 
** 日  　期: 														
****************************************************************************************/
void AppCreate(void)
{	
	printf("******************************************************************************\r\n");
	printf("******************************************************************************\r\n");
	printf("**      Standard stm32/ucos2 Project	                                    **\r\n");	   
	printf("**      Author  :       zzhere                                              **\r\n");
	printf("**      Version :       Potatoes 1.0.0                                      **\r\n");
	printf("**      Time    :       2014.06.29                                          **\r\n");
	printf("******************************************************************************\r\n");
	printf("/*****************************************************************************\r\n");

#if CONFIG_DEBUG
	OSTaskCreate(DebugTask, (void *)0,(OS_STK *)&STK_DebugTask[APP_TASK_STK_SIZE-1],DebugTask_PRI); 
#endif

#if CONFIG_LEDS
	OSTaskCreate(LedsTask, (void *)0,(OS_STK *)&STK_LedsTask[APP_TASK_STK_SIZE-1],LedsTask_PRI); 
#endif
    
#if CONFIG_LCD5110
	OSTaskCreate(LCD5110Task, (void *)0,(OS_STK *)&STK_LCD5110Task[APP_TASK_STK_SIZE-1],LCD5110Task_PRI); 
#endif	
	
#if CONFIG_MPU6050
	OSTaskCreate(MPU6050Task, (void *)0,(OS_STK *)&STK_MPU6050Task[APP_TASK_STK_SIZE-1],MPU6050Task_PRI);
#endif 


#if CONFIG_USBMCUFLASH
OSTaskCreate(USB_McuFlashTask, (void *)0,(OS_STK *)&STK_USB_McuFlashTask[APP_TASK_STK_SIZE-1],USB_McuFlashTask_PRI);

#endif
}



