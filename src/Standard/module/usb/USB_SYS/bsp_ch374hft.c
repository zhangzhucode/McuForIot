/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : bsp_ch374hft.c	
** Version      : V1.0
** Description  : CH374 Host Filesystem interface
                  Support:  FAT12/FAT16/FAT32

** -----------------------------------------------------------------------------------------------------
** Author       : WuPengbo  
** Date         : 02/01/2011
** -----------------------------------------------------------------------------------------------------
** Modify Person:
** Modify Date  :
** Description  :
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
/*
#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"
#include "bsp_printf.h"
#include "bsp_button.h"
#include "bsp_hw_cfg.h"
*/
#include <includes.h>	// 该文件包含了所有必需的.h文件 


/* 以下定义的详细说明请看CH374HFM.H文件 */
#define LIB_CFG_INT_EN			0		   // CH374的INT#引脚连接方式,0为"查询方式",1为"中断方式" 
//#define NO_DEFAULT_CH374_INT	1		   // 禁止CH374HF6.H中的默认中断处理程序,使用下面自行编写的中断程序 

// 只使用单片机内置的1KB外部RAM: 0000H-01FFH 为磁盘读写缓冲区, 以字节为单位读写文件不需要文件数据读写缓冲区FILE_DATA_BUF  
#define	DISK_BASE_BUF_ADDR		0x0000	// 外部RAM的磁盘数据缓冲区的起始地址,从该单元开始的缓冲区长度为SECTOR_SIZE  
#define DISK_BASE_BUF_LEN		4096
#define CH374_INT_WIRE			GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)	// PD10, INT0, CH374的中断线INT#引脚,连接CH374的INT#引脚,用于查询中断状态  
// 如果未连接CH374的中断引脚,那么应该去掉上述定义,自动使用寄存器查询方式  

#define NO_DEFAULT_CH374_F_ENUM		1		// 未调用CH374FileEnumer程序故禁止以节约代码  
#define NO_DEFAULT_CH374_F_QUERY	1		// 未调用CH374FileQuery程序故禁止以节约代码  
//#define NO_DEFAULT_CH374_RESET		1		// 未调用CH374Reset程序故禁止以节约代码  

#include "CH374HFM.H"		// 包含CORTEX-M3的库头文件  
#include "bsp_CH374hft.h"

#define USB_DataBuf_size         512   

u8      USB_DataBuf[USB_DataBuf_size] = {0};

CH374_Flag_t    CH374_Flag;

