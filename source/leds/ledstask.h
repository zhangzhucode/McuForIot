#ifndef __LED_TASK_H
#define __LED_TASK_H

#include <includes.h>	
#include "leddriver.h"

typedef  struct  {
        uint16_t width;
        uint16_t length;
        uint16_t time;
        unsigned char * buf;
}leds_frame;

extern void LedsTask(void *pd);

#endif

