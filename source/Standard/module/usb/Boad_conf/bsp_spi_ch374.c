/*********************************************************************************************************
**                                    
** -----------------------------------------------------------------------------------------------------
** 文件名  : bsp_spi_ch374.c	
** 版本    : BB_usb_ZZ_1.0
** 简介    : 用硬件标准SPI接口产生CH374的4线SPI串口读写时序 

** -----------------------------------------------------------------------------------------------------
** 作者     : zhangzhu   
** 日期     : 2012-08-23
** -----------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "includes.h"

/****************************************************************
**              片选函数 
**			    SPI2_/CS--PB12
******************************************************************/

// Select SPI CH374: ChipSelect pin low 
void Select_CH374(void)
{
     GPIO_ResetBits(GPIOB, GPIO_Pin_12);    //PB12 =0
}
// Deselect SPI CH374: ChipSelect pin high 
void NotSelect_CH374(void)  
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);      //PB12 =1 
}

/*******************************************************************************
*            	SPI 初始化
//--------------------------------------------------------------
* Function Name  : SPI_CH374_Init	 
*
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
*******************************************************************************/
void SPI_CH374_Init(void)
{

  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  NVIC_InitTypeDef  NVIC_InitStructure;
   RS232_PutString("\r\n===========================================\r\n");  
  //(1) Enable SPI2 GPIO clocks 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
 
  //(2) Configure SPI2 pins: SCK, MISO and MOSI 
  //PB13--SPI2_SCK
  //PB14--SPI2_MISO
  //PB15--SPI2_MOSI
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure PB12 as Output push-pull, used as Flash Chip select 
  //PB12--SPI2_/CS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //PD10---/USB_INT
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // Enable the EXTI10 Interrupt 						
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;						
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;						
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;						
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
  NVIC_Init(&NVIC_InitStructure);						

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10);
  EXTI_InitStructure.EXTI_Line=EXTI_Line10;               // EXTI10
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;      // Interrupt mode
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  // /INT: Low Level trigger
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;                // Interrupt enable
  EXTI_Init(&EXTI_InitStructure);	
   						
  
  NotSelect_CH374();  //禁止SPI片选 

  //(3) SPI2 configuration 
  // reference to CH374 datasheet
  // 设置SPI模式3, MSB first, CPOL=1, CPHA=1, CH374也支持SPI模式0 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//full duplex
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;         //SPI Master Mode
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;     //Data Fram Format
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;           //Clock Polarity: SCK=1 in idle time 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;          //Clock Phase: Capture data in the 2st edge of SCK
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;             //NSS pin Management by Software
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//BaudRate = Fpclk/4=72MHz/4=18MHz < 28MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;    //MSB Bit send first          
  SPI_InitStructure.SPI_CRCPolynomial = 7;              //Polynomial for CRC calculation
  SPI_Init(SPI2, &SPI_InitStructure);

  //(4) SPI2 Enable
  SPI_Cmd(SPI2, ENABLE);   
  RS232_PutString("\r\nspi_init_ok");
}

/*******************************************************************************
* Function Name  : SPI_CH374_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
unsigned char SPI_CH374_SendByte(unsigned char byte)
{
    
  // Loop while DR register in not emplty 
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  
  // Send byte through the SPI1 peripheral 
  SPI_I2S_SendData(SPI2, byte);

  // Wait to receive a byte 
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);


  // Return the byte read from the SPI bus 
  return SPI_I2S_ReceiveData(SPI2);

}


/*******************************************************************************
* Function Name  : SPI_CH374_ReadByte
* Description    : Reads a byte from the SPI CH374.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
unsigned char SPI_CH374_ReadByte(void)
{
  return (SPI_CH374_SendByte(0xFF));
}






/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
