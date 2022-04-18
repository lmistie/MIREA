//---------------------------------------------------------------------------
// 
//  ондопнцпюллш пюанрш я USART
//
//---------------------------------------------------------------------------

#ifndef _USART_H
#define _USART_H

void USART_Config(void);
bool USART_RX_Flag(void);
uint16_t USART_RX_Data(void);
void USART_TX_Data(char* buffer, uint16_t length);

#endif /* _USART_H */
