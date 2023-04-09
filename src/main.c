#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "systick.h"
#include "gpio.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>

int main(void) {
	SystemInit();
	SysTick_Init(16000);

	/* GPIO set */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;	

	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);
	
	GPIO_L3GD20_Init(GPIOxx, pin , af_num...<<--edit

	USART_Init(USART1);

	char clear[6];
	clear[0] = 0x1B;
	clear[1] = 0x5B;
	clear[2] = 0x32;
	clear[3] = 0x4A;
	clear[4] = '\r';
	clear[5] = '\010';

	usart_send(USART1, clear);
		
	char *str = "Welcome on this board\n";

	usart_send(USART1, str);

	Delay(3000);
	
	usart_send(USART1, clear);

	char *input_bar = "+--(user(+_+)stm32)-[~]\n+-$ ";

	char st[100];

	while(1)
	{
		usart_send(USART1, input_bar);
		usart_get(USART1, st, 99);
		usart_send(USART1, "\r");

		if(!(strcmp(st, "temp"))){
			usart_send(USART1, "Yeap");
			GPIOG->ODR ^= 1<<13;
			Delay(200);
			GPIOG->ODR ^= 1<<13;
		}else if(!(strcmp(st, "spi"))){
			usart_send(USART1, "Not today");
			GPIOG->ODR ^= 1<<14;
			Delay(200);
			GPIOG->ODR ^= 1<<14;
		}else if(!(strcmp(st, "clear"))){
			usart_send(USART1, clear);
<<<<<<< HEAD
			usart_send(USART1, clear);
=======
>>>>>>> 49e7f74 (Some initial tweaks)
		}else{
			usart_send(USART1, "My bad");
			GPIOG->ODR ^= 1<<13;
			GPIOG->ODR ^= 1<<14;
			Delay(250);
			GPIOG->ODR ^= 1<<13;
			GPIOG->ODR ^= 1<<14;
		}
		usart_send(USART1, "\n\r");
	}

	while (1){
<<<<<<< HEAD
		GPIOG->ODR ^= 1<<13;

		Delay(1000);
=======
		__asm__("nop");
>>>>>>> 49e7f74 (Some initial tweaks)
	}
}

