#include "stm32f429xx.h"

#define PLL_M 4
#define PLL_N 180
#define PLL_P 0 

void Clock_Init(){
	// 1.Enable HSE and wait for the HSE to become Ready
	RCC->CR |= 1<<16; //HSE ON
	while (!(RCC->CR & (1<<17))); // Wait till HSE RDY

	// 2.Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= 1<<28; // Power interface clock enable
	PWR->CR |= 3<<14; // Regulator voltage scaling output selection

	// 3.Configure the FLASH PREFETCH and the LATENCY Related Setting
	FLASH->ACR = (1<<10) | (1<<9) | (1<<8) | (5<<0);

	// 4.Configure the PRESCALARS HCLK, PCLK1, PCLK2
	RCC->CFGR &= ~(0xF<<4); // Set AHB Prescaler to 1 (all 0)

	RCC->CFGR &= ~(7<<10); // Clear APB1 Prescaler bits
	RCC->CFGR |= (5<<10); // Set APB1 Prescaler to 4 (101)

	RCC->CFGR &= ~(11<<13); // Clear APB2 Prescaler
	RCC->CFGR |= (4<<13); // Set APB2 Prescaler to 2 (100)

	// 5.Configure the MAIN PLL
	RCC->PLLCFGR |= (1<<22); //Set PLLSRC; HSE oscilator clock 

	RCC->PLLCFGR &= ~(3<<16); //Set PLLP

	RCC->PLLCFGR &= ~(0x1FF<<6);
	RCC->PLLCFGR |= (PLL_N<<6); //Set PLLN	

	RCC->PLLCFGR &= ~(0x3F<<0); // Clear PLLM
	RCC->PLLCFGR |= (PLL_M<<0); // Set PLLM

	// 6.Enable the PLL and wait for it to become ready
	RCC->CR |= (1<<24); // Enable PLL
	while(!(RCC->CR & (1<<25))); // Wait till PLL is ready

	// 7.Select the Clock Source and wait for it to be set
	RCC->CFGR &= ~(3<<0); // Clear SW reg bits
	RCC->CFGR |= (2<<0); // Set PLL as system clock

	while(!(RCC->CFGR & (2<<2))); // Wait till PLL is used as system clock
}

void Periph_Clock_Init(){
	// Enable SPI5 and it's clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	//RCC->APB2RSTR |= RCC_APB2RSTR_SPI5RST;
	//RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI5RST;

	// Init CS line for SPI5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
	
	// Initialize MISO MOSI SCK lines for SPI5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; // Enable GPIOF clock

	// Initialize GPIOG for LED 13 and 14
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;	

	// Initialize USART clocks needed GPIOA and USART1	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	
}
