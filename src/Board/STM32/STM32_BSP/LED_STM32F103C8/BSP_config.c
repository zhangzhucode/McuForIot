

#include <includes.h>	

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


//-----------------------------------------------------------
//函数名称: Bsp_Configuration
//功能描述: 系统主板配置函数
//参    数: None
//返 回 值: None 
//备    注：None
//-----------------------------------------------------------

void Bsp_Configuration(void)
{   
   	SystemInit();	

    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    Timer_Configuration();
    ADC_Configuration();
    DMA_Configuration();
    SysTick_Configuration();
    EXTI_Configuration();     
    WWdg_Configuration(); 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	Bsp_RS232_Initialise();
}

/***************************************************************************************
** 函数名称: RCC_Configuration
** 功能描述: 时钟配置函数
** 参    数: None
** 返 回 值: None 
****************************************************************************************/
void RCC_Configuration(void)
{

   u16 cn=0;
  ErrorStatus HSEStartUpStatus;
 
  RCC_DeInit();                                //RCC system reset(for debug purpose)
  RCC_HSEConfig(RCC_HSE_ON);                   // Enable HSE  
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  // Wait till HSE is ready 

  if(HSEStartUpStatus == SUCCESS)
  {
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);     // HCLK = SYSCLK  
    RCC_PCLK2Config(RCC_HCLK_Div1);      // PCLK2 = HCLK 
    RCC_PCLK1Config(RCC_HCLK_Div2);      // PCLK1 = HCLK/2 
  
    FLASH_SetLatency(FLASH_Latency_2);   // Flash 2 wait state    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // Enable Prefetch Buffer 
   
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    RCC_PLLCmd(ENABLE);                  // Enable PLL  
    for(cn=0;cn<20;cn++)
    {
      if(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
      { break;}
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);           // Select PLL as system clock source 
    for(cn=0;cn<20;cn++)
    {
      if(RCC_GetSYSCLKSource() == 0x08)break;
    }
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |RCC_APB1Periph_UART4, ENABLE); 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG , ENABLE);       // Enable WDG clocks 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


}



/***************************************************************************************
** 函数名称: SysTick_Config
** 功能描述: 系统时钟配置函数  ，OS系统运行时钟    10ms中断
** 参    数: None
** 返 回 值: None 
** 日  　期: 2008年11月27日
****************************************************************************************/
void SysTick_Configuration(void)
{
     RCC_ClocksTypeDef  rcc_clocks;
    INT32U      cnts;
    
    RCC_GetClocksFreq(&rcc_clocks);                              //得到RCC时钟    
    cnts = (INT32U)rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC; //设置计数值   72M/100    
    
    SysTick_Config(cnts);
    //SysTick_SetReload(cnts);                                     //将cnts设置成为重装值
    //SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);             //配置系统时钟源为HCLK
    //SysTick_CounterCmd(SysTick_Counter_Enable);                  //计数使能
                                           
    //SysTick_ITConfig(ENABLE);                                    //使能中断    
}

/***************************************************************************************
** 函数名称: GPIO_Configuration
** 功能描述: I/O配置函数
** 参    数: None
** 返 回 值: None 
** 日  　期: 2008年11月27日
****************************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_AFIODeInit();
   
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
 
}

/***************************************************************************************
** 函数名称: GPIO_Configuration
** 功能描述: 中断配置函数
** 参    数: None
** 返 回 值: None 
** 日  　期: 2008年11月27日
****************************************************************************************/
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef   NVIC_InitStructure;
	
	#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif 
	
NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************
** 函数名称: Timer_Configuration
** 功能描述: 中断配置函数
** 参    数: None
** 返 回 值: None 
** 日  　期: 2008年11月27日
****************************************************************************************/
void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStruct;

   TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
   TIM_TimeBaseInitStruct.TIM_Period =6000;           //TIM update rate =TIM counter clock/TIM_Period =6MHz/6000=1000Hz   
   TIM_TimeBaseInitStruct.TIM_Prescaler =11;           //TIM counter clock =TIMxCLK/(Prescaler +1) =72MHz/12=6MHz       
   TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;       //时钟倍频  
   TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);       
   
   //TIM_Cmd(TIM2, ENABLE); 
                                          
   TIM_ClearFlag(TIM2, TIM_FLAG_Update);
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);           // Enable TIM2 Update interrupt [TIM2溢出中断允许]
   TIM_Cmd(TIM2, ENABLE);                               // TIM2 enable counter [允许tim2计数]

}


void ADC_Configuration(void)
{
   
} 


void DMA_Configuration(void)
{
   
}

 

 /***************************************************************************************
** 函数名称: EXTI_Configuration
** 功能描述: 外部唤醒中断配置函数
** 参    数: None
** 返 回 值: None 
** 日  　期: 2008年11月27日
****************************************************************************************/
 void EXTI_Configuration(void )
{
 
  
} 
/***************************************************************************************
** 函数名称: Wdg_Init
** 功能描述: 看门狗初始化函数
** 参    数: None
** 返 回 值: None   
****************************************************************************************/
void WWdg_Configuration(void)
{

}
//*******************************************************************************
// 函数名称: RCC_GetFlagStatus
// 功能描述: 检查指定的RCC标志设置与否.
// 输入参数: RCC_FLAG:要检查的标志.
//                    这个参数可以是下面的值之一:
//                       - RCC_FLAG_HSIRDY: HIS振荡时钟就绪
//                       - RCC_FLAG_HSERDY: HSE振荡时钟就绪
//                       - RCC_FLAG_PLLRDY: PLL时钟就绪
//                       - RCC_FLAG_LSERDY: LSE振荡时钟就绪
//                       - RCC_FLAG_LSIRDY: LSI振荡时钟就绪
//                       - RCC_FLAG_PINRST: 引脚复位
//                       - RCC_FLAG_PORRST: POR/PDR复位
//                       - RCC_FLAG_SFTRST: 软件复位
//                       - RCC_FLAG_IWDGRST: 独立看门狗复位
//                       - RCC_FLAG_WWDGRST: 窗口看门狗复位
//                      - RCC_FLAG_LPWRRST: 低电量复位
// 输出参数: 无
// 返回参数: RCC_FLAG的新状态(SET或RESET).
//*******************************************************************************
//FlagStatus RCC_GetFlagStatus(u8 RCC_FLAG)
//*******************************************************************************
//* 函数名称: RCC_ClearFlag
//* 功能描述: 清除RCC复位标志.
//*           复位标志为: RCC_FLAG_PINRST, RCC_FLAG_PORRST,
//*                  RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST,
//*                  RCC_FLAG_LPWRRST
//* 输入参数: 无
//* 输出参数: 无
//* 返回参数: 无
//*******************************************************************************/
//读取复位值
void Fetch_Reset_Cause(void)
{

}
void Bsp_RS232_Initialise(void)
{
	 RS232_Initialise();
}

