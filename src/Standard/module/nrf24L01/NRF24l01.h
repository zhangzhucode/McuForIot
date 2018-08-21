#ifndef _NRF24L01_H 
#define _NRF24L01_H

#define NRF24_TX_Mode  TX_Mode
#define NRF24_RX_Mode  RX_Mode

void init_NRF24L01(void);
void TX_Mode(void);
void RX_Mode(void);

void NRF_Send(void);
void NRF_Receive(void);

#endif

