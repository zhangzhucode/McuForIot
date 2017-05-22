
#include <includes.h>	
#include "DriverConfig.h"
#include "leddriver.h"

void leds_gpio_init(void);

void leds_gpio_contrl(unsigned char gpio_n, unsigned char val);

#define LEDS_GPIO_BUF_WIDTH 10

unsigned char leds_gpio_buf[LEDS_GPIO_BUF_WIDTH] = {0};

unsigned char gpio_count = 0;

void led_gpio_state_clean(unsigned char val)
{
    leds_gpio_contrl(val, 0);
}
void led_gpio_state_set(unsigned char gpio_n, unsigned char val)
{
    leds_gpio_contrl(gpio_n, val);
}
void leds_init(void)
{
    leds_gpio_init();
}
void led_set_gpio(unsigned char *buf,uint16_t width )
{
    unsigned char i;
    
    if(width<=0)return;
    
    for(i=0;i<LEDS_GPIO_BUF_WIDTH;i++)
    {
        if(i == width)break;
            
        leds_gpio_buf[i] = *(buf+i);  
    }
 
}
void led_set_gpio_off(void)
{
    unsigned char i;
    
    for(i=0;i<LEDS_GPIO_BUF_WIDTH;i++)
    {
            
        leds_gpio_buf[i] = 0;  
    }
}
void leds_10ms_deal(void)
{
    led_gpio_state_clean(gpio_count);
    
    gpio_count++;
    if(gpio_count>= LEDS_GPIO_BUF_WIDTH)gpio_count = 0;
    
    led_gpio_state_set(gpio_count,leds_gpio_buf[gpio_count]);
    
}

/***********************************************************************************************
**
**
**   ≈‰÷√ledµƒgpio µœ÷
**
***********************************************************************************************/
void leds_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 						 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12 |GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
      
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    GPIO_ResetBits(GPIOC, GPIO_Pin_14);
    GPIO_ResetBits(GPIOC, GPIO_Pin_15);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    GPIO_ResetBits(GPIOB, GPIO_Pin_3);
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);
    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
    GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

enum
{
    led_B12_GB13_B14    = 0x00,
    led_B15_GA8_A9      = 0x01,
    led_A10_GA11_A12    = 0x02,
    led_A15_GB3_B4      = 0x03,
    led_B5_GB6_B7       = 0x04,
    
    led_B11_GB10_B1     = 0x05,
    led_B0_GA7_A6       = 0x06,
    led_A5_GA4_A3       = 0x07,
    led_A2_GA1_A0       = 0x08,
    led_C15_GC14_C13    = 0x09,
};
void leds_gpio_contrl(unsigned char gpio_n, unsigned char val)
{
    switch(gpio_n)
    {
        case led_B12_GB13_B14:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_12);
                GPIO_ResetBits(GPIOB, GPIO_Pin_14);
            }
            else if(1 == val)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_12);
                GPIO_ResetBits(GPIOB, GPIO_Pin_14);
            }
            else
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_12);
                GPIO_SetBits(GPIOB, GPIO_Pin_14);
            
            }
            break;
         case led_B15_GA8_A9:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_15);
                GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            }
            else if(1 == val)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_15);
                GPIO_ResetBits(GPIOA, GPIO_Pin_14);
            }
            else
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_15);
                GPIO_SetBits(GPIOA, GPIO_Pin_9);
            
            }
            break;
         case led_A10_GA11_A12:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_10);
                GPIO_ResetBits(GPIOA, GPIO_Pin_12);
            }
            else if(1 == val)
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_10);
                GPIO_ResetBits(GPIOA, GPIO_Pin_12);
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_10);
                GPIO_SetBits(GPIOA, GPIO_Pin_12);
            
            }
            break;
         case led_A15_GB3_B4:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_15);
                GPIO_ResetBits(GPIOB, GPIO_Pin_4);
            }
            else if(1 == val)
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_15);
                GPIO_ResetBits(GPIOB, GPIO_Pin_4);
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_15);
                GPIO_SetBits(GPIOB, GPIO_Pin_4);            
            }
            break;  
            
        case led_B5_GB6_B7:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_5);
                GPIO_ResetBits(GPIOB, GPIO_Pin_7);
            }
            else if(1 == val)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_5);
                GPIO_ResetBits(GPIOB, GPIO_Pin_7);
            }
            else
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_5);
                GPIO_SetBits(GPIOB, GPIO_Pin_7);
            
            }
            break;   
         case led_B11_GB10_B1:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_11);
                GPIO_ResetBits(GPIOB, GPIO_Pin_1);
            }
            else if(1 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_11);
                GPIO_SetBits(GPIOB, GPIO_Pin_1);
            }
            else
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_11);
                GPIO_ResetBits(GPIOB, GPIO_Pin_1);
            
            }
            break;   
         case led_B0_GA7_A6:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                GPIO_ResetBits(GPIOA, GPIO_Pin_6);
            }
            else if(1 == val)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                GPIO_SetBits(GPIOA, GPIO_Pin_6);
            }
            else
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
                GPIO_ResetBits(GPIOA, GPIO_Pin_6);
            
            }
            break;
            
        case led_A5_GA4_A3:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_5);
                GPIO_ResetBits(GPIOA, GPIO_Pin_3);
            }
            else if(1 == val)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_5);
                GPIO_SetBits(GPIOA, GPIO_Pin_3);
            }
            else
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_5);
                GPIO_ResetBits(GPIOA, GPIO_Pin_3);
            
            }
            break;
            
        case led_A2_GA1_A0:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_2);
                GPIO_ResetBits(GPIOA, GPIO_Pin_0);
            }
            else if(1 == val)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_2);
                GPIO_SetBits(GPIOA, GPIO_Pin_0);
            }
            else
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_2);
                GPIO_ResetBits(GPIOA, GPIO_Pin_0);
            
            }
            break;
            
       case led_C15_GC14_C13:
            if(0 == val)
            {
                GPIO_ResetBits(GPIOC, GPIO_Pin_15);
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);
            }
            else if(1 == val)
            {
                GPIO_ResetBits(GPIOC, GPIO_Pin_15);
                GPIO_SetBits(GPIOC, GPIO_Pin_13);
            }
            else
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_15);
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);
            }
            break;
        default:
            break;
    }
}

