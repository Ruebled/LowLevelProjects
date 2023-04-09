#include "stm32f429xx.h"

void SPI5_Init(){
	// Enable SPI5 and it's clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI5RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI5RST;
		
	SPI5->CR1 &= ~SPI_CR1_SPE; //Disable SPI

	SPI5->CR1 &= ~SPI_CR1_RXONLY; //Full duplex

	SPI5->CR1 &= ~SPI_CR1_BIDIMODE; // 2-line unidirectional data mode

	SPI5->CR1 &= ~SPI_CR1_BIDIOE; // Output disabled (Receive-only mode)
	
	SPI5->CR1 &= ~SPI_CR1_DFF; // Data frame format (8-bit data frame format)

	SPI5->CR1 &= ~SPI_CR1_LSBFIRST; // MSB transmitted first

	SPI5->CR1 &= ~SPI_CR1_CPOL; // CK to 0 when idle

	SPI5->CR1 &= ~SPI_CR1_CPHA; // The first clock transition is the first data capture

	SPI5->CR1 |= (3U<<3); // Set SPI clock to 16MHz/16 = 1MHz

	SPI5->CRCPR = 10; // CRC Polynomial

	SPI5->CR1 &= ~SPI_CR1_CRCEN; // CRC Polynomial

	SPI5->CR2 &= ~SPI_CR2_FRF; // Frame format (SPI Motorola mode)
	
	SPI5->CR1 |= SPI_CR1_SSM; // Software slave management enabled

	SPI5->CR1 |= SPI_CR1_MSTR; // Set as Master

	SPI5->CR1 |= SPI_CR1_SSI; // Internal slave select

	SPI5->CR1 |= SPI_CR1_SPE; // Enable SPI
}


