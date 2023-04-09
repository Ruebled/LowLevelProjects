#include "stm32f429xx.h"
#include "system_stm32f4xx.h"

void GPIO_Init (GPIO_TypeDef * GPIO, uint16_t pin){
	/* GPIO set */
	GPIO->MODER &= ~(3UL<<(pin*2));

	GPIO->MODER |= 1UL<<(pin*2);

	GPIO->OTYPER &= ~(1<<(pin*2));
	
	GPIO->OSPEEDR &= ~(3UL<<(pin*2));

	GPIO->PUPDR &= ~(3UL<<(pin*2));
}

void GPIO_L3GD20_Init(GPIO_TypeDef * GPIO, uint16_t pin, uint8_t af){
	/* GPIO set */
	GPIO->MODER &= ~(3U<<(pin*2));
	GPIO->MODER |= 2U<<(pin*2);

	if(af<8){
		GPIO->AFR[0] |= af<<pin;
	}else{
		GPIO->AFR[1] |= af<<(pin-8);
	}
	
	GPIO->OTYPER &= ~(1<<(pin*2));
	
	GPIO->OSPEEDR &= ~(3UL<<(pin*2));

	GPIO->PUPDR &= ~(3UL<<(pin*2));
}
