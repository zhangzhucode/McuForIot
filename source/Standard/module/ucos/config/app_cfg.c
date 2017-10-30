

#include <includes.h>

INT32U  OS_CPU_SysTickClkFreq (void)
{     
    RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    return ((INT32U)rcc_clocks.HCLK_Frequency);
}
