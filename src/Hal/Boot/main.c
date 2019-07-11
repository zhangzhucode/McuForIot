/****************************************************************************
创建时间：20120712
创建人：张柱
版本：stm32 v3.5
****************************************************************************/

#include <includes.h>
#include "UserappConfig.h"
#include "usb_devices.h"

#define MAIN_TASK_PRI      		15
#define TASK_STK_SIZE       	256
static  INT32U STK_MAIN_Manage[TASK_STK_SIZE];

void	MainTask(void *pd);

int main(void)
{
  	Board_Configuration();
	
	  OSInit();
    OSTaskCreate(MainTask,    (void *)0,(OS_STK *)&STK_MAIN_Manage[TASK_STK_SIZE-1],  MAIN_TASK_PRI); //15
    OSStart();
}
 

/***************************************************************************************
** 函数名称: MainTask
** 功能描述: 主任务
** 参    数: None
** 返 回 值: None 
** 日  　期: 
****************************************************************************************/
void MainTask(void *pd)
{	
		pd=pd;
    
	AppCreate();

	while(1)
	{  
    
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
    
		OSTimeDly(OS_TICKS_PER_SEC);

		GPIO_ResetBits(GPIOB, GPIO_Pin_12);

	  OSTimeDly(OS_TICKS_PER_SEC);  
	}	
}



