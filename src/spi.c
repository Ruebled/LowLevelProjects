#include "stm32f429xx.h"
#include <stdint.h>
#include "systick.h"

#define SPI5_Enable SPI5->CR1 |= SPI_CR1_SPE
#define SPI5_Disable SPI5->CR1 &= ~SPI_CR1_SPE

void SPI5_Init(){
	SPI5->CR1 &= ~SPI_CR1_SPE; //Disable SPI

	SPI5->CR1 &= ~SPI_CR1_RXONLY; //Full duplex

	SPI5->CR1 &= ~SPI_CR1_BIDIMODE; // 2-line unidirectional data mode

	SPI5->CR1 &= ~SPI_CR1_BIDIOE; // Output disabled (Receive-only mode)

	SPI5->CR1 &= ~SPI_CR1_DFF; // Data frame format (8-bit data frame format)

	SPI5->CR1 &= ~SPI_CR1_LSBFIRST; // MSB transmitted first

	SPI5->CR1 |= SPI_CR1_CPOL; // CK to 0 when idle

	SPI5->CR1 |= SPI_CR1_CPHA; // The first clock transition is the first data capture

	SPI5->CR1 &= ~(7U<<3); // Clear SPI baud rate control
	SPI5->CR1 |= (5U<<3); // Set SPI clock to 180MHz/64 = 2..MHz

	SPI5->CR1 &= ~SPI_CR1_CRCEN; // CRC Polynomial

	SPI5->CRCPR = 10; // CRC Polynomial

	SPI5->CR2 &= ~SPI_CR2_FRF; // Frame format (SPI Motorola mode)

	SPI5->CR1 |= SPI_CR1_SSM; // Software slave management enabled

	SPI5->CR1 |= SPI_CR1_MSTR; // Set as Master

	SPI5->CR1 |= SPI_CR1_SSI; // Internal slave select

	SPI5->CR1 |= SPI_CR1_SPE; // Enable SPI
	//SPI5_Enable;
}

void SPI5_REG_SEL(uint8_t reg, uint8_t *buff){
	// Enable SPI by setting CS line low
	GPIOC->BSRR = 1<<1; // pulling CS line low
	Delay(10);

	while( ( SPI5->SR & SPI_SR_TXE ) != SPI_SR_TXE);
    SPI5->DR = reg;

	*buff = SPI5->DR;
	// Disable SPI by setting CS line high 
	GPIOC->ODR |= (1<<(16+1)); //pulling CS line high
	Delay(10);
	
}

void SPI5_Write(uint8_t data, uint8_t *buff, int size){
	// Enable SPI by setting CS line low
	GPIOC->ODR &= ~(1UL<<1); // pulling CS line low
	Delay(10);

	for(int i = 0; i<size; i++){
		while( ( SPI5->SR & SPI_SR_TXE ) != SPI_SR_TXE);
		SPI5->DR = data;

		while( (SPI5->SR & SPI_SR_RXNE ) != SPI_SR_RXNE );
		buff[i] = SPI5->DR;
	}
	//Wait for BSY flag clear
	while( ( SPI5->SR & SPI_SR_BSY ) == SPI_SR_BSY );	

	// Disable SPI by setting CS line high 
	GPIOC->ODR |= (1<<1); //pulling CS line high
}

void SPI5_Read(uint8_t *buff, int size){
	// Enable SPI by setting CS line low
	GPIOC->BSRR = 1<<1; // pulling CS line low
	Delay(10);

	for(int i=0; i<size; i++){
		while( (SPI5->SR & SPI_SR_TXE ) != SPI_SR_TXE );
		SPI5->DR = 0xFF; 

		while( (SPI5->SR & SPI_SR_RXNE ) != SPI_SR_RXNE );
		buff[i] = SPI5->DR;
	}
	//Wait for BSY flag clear
	while( ( SPI5->SR & SPI_SR_BSY ) == SPI_SR_BSY );	

	// Disable SPI by setting CS line high 
	GPIOC->BSRR = 1<<(1+16); //pulling CS line high
	Delay(10);
}
