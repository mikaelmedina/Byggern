#ifndef UART_H
#define UART_H

void UART_init(void);
void UART_Transmit(unsigned char data);
unsigned char UART_Recieve(void);
#endif