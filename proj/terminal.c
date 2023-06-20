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

extern uint8_t *USART1_Buffer_Tx, *USART4_Buffer_Tx;

// Create a variable for clearing minicom settings when sent
char clear[] = {0x1B, 0x5B, 0x32, 0x4A, '\r', '\010', '\000'};
char *input_bar = "+--(user(+_+)stm32)-[~]\n+-$ ";
char *str = "Welcome on this board\n";

int main(void) {
	Clock_Init();

	//------------------------------Initialize SPI5----------------------------------//
	// Enable SPI5 and it's clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI5RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI5RST;

	// Init CS line for SPI5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
	
	// Initialize MISO MOSI SCK lines for SPI5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; // Enable GPIOF clock

	SPI5_Init();
	//------------------------------------------------------------------------------//

	//------------Systick initialize delay counter---------------------------------//
	SysTick_Init(180000);
	//------------------------------------------------------------------------------//

	//-------------------------------Init board leds--------------------------------//
	// Initialize GPIOG for LED 13 and 14
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;	

	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);
	//-----------------------------------------------------------------------------//


	//-----------------------------------------Init USART ports----------------------//
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	

	//GPIO set mode 10 => Alternate function for pin 9 10
	GPIOA->MODER &= ~(0xF << (2*9));
	GPIOA->MODER |= 0xA << (2*9);

	// Alternative function 7 = USART1 PA9
	GPIOA->AFR[1] &= ~(0x770);
	GPIOA->AFR[1] |= (0x770);

	// GPIO Speed 11 for high speed
	GPIOA->OSPEEDR |= 0xF <<(2*9);

	//GPIO push-pull
	GPIOA->PUPDR &= ~(0xF <<(2*9));
	GPIOA->PUPDR |= 0x5 << (2*9);

	// GPIO output type
	GPIOA->OTYPER &= ~(0x3<<9);
	//-------------------------------------------------------------------------------//

	//----------------------------------------Init USART1---------------------------//
	//Initialize USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	// Initialise USART1
	USART_Init(USART1);

	////Interrupt on receive register not empty
	//USART1->CR1 |= USART_CR1_RXNEIE;

	//// Transmit register empty interrupt
	//USART1->CR1 &= ~USART_CR1_TXEIE;

	//// Set highest priority
	//NVIC_SetPriority(USART1_IRQn, 0);

	//// Enable NVIC interrupt
	//NVIC_EnableIRQ(USART1_IRQn);

	//-----------------------------------------------------------------------------//

	//----------------------------Init btn interruption----------------------------//
	btn_enable();
	//-----------------------------------------------------------------------------//

	// Clear console and print welcome message
	usart_send(USART1, clear);
	usart_send(USART1, str);

	Delay(1000);

	// Clear console again
	usart_send(USART1, clear);

	char st[100];

	uint8_t txBuff;
	uint8_t rxBuff[2];

	while(1)
	{
		usart_send(USART1, input_bar);
		usart_get(USART1, st, 8);
		usart_send(USART1, "\r");

		if(!(strcmp(st, "temp"))){
			uint8_t temp_reg = 0x26 |0x80;

			SPI5_REG_SEL(temp_reg, &txBuff);
			print_bits(txBuff, 8);
			usart_send(USART1, "\n");
			
			//Read TEMP
			SPI5_Read(rxBuff, 1);

			//uint16_t temp = 0;

			//temp |= pBuff[0] & 0xFF;
			//temp <<= 8;
			//temp |= pBuff[1] & 0xFF;

			print_bits(rxBuff[0], 8);
			print_bits(rxBuff[1], 8);

			//print_bits(temp, 16);
			usart_send(USART1, "\n");

			// Print temperature
			usart_send(USART1, "Temp:");
			//print_number(temp);

		}else if(!(strcmp(st, "spi"))){
			uint8_t whoami = 0x0F | 0x80;

			usart_send(USART1, "REG_NAME: ");
			print_bits(whoami, 8);
			usart_send(USART1, "\n");

			SPI5_REG_SEL(whoami, &txBuff);
			print_bits(txBuff, 8);
			usart_send(USART1, "\n");

			SPI5_Read(rxBuff, 1);

			if(*rxBuff == 0xD4){
				usart_send(USART1, "The right thing\n");
			}

			print_bits(*rxBuff, 8);
			usart_send(USART1, "\n");

		}else if(!(strcmp(st, "axis"))){
			GYRO_SET_XYZ(0x7);

			uint8_t STATUS_REG = 0x27;

			SPI5_Write(STATUS_REG | 0x80, &txBuff, 1);

			SPI5_Read(rxBuff, 1);
			print_bits(*rxBuff, 8);
			usart_send(USART1, "\n");

			//Read x axis value if rxBuff at least 0x9
			uint8_t OUT_X_L = 0x28 | 0x80;
			uint8_t OUT_X_H = 0x29 | 0x80;

			SPI5_Write(OUT_X_H, &txBuff, 1);
			SPI5_Read(rxBuff, 8);
			print_bits(*rxBuff, 8);

			SPI5_Write(OUT_X_L, &txBuff, 1);
			SPI5_Read(rxBuff, 8);
			print_bits(*rxBuff, 8);

			usart_send(USART1, "\n");

			uint8_t OUT_Y_L = 0x2A | 0x80;
			uint8_t OUT_Y_H = 0x2B | 0x80;

			SPI5_Write(OUT_Y_H, &txBuff, 1);
			SPI5_Read(rxBuff, 8);
			print_bits(*rxBuff, 8);

			SPI5_Write(OUT_Y_L, &txBuff, 1);
			SPI5_Read(rxBuff, 8);
			print_bits(*rxBuff, 8);

			usart_send(USART1, "\n");

			uint8_t OUT_Z_L = 0x2C | 0x80;
			uint8_t OUT_Z_H = 0x2D | 0x80;

			SPI5_Write(OUT_Z_H, &txBuff, 1);
			SPI5_Read(rxBuff, 1);
			print_bits(*rxBuff, 1);

			SPI5_Write(OUT_Z_L, &txBuff, 1);
			SPI5_Read(rxBuff, 8);
			print_bits(*rxBuff, 8);

			usart_send(USART1, "\n");

			// Blink some led pattern at the end			
			GPIOG->BSRR |= 1<<13;
			Delay(500);
			GPIOG->BSRR |= 1<<(13+16);
			Delay(500);
		}else if(!(strcmp(st, "laser"))){
			//GPIOA->ODR ^= 1<<6;
			//Delay(250);
			//GPIOA->ODR ^= 1<<6;
			//Delay(250);
		}else{
			usart_send(USART1, "My bad");

			GPIOG->BSRR |= 1<<13;
			Delay(500);
			GPIOG->BSRR |= 1<<(13+16);
			Delay(500);
		}
		usart_send(USART1, "\n");
	}

	//	GPIOG->BSRR |= 1<<13;
	//	Delay(500);
	//	GPIOG->BSRR |= 1<<(13+16);
	//	Delay(500);
	//	GPIOG->BSRR |= 1<<14;
	//	Delay(500);
	//	GPIOG->BSRR |= 1<<(14+16);
	//	Delay(500);
}

