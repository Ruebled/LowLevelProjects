#ifndef SPI_H
#define SPI_H
#include <stdint.h>

void SPI5_Init();

void SPI5_Write(uint8_t whoami, uint8_t *buff, int size);

void SPI5_Read(uint8_t *buff, int size);

#endif
