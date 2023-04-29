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


int main(void) {
	Clock_Init();
	Periph_Clock_Init();

	SysTick_Init(180000);

	GYRO_Init();

	// Init led 13 14 as output
	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);

	USART_Init(USART1);


	// Init EXIT0 interrupt on PA0 User button
	//RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//GPIOA->MODER &= ~(0x1);
	//GPIOA->OTYPER &= ~(0x1);
	//GPIOA->OSPEEDR |= 2;
	//GPIOA->PUPDR |= 1;

	//SYSCFG->EXTICR[0] &= ~(0xF);

	//EXTI->RTSR |= 0x1;

	////EXTI_FTSR |= 0x1;
	//
	//EXTI->IMR |= 0x1; // Set interrupt mask register

	//NVIC_SetPriority(EXTI0_IRQn, 1);

	//NVIC_EnableIRQ(EXTI0_IRQn); // Enable interrupt in button


	// Create a variable for clearing minicom settings when sent
	char clear[] = {0x1B, 0x5B, 0x32, 0x4A, '\r', '\010', '\000'};
	usart_send(USART1, clear);

	char *str = "Welcome on this board\n";
	char *input_bar = "+--(user(+_+)stm32)-[~]\n+-$ ";

	usart_send(USART1, str);

	Delay(1000);

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
			uint8_t temp_reg = 0xE6;

			SPI5_Write(temp_reg, &txBuff, 1);

			//Read TEMP
			SPI5_Write(temp_reg, rxBuff, 2);
			//SPI5_Read(pBuff, 2);

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

			SPI5_Write(whoami, &txBuff, 1);

			SPI5_Write(whoami, rxBuff, 1);
			//SPI5_Read(&rxBuff, 1);

			if(*rxBuff == 0xD4){
				usart_send(USART1, "The right thing\n");
			}

			print_bits(*rxBuff, 8);
			usart_send(USART1, "\n");

		}else if(!(strcmp(st, "clear"))){

			usart_send(USART1, clear);

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
			GPIOG->ODR ^= 1<<13;
			Delay(250);
			GPIOG->ODR ^= 1<<13;
			Delay(250);
			GPIOG->ODR ^= 1<<13;
			Delay(250);
			GPIOG->ODR ^= 1<<13;
		}else{
			usart_send(USART1, "My bad");
			GPIOG->ODR ^= 1<<13;
			GPIOG->ODR ^= 1<<14;
			Delay(250);
			GPIOG->ODR ^= 1<<13;
			GPIOG->ODR ^= 1<<14;
		}
		usart_send(USART1, "\n");
	}

	//while (1){
	//	GPIOG->ODR ^= 1<<14;
	//	Delay(1000);
	//	GPIOG->ODR ^= 1<<14;	
	//	Delay(1000);
	//}
}

//void EXTI0_Handler(void){
//	if((EXTI->PR & 1) == 1){
//		GPIOG->ODR ^= 1<<13;
//
//		EXTI->PR |= 0x1;
//	}
//}
