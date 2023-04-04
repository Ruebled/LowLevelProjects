#ifndef SYSTICK_H
#define SYSTICK_H
#include <stdint.h>

void SysTick_Init(uint32_t ticks);

void Delay(uint32_t nTime);

void SysTick_Handler(void);


#endif
