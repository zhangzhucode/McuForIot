/*
*********************************************************************************************************
*
*	模块名称 : main模块
*	文件名称 : main.h
*	版    本 : V1.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _MAIN_H_
#define _MAIN_H_

/* 用于LCD界面标题 */
#define VER_INFO	"安富莱STM32-V4开发板  (V1.0d)"

/* 用于串口打印 */
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"999_STM32-V4出厂测试程序"
#define EXAMPLE_DATE	"2015-08-09"
#define DEMO_VER	"1.0d"

/* 主程序状态字定义, MS = Main Status */
enum
{
	MS_MAIN_MENU = 0,	/* 主界面 */
	
	MS_HARDWARE_INFO,	/* 硬件信息 */
	MS_TEST_TOUCH,		/* 触摸屏和按键 */
	MS_RADIO,			/* 收音机 */	
	MS_WM8978_REC,		/* WM8978录音机 */	
	MS_MEMS,			/* MEMS 传感器测试（I2C） 三轴陀螺仪，磁力计，气压计 */		
	MS_ETH_WEB,			/* 以太网DM9000和DM9161 WEB服务器 */
	
	MS_GPS,				/* GPS测试 */
	MS_GPRS,			/* GPRS模块测试 */	
	MS_MP3			,	/* MP3播放器状态 */	
	MS_MASS_STORAGE,	/* SD卡，NAND模拟U盘 */		
	MS_CAMERA,			/* 摄像头 */
	MS_OLED,			/* OLED显示器 */
	
	MS_AD7606,			/* AD7606数据采集模块测试 */		
	MS_AD7705,			/* AD7705模块 */
	MS_ADS1256,			/* ADS1256模块 */
	MS_RS485,			/* RS485通信 */
	MS_DAC8501,			/* DAC8501模块 */
	MS_DAC8562,			/* DAC8562模块 */
	MS_RA8875,			/* RA8875字库  */

	MS_CALIBRATION	,	/* 校准触摸状态 */	

	MS_CAN,				/* CAN通信 */	
	
	MS_DSO,				/* 示波器 */
	MS_WIFI,			/* wifi模块 */		
	
	MS_STEP_MOTO,		/* 5V减速步进电机，28BYJ48 5线 */	
	MS_AD9833,			/* AD9833波形发生器模块 */
	
	MS_RESERVED			/* 保留的功能状态 */
};

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

