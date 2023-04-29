#include "stm32f429xx.h"
#include "system_stm32f4xx.h"

void GPIO_Init (GPIO_TypeDef * GPIO, uint16_t pin){
	/* GPIO set */
	GPIO->MODER &= ~(3UL<<(pin*2));

	GPIO->MODER |= 1UL<<(pin*2);

	GPIO->OTYPER &= ~(1<<(pin));
	
	GPIO->OSPEEDR |= (2UL<<(pin*2));

	GPIO->PUPDR &= ~(3UL<<(pin*2));
}
