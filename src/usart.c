#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "usart.h"
#include <stdlib.h>

#define BufferSize 32

//uint8_t USART_Buffer_Rx[BufferSize], USART4_Buffer_Rx[BufferSize];
//volatile uint32_t Rx1_Counter = 0, Rx4_Counter = 0;
char* USART1_Buffer_Tx, *USART4_Buffer_Tx;
volatile uint32_t Tx1_Counter = 0, Tx4_Counter = 0;

void USART_Init (USART_TypeDef * USARTx){
	USARTx->CR1 |= (1<<15); // Oversampling by 8

	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	USARTx->CR2 &= ~(3UL<<12);

	USARTx->CR3 = 0;

	uint32_t clock = 180000000;

	USARTx->BRR = (clock / 9600);

	USARTx->CR1 |= USART_CR1_UE;
}

//void USART1_IRQHandler(void){
//	receive(USART1, USART1_Buffer_Rx, &Rx1_Counter);
//}
//
//
//void receive(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t *pCounter){
//	if(USARTx->ISR & USART_SR_RXNE){
//		buffer[*pCounter] = USARTx->DR;
//		(*pCounter)++;
//		if(buffer[*pCounter] == '\0'){
//			(*pCounter) = 0;
//			USARTx->SR &= ~ USART_SR_RXNE;
//			return;
//		}
//	}
//}
//
//void UART_Send(USART_TypeDef *USARTx, char *buffer){
//	USARTx->CR1 |= USART_CR1_TXEIE;
//
//	USART1_Buffer_Tx = buffer;
//	USARTx->DR = buffer[0];
//}
//
//void USART1_IRQHandler(void){
//	send(USART1, USART1_Buffer_Tx);
//}
//
//void send(USART_TypeDef *USARTx, char * buffer){
//	usart_send(USARTx, buffer);
//	if(USARTx->SR & USART_SR_TXE){
//		usart_send(USARTx, buffer);
//		USARTx->CR1 &= ~USART_CR1_TXEIE;
//	}
//}


// Sequential adressing outdated
void usart_send(USART_TypeDef * USARTx, char *s)
{
	while(*s != '\0')
	{
		usart_send_char(USARTx, *s);

		if(*s == '\n')
		{
			usart_send_char(USARTx, '\r');
		}
		s++;
	}
}

void usart_send_char(USART_TypeDef * USARTx, char ch)
{
	while(!(USARTx->SR & USART_SR_TXE_Msk));

	USARTx->DR = ch & USART_DR_DR_Msk;
}

void usart_get(USART_TypeDef * USARTx, char* s, int len){
	char *t = s;
	char c;
	int cnt = 0;

	while((c = usart_get_char(USARTx))!= '\r'){
		if(c == '\010' && t>s){
			usart_send(USARTx, "\010 \010");
			*t = '\0';
			t--;
		}else{
			//solve this shit
			if((t-s) >= (len-1)){
				break;
			}
			usart_send_char(USARTx, c);
			*t = c;
			t++;
			cnt++;
		}
	}
	usart_send_char(USARTx, '\n');
	*t = '\0';
}

char usart_get_char(USART_TypeDef * USARTx){
	while(!(USARTx->SR & USART_SR_RXNE_Msk)) {}
	return USARTx->DR;
}

void print_bits(int byte, int cnt){
	int bit = 0;
	char ch_bit = '\0';
	for(int i=cnt-1; i>=0; i--)
	{
		bit = (byte&(0x1<<i))&&1;
		ch_bit = (bit==1?'1':'0');
		usart_send_char(USART1, ch_bit);
	}
}

void print_number(uint16_t number){
	
}

