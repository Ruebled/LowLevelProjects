#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "spi.h"
#include "gpio.h"

void GYRO_Init(){
	/* GPIO set */
	GPIOF->MODER &= ~(0x3F<<7*2); // Clear Reg value for 7, 8, 9 pins
	GPIOF->MODER |= (0x2A<<7*2); // Set Reg values to 2(alternate func)

	GPIOF->OTYPER &= ~(0x7<<7); // Set output type of I/O as push-pull

	GPIOF->OSPEEDR |= (0x2A<<7*2); // Set output speed of I/O as HS

	GPIOF->PUPDR &= ~(0x3F<<7*2); // Set SCK and MOSI to No pull-up, pull-down
	GPIOF->PUPDR |= (1U<<8*2); // Set MISO to PULL-UP

	GPIOF->AFR[0] |= (0x5<<7*4); // Set up pin 7 to AF5
	GPIOF->AFR[1] |= 0x55; // Set up pin 8, 9 to AF5 
	// END of MISO MOSI SCK GPIO initialize

	
	// CS GPIO init end
	GPIOC->MODER &= ~(3<<2);
	GPIOC->MODER |= 2<<2; // Set CS line in output mode

	GPIOF->OTYPER &= ~(1<<1); // Set output type CS line as push-pull
	
	GPIOC->OSPEEDR |= 2<<2; // Set CS line speed as HS

	GPIOC->PUPDR &= ~(3<<2); // Clear CS line PUSH_UP PUSH_DOWN register NO PULL UP NO PULL DOWN

	GPIOC->AFR[0] |= 0x5<<4;
	//GPIOC->PUPDR |= 1<<2; // Set CS line as pull-up
	GPIOC->ODR |= (1<<1); //pulling CS line high

	// INT1 & INT2 think about
	//GPIO_Init(GPIOA, 1);
	//GPIO_Init(GPIOA, 2);

	SPI5_Init(); // SPI Initialize
}

void GYRO_SET_XYZ(uint8_t axis){
	// Set REG1
	uint8_t CTRL_REG1 = 0x20;	

	uint8_t *buff = 0x0;

	SPI5_Write(CTRL_REG1, buff, 1);

	uint8_t REG1_SET = 0x8 | axis;

	SPI5_Write(REG1_SET, buff, 1);

	// Set REG4
	uint8_t CTRL_REG4 = 0x23;	
	
	SPI5_Write(CTRL_REG4, buff, 1);

	uint8_t REG4_SET = 0x40;

	SPI5_Write(REG4_SET, buff, 1);
}
