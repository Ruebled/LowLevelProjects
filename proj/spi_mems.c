#include <stdlib.h>
#include <string.h>

#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "systick.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
#include "clock.h"
#include "gyro.h"

int main(){
	Clock_Init();
	Periph_Clock_Init();

	SysTick_Init(180000);

	GYRO_Init();

	// Init led 13 14 as output
	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);

	USART_Init(USART1);

	while(1){
		Delay(1000);
		GPIOG->ODR ^= 1<<13;
	}
}
