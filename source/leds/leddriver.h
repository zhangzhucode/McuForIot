#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#include <includes.h>	

extern void leds_10ms_deal(void);

void leds_init(void);
void led_set_gpio(unsigned char *buf,uint16_t width );
void led_set_gpio_off(void);

#endif

