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
#include "exti.h"

int main(){
	/// Test script
	Clock_Init();

	SysTick_Init(180000);

	// Initialize GPIOG for LED 13 and 14
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;	

	// Init led 13 14 as output
	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);

	btn_enable();

	while(1){
		///if((GPIOB->IDR & 0x1) == 1){
		///	GPIOG->ODR ^= 1<<13;
		///	Delay(500);
		///}
			GPIOG->ODR ^= 1<<13;
			Delay(500);
	}
}
