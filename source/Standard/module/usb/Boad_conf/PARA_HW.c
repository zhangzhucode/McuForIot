/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : PARA_HW.c	
** Version      : V1.0
** Description  : 用硬件标准SPI接口产生CH374的4线SPI串口读写时序 

** -----------------------------------------------------------------------------------------------------
** Author       : WuPengbo  
** Date         : 04/22/2011
** -----------------------------------------------------------------------------------------------------
** Modify Person:
** Modify Date  :
** Description  :
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include	"HAL.H"
#define		CH374HF_NO_CODE		1
#include	"CH374HFM.H"
#include	"CH374INC.H"
#include	"bsp_spi_ch374.h"

#include <RS232.h>


/*******************************************************************************
* Function Name  : CH374_PORT_INIT
* Description    : Initializes the peripherals used by the SPI CH374 driver.
                   PB13--SPI2_SCK
                   PB14--SPI2_MISO
                   PB15--SPI2_MOSI
                   PB12--SPI2_/CS
                   PD10---/USB_INT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH374_PORT_INIT( )  
{
    //SPI_CH374_Init();

}

/* SPI输出8个位数据 */
#define	Spi374OutByte( d )		SPI_CH374_SendByte(d)   
/* SPI输入8个位数据 */
#define	Spi374InByte( )			SPI_CH374_ReadByte()	


/* SPI开始 */
static	void	Spi374Start( UINT8 addr, UINT8 cmd )  
{
	Select_CH374();          /* SPI片选有效 */
	Spi374OutByte( addr );
	Spi374OutByte( cmd );
}

/* SPI结束 */
#define	Spi374Stop( )	{ NotSelect_CH374(); }  



//======================================================================================
/* SPI接口子程序,供CH374子程序库调用 */
//======================================================================================

UINT8	CH374_READ_REGISTER( UINT8 mAddr )  /* 外部定义的被CH374程序库调用的子程序,从指定寄存器读取数据 */
{
	UINT8	d;
	//RS232_PutString("\r\nspi_1");
	Spi374Start( mAddr, CMD_SPI_374READ );
	d = Spi374InByte( );
	Spi374Stop( );
	return( d );
}

void	CH374_WRITE_REGISTER( UINT8 mAddr, UINT8 mData )  /* 外部定义的被CH374程序库调用的子程序,向指定寄存器写入数据 */
{
	//RS232_PutString("\r\nspi_2");
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	Spi374OutByte( mData );
	Spi374Stop( );
}

void	CH374_READ_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf )  /* 外部定义的被CH374程序库调用的子程序,从指定起始地址读出数据块 */
{
	RS232_PutString("\r\nspi_3");
	Spi374Start( mAddr, CMD_SPI_374READ );
	while ( mLen -- ) *mBuf++ = Spi374InByte( );
	Spi374Stop( );
}

void	CH374_WRITE_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8  mBuf )  /* 外部定义的被CH374程序库调用的子程序,向指定起始地址写入数据块 */
{
	RS232_PutString("\r\nspi_4");
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	while ( mLen -- ) Spi374OutByte( *mBuf++ );
	Spi374Stop( );
}

PUINT8 	CH374_READ_BLOCK64( UINT8 mAddr, PUINT8  mBuf )  /* 外部定义的被CH374程序库调用的子程序,从双缓冲区读出64字节的数据块,返回当前地址 */
{
	UINT8	i;
	RS232_PutString("\r\nspi_5");
	Spi374Start( mAddr, CMD_SPI_374READ );
	for ( i = CH374_BLOCK_SIZE; i != 0; i -- ) *mBuf++ = Spi374InByte( );
	Spi374Stop( );
	return( mBuf );
}

PUINT8 	CH374_WRITE_BLOCK64( UINT8 mAddr, PUINT8  mBuf )  /* 外部定义的被CH374程序库调用的子程序,向双缓冲区写入64字节的数据块,返回当前地址 */
{
	UINT8	i;
	RS232_PutString("\r\nspi_6");
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	for ( i = CH374_BLOCK_SIZE; i != 0; i -- ) Spi374OutByte( *mBuf++ );
	Spi374Stop( );
	return( mBuf );
}

void	CH374_WRITE_BLOCK_C( UINT8 mLen, PUINT8C mBuf )  /* 外部定义的被CH374程序库调用的子程序,向RAM_HOST_TRAN写入常量型数据块 */
{
	RS232_PutString("\r\nspi_7");
	Spi374Start( RAM_HOST_TRAN, CMD_SPI_374WRITE );
	do {
		Spi374OutByte( *mBuf );
		mBuf ++;
	} while ( -- mLen );
	Spi374Stop( );
}




/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
