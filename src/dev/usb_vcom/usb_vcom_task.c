
#include "stm32f10x.h"	

#include "vcom_hw_config.h"

void USB_VcomTask(void)
{
 
    uint32_t len = 0;
    static uint8_t buf[200] = {0};
    

    USB_Config();
    while (1)
    {

        len = USB_RxRead(buf, sizeof(buf));
        if (len > 0)
        {
            USB_TxWrite(buf, len);
        }
    }
}
#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
