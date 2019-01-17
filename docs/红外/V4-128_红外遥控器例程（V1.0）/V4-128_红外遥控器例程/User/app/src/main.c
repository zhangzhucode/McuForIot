/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.2
*	说    明 : 主程序
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2015-08-30 armfly  首发
*	
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"		/* 如果要用ST的固件库，必须包含这个文件 */
#include "math.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"V4-128_红外遥控器例程"
#define EXAMPLE_DATE	"2015-08-30"
#define DEMO_VER		"1.0"

/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	uint8_t ucKeyCode;
	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为72MHz，如果需要更改，可以修改：
		\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\system_stm32f10x.c
		中配置系统时钟的宏。
	*/

	bsp_Init();
	PrintfLogo();		/* 打印例程Logo到串口1 */

	//bsp_InitIRD();		/* 配置GPIO  */

	/* 打印菜单 */
	printf("请操作红外遥控器，观察键值\r\n");
	printf(" 主板K1键 = 开始解码红外信号\r\n");
	printf(" 主板K2键 = 停止解码红外信号\r\n");

	IRD_StartWork();
	while (1)
	{
		bsp_Idle();		/* 空闲时执行的函数,比如喂狗. 在bsp.c中 */

		/* 按键滤波和检测由后台systick中断服务程序实现，我们只需要调用bsp_GetKey读取键值即可。 */
		ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			/* 主板K1键按下 */
					IRD_StartWork();		/* 开始解码红外遥控信号 */
					printf("K1 开始红外解码\r\n");
					break;

				case KEY_DOWN_K2:			/* 主板K2键按下 */
					IRD_StopWork();			/* 停止解码红外遥控信号 */
					printf("K1 停止红外解码\r\n");
					break;

				/* 下面是红外遥控器的事件 */
				case IR_KEY_POWER:
					printf("IR Power\r\n");
					break;

				case IR_KEY_MENU:
					printf("IR Menu\r\n");
					break;

				case IR_KEY_TEST:
					printf("IR Test\r\n");
					break;

				case IR_KEY_UP:
					printf("IR +\r\n");
					break;

				case IR_KEY_RETURN:
					printf("IR Return\r\n");
					break;

				case IR_KEY_LEFT:
					printf("IR Left\r\n");
					break;

				case IR_KEY_OK:
					printf("IR Ok\r\n");
					break;

				case IR_KEY_RIGHT:
					printf("IR Right\r\n");
					break;

				case IR_KEY_0:
					printf("IR 0\r\n");
					break;

				case IR_KEY_DOWN:
					printf("IR -\r\n");
					break;

				case IR_KEY_C:
					printf("IR C\r\n");
					break;

				case IR_KEY_1:
					printf("IR 1\r\n");
					break;

				case IR_KEY_2:
					printf("IR 2\r\n");
					break;

				case IR_KEY_3:
					printf("IR 3\r\n");
					break;

				case IR_KEY_4:
					printf("IR 4\r\n");
					break;

				case IR_KEY_5:
					printf("IR 5\r\n");
					break;

				case IR_KEY_6:
					printf("IR 6\r\n");
					break;

				case IR_KEY_7:
					printf("IR 7\r\n");
					break;

				case IR_KEY_8:
					printf("IR 8\r\n");
					break;

				case IR_KEY_9:
					printf("IR 9\r\n");
					break;

				default:
					if (ucKeyCode >= IR_KEY_STRAT)
					{
						printf("IR 无法识别的遥控按键\r\n");
						printf("%02X %02X %02X %02X\r\n", g_tIR.RxBuf[0], g_tIR.RxBuf[1],
							g_tIR.RxBuf[2], g_tIR.RxBuf[3]);
					}
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
			__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("* \n\r");	/* 打印一行空格 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");
}
