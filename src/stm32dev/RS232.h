  


#ifndef __RS232_H
#define __RS232_H

#include <includes.h>

#define  RS232_Receive_BUFSIZE   300

extern u8   RS232_RecBuf[RS232_Receive_BUFSIZE];
extern s16  RS232_Receive_Index;

//------------------------------------------------------
//Declaration of public functions.
//------------------------------------------------------
extern void RS232_Initialise(void);
extern bool RS232_PutChar(u8 *s,u16 length);
extern bool RS232_PutWord(u32 ch,u8 bit);
extern bool RS232_PutHex(u8 *s,u16 length);
extern bool RS232_PutString(char *s);
extern u16  Process_RS232_Receive(void);
extern s16 RS232_RX_Char(void);

extern void USART1_IRQHandler(void);
#endif
