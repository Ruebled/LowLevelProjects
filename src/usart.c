#include "stm32f429xx.h"
#include "system_stm32f4xx.h"

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
