/********************************************************************/
#include "includes.h"
#include "SPI1.h"

#define  SPI_Send16Address(Address) {    \
SPI_Send_Byte( (uint8)(Address>>8) );    \
SPI_Send_Byte( (uint8)(Address&0xff) );  \
}


/*
   7   6    5    4    3    2    1    0 
 SPIE SPE DORD MSTR CPOL CPHA SPR1 SPR0
                                                                             
 7 SPIE If bonabled.        th SPIE and ES are set to one, SPI interrupts are e
 6 SPE SPI en               able bit.                                          
    0: Disa               bles SPI.                                          
    1: Enabns P1.4, P1.5, les SPI and connects SS#, MOSI, MISO, and SCK to piP1.6, P1.7.
 5 DORD Data                Transmission Order.                                
    0: MSB                first in data transmission.  (y)                        
    1: LSB                first in data transmission.                        
 4 MSTR Maste               r/Slave select.                                    
    0: Sele               cts Slave mode.                                    
    1: Sele               cts Master mode.             (y)                      
 3 CPOL Clock                Polarity                                          
    0: SCK                is low when idle (Active High).  (y)                  
    1: SCK                is high when idle (Active Low).                    
 2 CPHA Clock                Phase control bit.                                
    0: Shif               t triggered on the leading edge of the clock. (y)     
    1: Shif               t triggered on the trailing edge of the clock.     
 1 0 SPR1, SPR0 the SCK rate o SPI Clock Rate Select bits. These two bits controlf the device
 configuredve. The relatio as master. SPR1 and SPR0 have no effect on the slanship
 between SC               K and the oscillator frequency, fOSC, is as follows
*/

void SPI1_Init(void)
{
  	SPI_InitTypeDef  SPI_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
  
   /* Enable SPI1 and GPIOA clocks */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

   /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//SPI_CPOL_High=模式3，时钟空闲为高 //SPI_CPOL_Low=模式0，时钟空闲为低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//SPI_CPHA_2Edge;//SPI_CPHA_1Edge, SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//SPI_NSS_Soft;//SPI_NSS_Hard
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//SPI_BaudRatePrescaler_2=18M;//SPI_BaudRatePrescaler_4=9MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据从高位开始发送
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  SPI_Init(SPI1, &SPI_InitStructure);
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);   
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI1_Send_Byte(u8 dat)
{
  	/* Loop while DR register in not emplty */
  	//while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  	/* Send byte through the SPI1 peripheral */
  	//SPI_SendData(SPI1, dat);

  	/* Wait to receive a byte */
  	//while(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

  	/* Return the byte read from the SPI bus */
  	//return SPI_ReceiveData(SPI1);
	return 0;
}

