#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "systick.h"
#include "gpio.h"
#include "usart.h"

int main(void) {
	SystemInit();
	SysTick_Init(16000);

	/* GPIO set */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;	

	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);

	USART_Init(USART1);

	uint16_t c = 'a';

	while (1){
		GPIOG->ODR ^= 1<<13;

		Delay(1000);

		while (!(USART1->SR & USART_SR_TXE_Msk));

		USART1->DR = (c & 0x1FF);
	}
}

