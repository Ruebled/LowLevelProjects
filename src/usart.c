#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "usart.h"
#include <stdlib.h>

void USART_Init (USART_TypeDef * USARTx){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	

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
	USARTx->CR1 |= USART_CR1_UE;

	USARTx->CR2 &= ~(3UL<<12);

	USARTx->CR3 = 0;

	uint32_t clock = 16000000;

	USARTx->BRR = ((clock + 9600/2)/9600);

	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE) ;
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

	*t = '\0';
	while((c = usart_get_char(USARTx))!= '\r'){
		*t = c;

		usart_send_char(USARTx, c);
		if((t-s) < len)
			t++;
	}
	usart_send_char(USARTx, '\n');
	*t = '\0';
}

char usart_get_char(USART_TypeDef * USARTx){
	while(!(USARTx->SR & USART_SR_RXNE_Msk)) {}
	return USARTx->DR;
}
