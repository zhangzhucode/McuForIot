/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_pwr.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Connection/disconnection & power management
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usb_lib.h"
#include "mouse_usb_conf.h"
#include "mouse_usb_pwr.h"
#include "mouse_hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Mouse_bDeviceState = UNCONNECTED; /* USB device status */
__IO FunctionalState Mouse_fSuspendEnabled = ENABLE;  /* true when suspend is possible */

struct
{
  __IO RESUME_STATE eState;
  __IO uint8_t bESOFcnt;
}
Mouse_ResumeS;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : PowerOn
* Description    :
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
RESULT PowerOn(void)
{
  u16 wRegVal;

  /*** cable plugged-in ? ***/
  Mouse_USB_Cable_Config(ENABLE);

  /*** CNTR_PWDN = 0 ***/
  wRegVal = CNTR_FRES;  /* Force USB RESet */
  _SetCNTR(wRegVal);

  /*** CNTR_FRES = 0 ***/
  wInterrupt_Mask = 0;
  _SetCNTR(wInterrupt_Mask); /* Remove Force USB RESet */
  /*** Clear pending interrupts ***/
  _SetISTR(0);
  /*** Set interrupt mask ***/
  wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM; /* USB¸´Î»ÖÐ¶ÏÆÁ±ÎÎ»  ¹ÒÆðÖÐ¶ÏÆÁ±ÎÎ» »½ÐÑÖÐ¶ÏÆÁ±ÎÎ»*/
  _SetCNTR(wInterrupt_Mask);

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : PowerOff
* Description    : handles switch-off conditions
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
RESULT PowerOff()
{
  /* disable all ints and force USB reset */
  _SetCNTR(CNTR_FRES);
  /* clear interrupt status register */
  _SetISTR(0);
  /* Disable the Pull-Up*/
  Mouse_USB_Cable_Config(DISABLE);
  /* switch-off device */
  _SetCNTR(CNTR_FRES + CNTR_PDWN);
  /* sw variables reset */
  /* ... */

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Suspend
* Description    : sets suspend mode operating conditions
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
void Suspend(void)
{
  u16 wCNTR;
  /* suspend preparation */
  /* ... */

  /* macrocell enters suspend mode */
  wCNTR = _GetCNTR();
  wCNTR |= CNTR_FSUSP;
  _SetCNTR(wCNTR);

  /* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
  /* power reduction */
  /* ... on connected devices */


  /* force low-power mode in the macrocell */
  wCNTR = _GetCNTR();
  wCNTR |= CNTR_LPMODE;
  _SetCNTR(wCNTR);

  /* switch-off the clocks */
  /* ... */
  Mouse_Enter_LowPowerMode();

}

/*******************************************************************************
* Function Name  : Mouse_Resume_Init
* Description    : Handles wake-up restoring normal operations
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
void Mouse_Resume_Init(void)
{
  u16 wCNTR;

  /* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
  /* restart the clocks */
  /* ...  */

  /* CNTR_LPMODE = 0 */
  wCNTR = _GetCNTR();
  wCNTR &= (~CNTR_LPMODE);
  _SetCNTR(wCNTR);

  /* restore full power */
  /* ... on connected devices */
  Mouse_Leave_LowPowerMode();

  /* reset FSUSP bit */
  _SetCNTR(IMR_MSK);

  /* reverse suspend preparation */
  /* ... */ 

}

/*******************************************************************************
* Function Name  : Resume
* Description    : This is the state machine handling resume operations and
*                 timing sequence. The control is based on the Resume structure
*                 variables and on the ESOF interrupt calling this subroutine
*                 without changing machine state.
* Input          : a state machine value (RESUME_STATE)
*                  RESUME_ESOF doesn't change Mouse_ResumeS.eState allowing
*                  decrementing of the ESOF counter in different states.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Resume(RESUME_STATE eResumeSetVal)
{
  u16 wCNTR;

  if (eResumeSetVal != RESUME_ESOF)
    Mouse_ResumeS.eState = eResumeSetVal;

  switch (Mouse_ResumeS.eState)
  {
    case RESUME_EXTERNAL:
      Mouse_Resume_Init();
      Mouse_ResumeS.eState = RESUME_OFF;
      break;
    case RESUME_INTERNAL:
      Mouse_Resume_Init();
      Mouse_ResumeS.eState = RESUME_START;
      break;
    case RESUME_LATER:
      Mouse_ResumeS.bESOFcnt = 2;
      Mouse_ResumeS.eState = RESUME_WAIT;
      break;
    case RESUME_WAIT:
      Mouse_ResumeS.bESOFcnt--;
      if (Mouse_ResumeS.bESOFcnt == 0)
        Mouse_ResumeS.eState = RESUME_START;
      break;
    case RESUME_START:
      wCNTR = _GetCNTR();
      wCNTR |= CNTR_RESUME;
      _SetCNTR(wCNTR);
      Mouse_ResumeS.eState = RESUME_ON;
      Mouse_ResumeS.bESOFcnt = 10;
      break;
    case RESUME_ON:
      Mouse_ResumeS.bESOFcnt--;
      if (Mouse_ResumeS.bESOFcnt == 0)
      {
        wCNTR = _GetCNTR();
        wCNTR &= (~CNTR_RESUME);
        _SetCNTR(wCNTR);
        Mouse_ResumeS.eState = RESUME_OFF;
      }
      break;
    case RESUME_OFF:
    case RESUME_ESOF:
    default:
      Mouse_ResumeS.eState = RESUME_OFF;
      break;
  }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
