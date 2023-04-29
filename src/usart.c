#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "usart.h"
#include <stdlib.h>

void USART_Init (USART_TypeDef * USARTx){
	//GPIO set mode 10 => Alternate function for pin 9 10
	GPIOA->MODER &= ~(0xF << (2*9));
	GPIOA->MODER |= 0xA << (2*9);

	// Alternative function 7 = USART1 PA9
	GPIOA->AFR[1] &= ~(0x770);
	GPIOA->AFR[1] |= (0x770);

	// GPIO Speed 11 for high speed
	GPIOA->OSPEEDR |= 0xF <<(2*9);

	//GPIO push-pull
	GPIOA->PUPDR &= ~(0xF <<(2*9));
	GPIOA->PUPDR |= 0x5 << (2*9);

	// GPIO output type
	GPIOA->OTYPER &= ~(0x3<<9);


	//Initialize USART1
	USARTx->CR1 &= USART_CR1_UE;

	USARTx->CR1 |= (1<<15); // Oversampling by 8

	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE) ;
	
	USARTx->CR2 &= ~(3UL<<12);

	USARTx->CR3 = 0;

	uint32_t clock = 180000000;

	USARTx->BRR = (clock / 9600);

	USARTx->CR1 |= USART_CR1_UE;
}

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
