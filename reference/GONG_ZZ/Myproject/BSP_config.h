#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

void Bsp_Configuration(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);
void SysTick_Configuration(void);
void Timer_Configuration(void);
void GPIO_Configuration(void);
void ADC_Configuration(void);
void DMA_Configuration(void);
void EXTI_Configuration(void);
void WWdg_Configuration(void);
void Bsp_RS232_Initialise(void);

#endif 
