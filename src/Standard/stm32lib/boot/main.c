/****************************************************************************
创建时间：20120712
创建人：张柱
版本：stm32 v3.5
****************************************************************************/

#include <includes.h>
#include "UserappConfig.h"
#include "MPU6050.h"
#include "usb_devices.h"
/*********************PRI*****************************/
#define MAIN_TASK_PRI      		15
	
/*******************TASK_STK*****************************/
#define TASK_STK_SIZE       256
static  INT32U STK_MAIN_Manage[TASK_STK_SIZE];
/************************************************/

u32 SystemTime_10_ms;
u32 SystemTime_1000_ms;
void  	MainTask(void *pd);

int main(void)
{

	OSInit();
    OSTaskCreate(MainTask,    (void *)0,(OS_STK *)&STK_MAIN_Manage[TASK_STK_SIZE-1],  MAIN_TASK_PRI); //15
    OSStart();
}
 
void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
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


  	Bsp_Configuration();
	/*********************创建任务*****************************************************/
	AppCreate();

	while(1)
	{  

//		GPIO_SetBits(GPIOC, GPIO_Pin_13);
//		Delay(0xfffff);
//	  Delay(0xfffff);	
//		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//	  Delay(0xfffff);
//	  Delay(0xfffff);			
	  OSTimeDly(OS_TICKS_PER_SEC);
	   
	}	
}



