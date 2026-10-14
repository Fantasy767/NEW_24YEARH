#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

/* ============ empty.c 对外提供的接口 ============ */

extern volatile uint32_t uwTick;   /* 1ms 系统节拍, SysTick 中断里自增 */

/* 循迹节拍标志: search() 到点置 1, track_work() 取走并清 0 */
extern uint8_t PID_Control_Flag;

/* 按 TIMG7 计时, 每 TRACK_PERIOD_US(5ms) 把 PID_Control_Flag 置 1 */
void search(void);

#endif /* MAIN_H */
