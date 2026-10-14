#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/* 系统滴答：由 empty.c 里的 SysTick_Handler 每 1ms 递增 */
extern volatile uint32_t uwTick;

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif
