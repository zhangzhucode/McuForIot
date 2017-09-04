/****************************************************************************
创建时间：	20120712
创建人：	张柱
版本：		stm32 v3.5
****************************************************************************/
#include <includes.h>

#include "UserappConfig.h"
#include "DriverConfig.h" 
#include "DebugTask.h"
#include "MPU6050Task.h"

/*********************PRI*****************************/
#define DebugTask_PRI		 	17 
#define LCD5110Task_PRI		 	18 
#define MPU6050Task_PRI		 	18 	
/*******************TASK_STK**************************/
#define APP_TASK_STK_SIZE       256
static  INT32U STK_DebugTask[APP_TASK_STK_SIZE]; 
static  INT32U STK_LCD5110Task[APP_TASK_STK_SIZE];
#if CONFIG_MPU6050
	static  INT32U STK_MPU6050Task[APP_TASK_STK_SIZE];
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

	OSTaskCreate(DebugTask, (void *)0,(OS_STK *)&STK_DebugTask[APP_TASK_STK_SIZE-1],DebugTask_PRI); //17
	//OSTaskCreate(LCD5110Task, (void *)0,(OS_STK *)&STK_LCD5110Task[APP_TASK_STK_SIZE-1],LCD5110Task_PRI); //17
	
	#if CONFIG_MPU6050
	OSTaskCreate(MPU6050Task, (void *)0,(OS_STK *)&STK_MPU6050Task[APP_TASK_STK_SIZE-1],MPU6050Task_PRI); 
	#endif 

}



