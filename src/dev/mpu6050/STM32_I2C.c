#include "STM32_I2C.h"

#define CMD_Write     0		 //mpu6050 写命令位最低位有效
#define CMD_Read      1		 //mpu6050 读命令位最低位有效
/*****
***	 	IIc引脚：SDA PE0 , SCL PE1 
***
*******/
#define SCL_H         GPIOE->BSRR = GPIO_Pin_1 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOE->BRR  = GPIO_Pin_1 /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOE->BSRR = GPIO_Pin_0 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOE->BRR  = GPIO_Pin_0 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read      GPIOE->IDR  & GPIO_Pin_1 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIOE->IDR  & GPIO_Pin_0 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */

void i2cInit(void)
{
    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOE, &gpio);
}

/*****************************************************************************************************/
static void I2C_delay(void)
{
    volatile int i =10 ;	  //zz 7->10
    while (i)
        i--;
}

static bool I2C_Start(void)
{
    SDA_H;
    SCL_H;
    I2C_delay();
    if (!SDA_read)
        return FALSE;
    SDA_L;
    I2C_delay();
    if (SDA_read)
        return FALSE;
    SDA_L;
    I2C_delay();
    return TRUE;
}

static void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}

static void I2C_Ack(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static void I2C_NoAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static bool I2C_WaitAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    if (SDA_read) {
        SCL_L;
        return FALSE;
    }
    SCL_L;
    return TRUE;
}

static void I2C_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--) {
        SCL_L;
        I2C_delay();
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}

static uint8_t I2C_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    SDA_H;
    while (i--) {
        byte <<= 1;
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        if (SDA_read) {
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}

/********************************************************************************
** 函数名： bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
** 函数功能：写入数据块
** 参量：
      uint8_t addr,       模块地址
	  uint8_t reg, 		  寄存器地址
	  uint8_t len, 		  数据长度
	  uint8_t * data	  数据
**返回值 true 成功  false 失败
*********************************************************************************/
bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(addr << 1 | CMD_Write);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    for (i = 0; i < len; i++) {
        I2C_SendByte(data[i]);
        if (!I2C_WaitAck()) {
            I2C_Stop();
            return FALSE;
        }
    }
    I2C_Stop();
    return TRUE;
}
/********************************************************************************
** 函数名： bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
** 函数功能：写入一个字节到指定寄存器
** 参量：
      uint8_t addr,       模块地址
	  uint8_t reg, 		  寄存器地址
	  uint8_t data  	  数据
**返回值 true 成功  false 失败
*********************************************************************************/
bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(addr << 1 | CMD_Write);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
    return TRUE;
}
/********************************************************************************
** 函数名： bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
** 函数功能：读数据从指定寄存器
** 参量：
      uint8_t addr,       模块地址
	  uint8_t reg, 		  寄存器地址
	  uint8_t len, 	      读取长度
	  uint8_t *buf		  读取到的空间
**返回值 true 成功  false 失败
*********************************************************************************/
bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(addr << 1 | CMD_Write);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(addr << 1 | CMD_Read);
    I2C_WaitAck();
    while (len) {
        *buf = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return TRUE;
}

uint16_t i2cGetErrorCounter(void)
{
    // TODO maybe fix this, but since this is test code, doesn't matter.
    return 0;
}
