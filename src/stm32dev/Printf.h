				

#ifndef __DRIVER_PRINTF_H
#define __DRIVER_PRINTF_H

#include "Printf.h"
#include "RS232.h"
#include "DriverConfig.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if CONFIG_DEV_PRINTF
	#pragma import(__use_no_semihosting)             
	//标准库需要的支持函数                 
	typedef struct
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}FILE; 
	/* FILE is typedef’ d in stdio.h. */ 
	FILE __stdout;
	       
	/*************定义_sys_exit()以避免使用半主机模式********************/    
	_sys_exit(int x) 
	{ 
		x = x; 
	} 
	/*************重定义fputc函数*****************************************/
	int fputc(int ch, FILE *f)
	{      
		u8 val = (u8)ch;
		RS232_PutChar(&val,1);      
		return ch;
	}
#else
	#define	 printf   RS232_PutString
#endif 
#endif