USB_Flag_e     USB_Flag;
//************************内部函数*********************************
extern u8 USB_FILE_READ(char *fname,u16 length ,u8* data_buf ); 
extern u8 USB_FILE_Create(char *fname);
extern u8 USB_FILE_WriteX(u8* addr);
/*
void xQueryInterrupt( void )
{
   while(USB_Flag.Transf_one ==FALSE);
   USB_Flag.Transf_one =FALSE;
}  */
/********************************************************************************************
**	函数名称 ：void EXTI15_10_IRQHandler(void)
**	函数功能 ：usb中断处理
**	输出接口 ：
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/


void EXTI15_10_IRQHandler(void)
{
    u8 USB_EXTI_Flag;
	OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();          //进入 临界段
    OSIntNesting++;
    OS_EXIT_CRITICAL();           //退出临界段

	//**************************************************************
	//***代码区*********
	//---------------------------------------------

	 if(EXTI_GetITStatus(EXTI_Line10) != RESET)
     {
	     //Write_To_Log( UDISK_TASK ,"Enter USB EXTI_IRQHandler !!");

		 USB_EXTI_Flag =CH374_READ_REGISTER( REG_INTER_FLAG );					//读取ch374中断
		 //RS232_PutString("USB_EXTI_Flag:");
		 //RS232_PutWord(USB_EXTI_Flag,8);
		 if((USB_EXTI_Flag & BIT_IF_DEV_DETECT)!=0)						    	//热插拔中断
		 {
		     if((USB_EXTI_Flag & BIT_IF_DEV_ATTACH)!=0)
			 {
			     #ifdef EN_HUB_DISK
		           CH374vHubPortCount = 0;                 
                  #endif
        
			 	 CH374_WRITE_REGISTER( REG_USB_SETUP, BIT_SETP_HOST_MODE );	    //设置主机模式
				 //Write_To_Log( UDISK_TASK ,"USB_Enter..........");
				 USB_Flag.In_Out =TRUE ;
				 RS232_PutString("\r\nUSB_Enter..........");
			 }
			 else
			 {
			 	 //Write_To_Log( UDISK_TASK ,"USB_Exit..........");
				 RS232_PutString("\r\nUSB_Exit_1..........");
				 //USB_Flag.In_Out =FALSE ;			 
			 }
			 CH374_WRITE_REGISTER( REG_INTER_FLAG,BIT_IF_USB_PAUSE| BIT_IF_DEV_DETECT ); //清除
		 }
		 if((USB_EXTI_Flag & BIT_IF_TRANSFER)!=0)								//数据传输完成中断
		 {
		 	 if((USB_EXTI_Flag & BIT_IF_DEV_ATTACH)!=0)
			 {
			 	 //Write_To_Log( UDISK_TASK ,"USB_Trans_OK——one!!");
				 //RS232_PutString("\r\nUSB_Trans_OK——one!!");
				 USB_Flag.Transf_one =TRUE ;
			 }
			 else
			 {
			 	 //Write_To_Log( UDISK_TASK ,"USB_Exit..........");
				 RS232_PutString("\r\nUSB_Exit_2..........");
				 //USB_Flag.In_Out =FALSE ;			 
			 }	
			 //CH374_WRITE_REGISTER( REG_INTER_FLAG, BIT_IF_TRANSFER ); //清除	 
		 }

	 
	     EXTI_ClearITPendingBit(EXTI_Line10);
	 }
	//**************************************************************

    OSIntExit();
}
//硬件配置初始化
void USB_Init(void)
{
    SPI_CH374_Init();
}
/********************************************************************************************
**	函数名称 ：void USB_start(void)
**	函数功能 ：usb 启动
**	输出接口 ：
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/

void USB_start(void)
{
   	u8 temp;

	RS232_PutString("\r\nZZ--1\r\n");
    //CH374Reset();
    RS232_PutString("\r\nZZ--2\r\n");
	temp=CH374LibInit();               // 初始化CH374程序库和CH374芯片,操作成功返回0 
    if(temp==0)
	{
	    Write_To_Log( UDISK_TASK , "USB_Init_OK..........");
		CH374_Flag.State= TRUE;
		RS232_PutString("\r\nZZ--3\r\n");
	}
	else
	{
	   Write_To_Log( UDISK_TASK , "USB_Init_Error..........");
	   CH374_Flag.State = FALSE;
	   RS232_PutString("\r\nZZ--4\r\n");
	}
	RS232_PutString("\r\nZZ--5\r\n");
}
/********************************************************************************************
**	函数名称 ：void USB_Dealing(void)
**	函数功能 ：usb处理函数
**	输出接口 ：
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/



void USB_Dealing(void)
{
	  RS232_PutString("\r\nUSB_Dealing..........");

	  App_Task_ClrDog(UDISK_TASK);         //UDISK_TASK 任务喂狗 

	  if(USB_Flag.In_Out == TRUE)
      {
		   USB_Flag.State =TRUE;
		   RS232_PutString("\r\nUSB_Dealing1..........");
	  }
	  else if((USB_Flag.In_Out == FALSE)&&(USB_Flag.State == TRUE ))
      {
			USB_Flag.State =FALSE ;
			clrram((u8 *) &USB_Flag,sizeof(USB_Flag));
			RS232_PutString("\r\nUSB_Dealing2..........");
	  }

      if(CH374_Flag.State == FALSE )
	  {	  
	  	   USB_start();	 
		   RS232_PutString("\r\nUSB_Dealing3.........."); 
	  }
	  else if(USB_Flag.State == TRUE)
	  { 
		   RS232_PutString("\r\nUSB_Dealing4..........");
		   if(USB_Flag.ReadFlag_One==FALSE )	   //读1
		   {
		   		RS232_PutString("\r\nUSB_du1");
				if(0==USB_SetIC_READ())
				{
				     USB_Flag.ReadFlag_One=TRUE;
					 RS232_PutString("\r\nUSB_du2..........");
				}
				else
				{
					USB_Flag.State =FALSE ;
				//	CH374Reset();
				}
		   }
		   else if(USB_Flag.ReadFlag_Two==FALSE )	   //读2
		   { 
				 RS232_PutString("\r\nUSB_du3..........");
				 if(0==USB_SetXSJLY_READ())
				{
				    USB_Flag.ReadFlag_Two=TRUE;
					RS232_PutString("\r\nUSB_du4..........");
				} 
				else
				{
					USB_Flag.State =FALSE ;
				//	CH374Reset();
				}
		   }
		   else if(USB_Flag.WriteFlag ==FALSE)		   //写
		   {
		   		  RS232_PutString("\r\nUSB_xie1..........");
				  if(0==USB_CJData_Write())
				{
				    USB_Flag.WriteFlag=TRUE;
					RS232_PutString("\r\nUSB_xie2..........");
				} 
				else
				{
					USB_Flag.State =FALSE ;
				//	CH374Reset();
				}
		   }	 
	  }
	  
}

/********************************************************************************************
**	函数名称 ：u8 USB_FILE_READ((char *)fname,u16 length ,u8* data_buf ) 
**	函数功能 ：读取u盘的里面指定文件的数据
**	输出接口 ： 0 成功	1 失败 2未连接u盘 3u盘未准备好	4 无法打开文件
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/


u8 USB_FILE_READ(char* fname,u16 length ,u8* data_buf )  
{
    u8 success=1,temp,i;
	u8 GetNum;
    u16  Data_Count,j=0;
	RS232_PutString("\r\nUSB_du1_11");
	temp = CH374DiskConnect();
	if(temp != ERR_SUCCESS)
	{  
	    success=2; 
		RS232_PutString("\r\nUSB_du1_12");
	    return 	 success;
	}
	RS232_PutString("\r\nUSB_du1_13");
	temp = CH374DiskReady();
	RS232_PutString("\r\nUSB_du1_14");
	if(temp != ERR_SUCCESS)
	{  
	    success=3; 
		RS232_PutString("\r\nUSB_du1_15");
	    return 	 success;
	} 
    RS232_PutString("\r\nCH374DiskReady:");
    RS232_PutWord(temp,8);
    strcpy( (char *)mCmdParam.Open.mPathName,fname );
	RS232_PutString("\r\nUSB_du1_16");
	temp = CH374FileOpen();
	RS232_PutString("\r\nUSB_du1_17");
	if(temp != ERR_SUCCESS)
	{  
	    success=4; 
	   RS232_PutString("\r\nUSB_du1_18");
	    return 	 success;
	}                        		  
    RS232_PutString("\r\nUSB_du1_19");
	Data_Count = length +10;                       // 数据长度 
	while ( Data_Count ) 
    { 
		App_Task_ClrDog(UDISK_TASK);               //UDISK_TASK 任务喂狗 

		if ( Data_Count > MAX_BYTE_IO ) 		   //计算当次读取长度
        {   GetNum= MAX_BYTE_IO;  	  }
		else 
        {    GetNum = Data_Count;     }
        
		mCmdParam.ByteRead.mByteCount = GetNum;    //请求读出数据 
		temp = CH374ByteRead( );  
	    if ( temp != ERR_SUCCESS ) 
	    {
            success=1;
			break;
	    }
		Data_Count -= mCmdParam.ByteRead.mByteCount;  //计数,减去当前实际已经读出的字符数 
		for ( i=0; i<mCmdParam.ByteRead.mByteCount; i++,j++ )
		{
            data_buf[j]=mCmdParam.ByteRead.mByteBuffer[i] ;
		}
        if ( mCmdParam.ByteRead.mByteCount < GetNum ) 
		{  
		    success=0;
			break;
		}
	}
	temp = CH374FileClose( );
	if(temp != ERR_SUCCESS)
    {  success=4; }
	return success;
}
/********************************************************************************************
**	函数名称 ：U8 USB_SetIC_READ(void)
**	函数功能 ：usb处理函数
**	输出接口 ：0 成功	 1 失败
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/

u8 USB_SetIC_READ(void)
{
	u8  temp,success;

	clrram(USB_DataBuf,USB_DataBuf_size);
    temp = USB_FILE_READ("/ZZ.TY",0x68 ,USB_DataBuf );
		RS232_PutString("\r\nUSB_du1_01");
	if(temp ==0)
	{	
	    Write_To_Log2(UDISK_TASK,"Read USB_DRIVERID_Data:",1,(u32)USB_DataBuf,0x68);

	    RS232_PutString("\r\nUSB_du1_02");
	    success =0;
	}
	else if(temp ==4)   //无文件打开
	{
		 Write_To_Log( UDISK_TASK , "NO DRIVERID.TY");
		 success =0;
		 	RS232_PutString("\r\nUSB_du1_03");
	}
	else
	{
	    success =1;
	}
	return 	 success;
}
/********************************************************************************************
**	函数名称 ：U8 USB_SetXSJLY_READ(void)
**	函数功能 ：usb处理函数
**	输出接口 ：0 成功	 1 失败
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/
u8 USB_SetXSJLY_READ(void)
{
	u8  temp,success;

	clrram(USB_DataBuf,USB_DataBuf_size);
    temp = USB_FILE_READ("/GBSETUP.TY",0x68 ,USB_DataBuf ); 
	if(temp ==0)
	{	
	    Write_To_Log2(UDISK_TASK,"Read USB_GBSETUP_Data:",1,(u32)USB_DataBuf,0x68);



	    success =0;
	}
	else if(temp ==4)   //无文件打开
	{
		 Write_To_Log( UDISK_TASK , "NO GBSETUP.TY");
		 success =0;
	}
	else
	{
	    success =1;
	}
	return 	 success;
}
/********************************************************************************************
**	函数名称 ：u8 USB_FILE_Create((char *)fname) 
**	函数功能 ：在u盘里新建文件并打开
**	输出接口 ： 0 成功 1 无法创建文件	2未连接u盘 3u盘未准备好	
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/
u8 USB_FILE_Create(char* fname)
{   
    u8  temp,success;
	RS232_PutString("\r\nUSB_xie001..........");
	temp = CH374DiskConnect();
	RS232_PutString("\r\nUSB_xie002..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=2; 
		RS232_PutString("\r\nUSB_xie003..........");
	    return 	 success;
	}
	RS232_PutString("\r\nUSB_xie004..........");
	temp = CH374DiskReady();
	RS232_PutString("\r\nUSB_xie005..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=3; 
		RS232_PutString("\r\nUSB_xie006..........");
	    return 	 success;
	} 
	RS232_PutString("\r\nUSB_xie007..........");
    strcpy( (char *)mCmdParam.Create.mPathName,"/ZZ.TY" );
	RS232_PutString("\r\nUSB_xie008..........");
	temp = CH374FileCreate();
	RS232_PutString("\r\nUSB_xie009..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=1; 
		RS232_PutString("\r\nUSB_xie010..........");
	    return 	 success;
	}       
	RS232_PutString("\r\nUSB_xie011..........");                 		  
	success=0;
	return 	 success;
}
/********************************************************************************************
**	函数名称 ：u8 USB_FILE_WriteX(u8* addr) 
**	函数功能 ：写入数据
**	输出接口 ： 0 成功 1 失败
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/
u8 USB_FILE_WriteX(u8* addr)
{
    u8 success;

    App_Task_ClrDog(UDISK_TASK);                   //UDISK_TASK 任务喂狗
	
    mCmdParam.Locate.mSectorOffset = 0xffffffff; //移动文件指针到文件末尾 
    CH374FileLocate();             
    mCmdParam.WriteX.mSectorCount = 1;
    mCmdParam.WriteX.mDataBuffer = addr;  // 缓冲区 

	if ( CH374FileWriteX() != ERR_SUCCESS )
	{
	    success=1; 
	}                        		  
	success=0;
	return 	 success;

}

/********************************************************************************************
**	函数名称 ：U8 USB_CJData_Write(void)
**	函数功能 ：usb写入文件
**	输出接口 ：0 成功	 1 失败
**	作    者 ：  zz
**	时    间 ：	20120823
*********************************************************************************************/
u8 USB_CJData_Write(void)
{
    u8    temp,success,Index;
	char  FileName[15]={0};
	u8    RTCTime_Cur[6];

  	Index=Car_info.Car_No[0];
    for(temp=0;temp< Index;temp++)
	{ FileName[temp]= Car_info.Car_No[temp+1]; }	
	FileName[Index++]='.';
	FileName[Index++]='T';
	FileName[Index++]='Y';
    RS232_PutString("\r\nUSB_xie01..........");
    temp = USB_FILE_Create("/ZZ.TY");
	RS232_PutString("\r\nUSB_xie02..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=1; 
		RS232_PutString("\r\nUSB_xie03..........");
	    return 	 success;
	}
	RS232_PutString("\r\nUSB_xie04..........");
	//********************************************************
    //数据内容
		clrram(USB_DataBuf,USB_DataBuf_size);
		USB_DataBuf[0]=9;
		USB_DataBuf[1]=8;
		USB_DataBuf[2]=7;
		USB_DataBuf[3]=6;
		USB_DataBuf[4]=5;
		USB_DataBuf[5]=4;
		USB_DataBuf[6]=3;
		USB_DataBuf[7]=2;
		USB_DataBuf[8]=1;
	    //strcpy( (char *)USB_DataBuf,"zz-20120819" );
		RS232_PutString("\r\nUSB_xie05..........");
	    temp=USB_FILE_WriteX(USB_DataBuf);
		RS232_PutString("\r\nUSB_xie06..........");
		if(temp != ERR_SUCCESS)
	    {  
	    success=1; 
		RS232_PutString("\r\nUSB_xie07..........");
	    return 	 success;
	    }

		Write_To_Log2(UDISK_TASK,"write USB_DRIVERID_Data:",1,(u32)USB_DataBuf,20);
	//********************************************************

    mCmdParam.Locate.mSectorOffset = 0xffffffff; // 移动文件指针到文件末尾 
    CH374FileLocate();                          
    mCmdParam.WriteX.mSectorCount = 0;
    mCmdParam.WriteX.mDataBuffer = USB_DataBuf;  // 缓冲区 
    CH374FileWriteX();                           // 写目录的第一个扇区 


    //Get current time
    Time_Display(RTC_GetCounter(),RTCTime_Cur);           
	mCmdParam.Modify.mFileAttr = 0xff;            //   输入参数: 新的文件属性不修改
	mCmdParam.Modify.mFileTime = MAKE_FILE_TIME( RTCTime_Cur[RTC_HOUR], RTCTime_Cur[RTC_MIN], RTCTime_Cur[RTC_SEC]);//0xffff;//   输入参数: 新的文件时间,为0FFFFH则不修改,使用新建文件产生的默认时间
	mCmdParam.Modify.mFileDate = MAKE_FILE_DATE( (2000+RTCTime_Cur[RTC_YEAR]), RTCTime_Cur[RTC_MONTH], RTCTime_Cur[RTC_DAY]);//  输入参数: 新的文件日期: 2004.05.18
	mCmdParam.Modify.mFileSize = 0xffffffff;      //文件时自动更新长度,所以此处不修改                                          
	temp=CH374FileModify( );                     // 修改当前文件的信息
	if(temp != ERR_SUCCESS)
	{  
	    success=1; 
	    return 	 success;
	}
    if ( CH374FileClose()!= ERR_SUCCESS ) 			    // 关闭文件
    {  
	    success=1; 
	}    
	else
	{
	    success=0;
	}
	 return 	 success;                     
}  

