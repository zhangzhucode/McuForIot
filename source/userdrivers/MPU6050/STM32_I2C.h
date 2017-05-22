#ifndef __STM32_I2C_H
#define __STM32_I2C_H
#include "includes.h"
#include "stm32f10x.h"

/*====================================================================================================*/
/*====================================================================================================*/
extern void i2cInit(void);
extern bool i2cWriteBuffer(uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
extern bool i2cWrite(uint8_t addr_, uint8_t reg_, uint8_t data);
extern bool i2cRead(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf);
/*====================================================================================================*/
/*====================================================================================================*/
#endif
