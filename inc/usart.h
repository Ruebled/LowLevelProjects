#ifndef USART_H
#define USART_H
#include "stm32f429xx.h"

#include <stdint.h>

void USART_Init (USART_TypeDef * USARTx);
void usart_send (USART_TypeDef * USARTx, char * s);
void usart_send_char (USART_TypeDef * USARTx, char ch);
char usart_get_char (USART_TypeDef * USARTx);
void usart_get (USART_TypeDef * USARTx, char *s, int len);

void print_bits(int byte, int cnt);
void print_number(uint16_t number);

#endif
