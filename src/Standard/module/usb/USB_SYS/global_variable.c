/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : global_variable.c	
** Version      : V1.0
** Description  : global variable define
** -----------------------------------------------------------------------------------------------------
** Author       : WuPengbo  
** Date         : 01/25/2011
** -----------------------------------------------------------------------------------------------------
** Modify Person:
** Modify Date  :
** Description  :
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>	/* 该文件包含了所有必需的.h文件 */


/* Global variant -------------------------------------------------------------*/
/*
u8 G_Version[2]={0x01,0x00};//V1.0

//Car information
//Car_Info G_Car_Info={CAR_STOP,0,0,0};
 */
/*
特征系数   ----- 03Bytes
驾驶员代码 ----- 03Bytes
驾驶证号码 ----- 18Bytes: ASCII code
车辆VIN    ----- 17Bytes: ASCII code 
车牌号码   ----- 12Bytes: 2字节Hex(1个汉字)+7个ASCII码（7个数字）+3字节Hex(备用)  
车牌分类   ----- 12Bytes: 8字节Hex(4个汉字)+4字节Hex(备用)
*/
/*
u32 G_CarCharValue=624;	//车辆特征系数 : 每公里传感器转数: R/km
u8  G_CarSensorValue=8;	//传感系数     : 每圈的传感器输出的脉冲个数
u32 G_Car_KValue=4992;	//车辆特征系数  * 传感系数 : (624*8)

u32 G_DriverNum=0;          //驾驶员代码
u8  G_CarVINNum[17]={0};    //车辆VIN
u8  G_CarNum[12]={0};       //车牌号码
u8  G_CarType[12]={0};      //车牌分类


//u8  G_MinRecord[RecordLen_360h]={0};    //TMile_L1,TMile_L2,TMile_H1,TMile_H2,MinMile_L,MinMile_H,Min,Hour,Day,Month,Year
//u8  G_StopCar_LastMinState=LASTMIN_60S;
u8  G_CurMinRecord_Empty=EMPTY;	// 当前分钟记录是否为空

//Current is LCD Working menu
u8 G_MenuState_flag=ID_Working;        

//AT45DB081D buffer array
u8 G_Flash_Buf[FLASH_PAGESIZE]={0};        //FlashBufSize=264

//U Disk Sector buffer
u8 G_UDISKSector_Buf[UDISK_SECTORSIZE]={0};  //Udisk_SecotrSize=512

//Serial
u8  G_Serial_Buf[150]={0};
u8  G_Serial2_CurData=0;
u8	G_Serial1_RECV=ENABLE;
	
//Printf Com number: COM1,COM2
u8 G_Printf_ComNum=COM1;

//Timer4 counter for returning to main menu
u8 G_Timer4_Cnt=0;  

//LCD BackLight Status
u8 G_LCD_BL_Status=BL_ON;
*/
/*********************************************************************************************************
** Function Name: delay_nus
** Description  : delay n us
** Input	    : n
** Output	    : None
** Return	    : None
********************************************************************************************************/
/*void delay_nus(u16 n)  //延时n us: n>=6,最小延时单位6us
{ 
  u16 j;
  while(n--)              // 外部晶振：8M；PLL：9；8M*9=72MHz
  {
    j=8;				  // 微调参数，保证延时的精度
	while(j--);
  }
}
 */
/*********************************************************************************************************
** Function Name: delay_nms
** Description  : delay n ms
** Input	    : n
** Output	    : None
** Return	    : None
********************************************************************************************************/
/*
void delay_nms(u16 n)  //延时n ms
{
  while(n--)		   // 外部晶振：8M；PLL：9；8M*9=72MHz
  {
	 delay_nus(1100);   // 1ms延时补偿
  	 IWDG_ReloadCounter();//重启计数器（喂狗）
  }
    
}
*/

/*********************************************************************************************************
** Function Name: Hex_BCD_Array
** Description  : Hex array converted to BCD array
** Input	    : Hex: point to Hex array
                  Len: array length
** Output	    : BCD: point to BCD array
** Return	    : None
********************************************************************************************************/
void Hex_BCD_Array(u8 *Hex,u8 *BCD,u8 Len)
{
//    u8 i=0;

    //for(i=0;i<Len;i++)
//        BCD[i]=HEX2BCD_1Byte(Hex[i]);
    
}


/*******************************************************************************
* Function Name  : Byte2ASCII
* Description    : Convert 1 Byte to ASCII                   
* Input          : hex: hex data                   
* Output         : pBuf[0]: High 4bit
                   pBuf[1]: Low  4bit                  
* Return         : None
*******************************************************************************/
void Byte2ASCII(u8 hex,u8 *pBuf)
{
//    u8 temp;

    //Low 4bit
//    temp= hex & 0x0f;
//    pBuf[1]=HEX2ASCII_HalfByte(temp);

    //High 4bit
//    temp= hex >> 4;
//    pBuf[0]=HEX2ASCII_HalfByte(temp);
}


/*******************************************************************************
* Function Name  : Hex2Dec_16bit
* Description    : Convert 16 bit Hex to Decmial                   
* Input          : MSB-->LSB
                   Hex_in[0-1]                    
* Output         : Dec_out[0-1]                   
* Return         : None
*******************************************************************************/
void Hex2Dec_16bit(u8 *Hex_in,u8 *Dec_out)
{
    u16 temp;

    temp =Hex_in[0] << 8;
    temp+=Hex_in[1];

    Dec_out[0]=temp/100;
    Dec_out[1]=temp%100;

}


/*******************************************************************************
* Function Name  : Hex2Dec_32bit
* Description    : Convert 32 bit Hex to Decmial                   
* Input          : MSB-->LSB
                   Hex_in[0-3]                    
* Output         : Dec_out[0-3]                   
* Return         : None
*******************************************************************************/
void Hex2Dec_32bit(u8 *Hex_in,u8 *Dec_out)
{
    u32 temp;

    temp =Hex_in[0] << 24;
    temp+=Hex_in[1] << 16;
    temp+=Hex_in[2] << 8;
    temp+=Hex_in[3];
    
    Dec_out[0]=temp/1000000; //100_00_00
    temp=temp%1000000;
    Dec_out[1]=temp/10000;   //100_00;
    temp=temp%10000;
    Dec_out[2]=temp/100;
    Dec_out[3]=temp%100;

}



/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
