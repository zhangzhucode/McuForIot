/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.2
*	说    明 : 按键检测和LED控制例程。核心文件为 bsp_timer.c bsp_key.c 和 bsp_led.c
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2013-02-01 armfly  首发
*		v1.1    2013-06-20 armfly  更新bsp代码; 更新打印提示信息。
*		V1.2    2014-02-28 armfly  升级固件库到V1.3.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"			/* 底层硬件驱动 */

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"V5-101_按键检测和LED控制例程"
#define EXAMPLE_DATE	"2014-02-28"
#define DEMO_VER		"1.2"

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
	uint8_t ucKeyCode;		/* 按键代码 */

	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
	配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
	*/

	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/* 打印例程信息到串口1 */

	printf("LED4正在闪烁(闪烁频率 = 1Hz)\r\n");
	printf("按下K1键点亮LED1\r\n");
	printf("按下K2键点亮LED2\r\n");
	printf("按下K3键点亮LED3\r\n");
	printf("操作按键和摇杆会打印按键事件\r\n");

	bsp_StartAutoTimer(0, 500);	/* 启动1个500ms的自动重装的定时器 */

	/* 进入主程序循环体 */
	while (1)
	{
		bsp_Idle();		/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */

		if (bsp_CheckTimer(0))	/* 判断定时器超时时间 */
		{
			/* 每隔500ms 进来一次 */
			bsp_LedToggle(4);	/* 翻转LED4的状态 */
		}

		/* 按键滤波和检测由后台systick中断服务程序实现，我们只需要调用bsp_GetKey读取键值即可。 */
		ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			/* K1键按下 */
					bsp_LedOn(1);
					printf("K1键按下, LED1点亮\r\n");
					break;

				case KEY_UP_K1:				/* K1键弹起 */
					bsp_LedOff(1);
					printf("K1键弹起, LED1熄灭\r\n");
					break;

				case KEY_DOWN_K2:			/* K2键按下 */
					bsp_LedOn(2);
					printf("K2键按下, LED2点亮\r\n");
					break;

				case KEY_UP_K2:				/* K2键弹起 */
					bsp_LedOff(2);
					printf("K2键弹起, LED2熄灭\r\n");
					break;

				case KEY_DOWN_K3:			/* K3键按下 */
					bsp_LedOn(3);
					printf("K3键按下, LED3点亮\r\n");
					break;

				case KEY_UP_K3:				/* K3键弹起 */
					bsp_LedOff(3);
					printf("K3键弹起, LED3熄灭\r\n");
					break;

				case JOY_DOWN_U:			/* 摇杆UP键按下 */
					printf("摇杆上键按下\r\n");
					break;

				case JOY_DOWN_D:			/* 摇杆DOWN键按下 */
					printf("摇杆下键按下\r\n");
					break;

				case JOY_DOWN_L:			/* 摇杆LEFT键按下 */
					printf("摇杆左键按下\r\n");
					break;

				case JOY_DOWN_R:			/* 摇杆RIGHT键按下 */
					printf("摇杆右键按下\r\n");
					break;

				case JOY_DOWN_OK:			/* 摇杆OK键按下 */
					printf("摇杆OK键按下\r\n");
					break;

				case JOY_UP_OK:				/* 摇杆OK键弹起 */
					printf("摇杆OK键弹起\r\n");
					break;

				default:
					/* 其它的键值不处理 */
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
	/* 检测CPU ID */
	{
		/* 参考手册：
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\nCPU : STM32F407IGT6, LQFP176, UID = %08X %08X %08X\n\r"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* 打印一行空格 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* 旺旺  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* 淘宝店: armfly.taobao.com\r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
