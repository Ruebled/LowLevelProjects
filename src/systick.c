#include "stm32f429xx.h"
#include "system_stm32f4xx.h"

volatile uint32_t TimeDelay;

void SysTick_Init(uint32_t ticks){
	// Disable Systick IRQ and SysTick counter
	SysTick->CTRL = 0;

	// Set reload register
	SysTick->LOAD = ticks - 1;

	// Set interrupt priority of SysTick
	// Make SysTick least urgent(with the highest priority number i.e. 15)
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

	// Reset SysTick counter value
	SysTick->VAL = 0;

	// Select processor clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// Enable SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void){
	// TimeDelay is a global variable declared as volatile
	if(TimeDelay > 0)
		TimeDelay--;
}

void Delay(uint32_t nTime) {
	// nTime: specifies the delay time length
	TimeDelay = nTime;
	while(TimeDelay != 0);
}

