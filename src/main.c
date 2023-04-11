#include <stdlib.h>
#include <string.h>

#include "stm32f429xx.h"
#include "system_stm32f4xx.h"
#include "systick.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
#include "clock.h"


int main(void) {
	Clock_Init();

	SysTick_Init(180000);

	/* GPIO set */
	// Initialize MISO MOSI SCK lines for SPI5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; // Enable GPIOF clock

	GPIOF->MODER &= ~(0x3F<<7*2); // Clear Reg value for 7, 8, 9 pins
	GPIOF->MODER |= (0x2A<<7*2); // Set Reg values to 2(alternate func)

	GPIOF->OTYPER &= ~(0x7<<7); // Set output type of I/O as push-pull

	GPIOF->OSPEEDR |= (0x2A<<7); // Set output speed of I/O as HS

	GPIOF->PUPDR &= ~(0x3F<<7); // Set SCK and MOSI to No pull-up, pull-down
	GPIOF->PUPDR |= (1U<<8); // Set MISO to PULL-UP

	GPIOF->AFR[0] |= (0x5<<7); // Set up pin 7 to AF5
	GPIOF->AFR[1] |= (0x55<<0); // Set up pin 8, 9 to AF5 
	// END of MISO MOSI SCK GPIO initialize

	// Init CS line for SPI5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
	
	GPIOC->MODER = (1<<1); // Set CS line in output mode
	
	GPIOC->OSPEEDR |= (0x2<<1); // Set CS line speed as HS

	GPIOF->OTYPER &= ~(1<<1); // Set output type CS line as push-pull

	GPIOC->PUPDR &= ~(3<<1); // Clear CS line PUSH_UP PUSH_DOWN register 
	GPIOC->PUPDR |= 1<<1; // Set CS line as pull-up
	// CS GPIO init end
	GPIOC->ODR |= (1<<1); //pulling CS line high

	// Init led 13 14 as output
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;	
	GPIO_Init(GPIOG, 13);
	GPIO_Init(GPIOG, 14);

	// INT1 & INT2
	//GPIO_Init(GPIOA, 1);
	//GPIO_Init(GPIOA, 2);

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	
	USART_Init(USART1);

	// Enable SPI5 and it's clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI5RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI5RST;

	SPI5_Init(); // SPI Initialize

	// Init EXIT0 interrupt on PA0 User button
//	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
//
//	GPIOA->MODER &= ~(0x1);
//	GPIOA->OTYPER &= ~(0x1);
//	GPIOA->OSPEEDR |= 2;
//	GPIOA->PUPDR |= 1;
//
//	SYSCFG->EXTICR[1] &= ~(0x1);
//
//	EXTI->RTSR |= 0x1;
//
//	//EXTI_FTSR |= 0x1;
//	
//	EXTI->IMR |= 0x1; // Set interrupt mask register
//
//	NVIC_EnableIRQ(EXTI0_IRQn);


	// Create a variable for clearing minicom settings when sent
	char clear[6];
	clear[0] = 0x1B;
	clear[1] = 0x5B;
	clear[2] = 0x32;
	clear[3] = 0x4A;
	clear[4] = '\r';
	clear[5] = '\010';

	usart_send(USART1, clear);
		
	char *str = "Welcome on this board\n";
	char *input_bar = "+--(user(+_+)stm32)-[~]\n+-$ ";

	usart_send(USART1, str);

	Delay(1000);
	
	usart_send(USART1, clear);

	char st[100];

	while(1)
	{
		usart_send(USART1, input_bar);
		usart_get(USART1, st, 8);
		usart_send(USART1, "\r");

		if(!(strcmp(st, "temp"))){
			usart_send(USART1, "Yeap");
			GPIOG->ODR ^= 1<<13;
			Delay(200);
			GPIOG->ODR ^= 1<<13;
		}else if(!(strcmp(st, "spi"))){
			//usart_send(USART1, "Not today");
			// Enable SPI by setting CS line low
			
			//some shit code
			uint8_t whoami = 0xF | 0x80;

			uint8_t rxBuff = 1;
			uint8_t pBuff = 1;

			GPIOC->ODR &= ~(1<<1);
			Delay(10);

			SPI5_Write(whoami, &rxBuff);

			SPI5_Read(&pBuff);

			GPIOC->ODR |= (1<<1); //pulling CS line high

			// Write register needed value
			if(rxBuff == 0xD4){
				usart_send(USART1, "The right thing\n");
			}
			for(int i=8; i>=0; i--)
			{
				int bit = (rxBuff&(0x1<<i))&&1;
				char ch_bit = (bit==1?'1':'0');
				usart_send_char(USART1, ch_bit);
			}
			usart_send_char(USART1, '\r');
			usart_send_char(USART1, '\n');

			GPIOG->ODR ^= 1<<14;
			Delay(200);
			GPIOG->ODR ^= 1<<14;
		}else if(!(strcmp(st, "clear"))){
			usart_send(USART1, clear);
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
		__asm__("nop");
	}
}

void EXTI0_Handler(void){
	GPIOA->ODR ^= 1;	
	Delay(1000);
	GPIOA->ODR ^= 1;	

	EXTI->PR |= 1;
}
