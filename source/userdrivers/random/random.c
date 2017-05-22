#include "random.h"


unsigned char random_count=0;

void random(void)
{
    random_count++;
    if(random_count>=30)random_count = 0;
}

unsigned char get_random(void)
{
    return random_count;
}
