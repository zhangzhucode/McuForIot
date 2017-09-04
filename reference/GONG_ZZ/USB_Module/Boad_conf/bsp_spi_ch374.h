/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : bsp_SPI_CH374.h	
** Version      : V1.0
** Description  : 
** -----------------------------------------------------------------------------------------------------
** Author       : WuPengbo  
** Date         : 04/22/2011
** -----------------------------------------------------------------------------------------------------
** Modify Person:
** Modify Date  :
** Description  :
*********************************************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------------------*/
#ifndef __BSP_SPI_CH374_H 
#define __BSP_SPI_CH374_H

/* Includes ---------------------------------------------------------------------------*/



// SPI2_/CS--PB12
// Select SPI CH374: ChipSelect pin low 
void Select_CH374(void);
// Deselect SPI CH374: ChipSelect pin high 
void NotSelect_CH374(void);


/*******************************************************************************
* Function Name  : SPI_CH374_Init
* Description    : Initializes the peripherals used by the SPI CH374 driver.
                   (1)如果是硬件SPI接口,那么可使用mode3(CPOL=1&CPHA=1)或
                      mode0(CPOL=0&CPHA=0),
                      CH374在时钟上升沿采样输入,下降沿输出,数据位是高位在前
                   (2)Pins: 
                   PB13--SPI2_SCK
                   PB14--SPI2_MISO
                   PB15--SPI2_MOSI
                   PB12--SPI2_/CS
                   PD10---/USB_INT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_CH374_Init(void);



/*******************************************************************************
* Function Name  : SPI_CH374_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
unsigned char SPI_CH374_SendByte(unsigned char byte);



/*******************************************************************************
* Function Name  : SPI_CH374_ReadByte
* Description    : Reads a byte from the SPI CH374.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
unsigned char SPI_CH374_ReadByte(void);



#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/



