#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "systick.h"
#include "usart.h"

extern char* clear;
extern char* input_bar;

void btn_enable(void){
	// Init EXIT0 interrupt on PA0 User button
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~(0x3UL);
	GPIOA->PUPDR &= ~(0x3UL);

	//Set interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	SYSCFG->EXTICR[0] &= ~(0x000F);

	EXTI->RTSR |= EXTI_RTSR_TR0;

	EXTI->FTSR |= EXTI_FTSR_TR0;

	EXTI->IMR |= EXTI_IMR_IM0; // Set interrupt mask register

	NVIC_SetPriority(EXTI0_IRQn, 1);

	NVIC_EnableIRQ(EXTI0_IRQn); // Enable interrupt in button
}

void EXTI0_Handler(void){
	if((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0){
		//check if USART1 enable	

		//if(USART1->CR1 && USART_CR1_UE == USART_CR1_UE){
		//	//Send clear command
		//	usart_send(USART1, clear);
		//	usart_send(USART1, input_bar);
		//}

		GPIOG->ODR ^= 1<<14;

		//Delay(500);

		EXTI->PR |= EXTI_PR_PR0;
	}
}
