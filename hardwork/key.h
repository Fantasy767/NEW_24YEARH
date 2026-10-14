#ifndef KEY_H
#define KEY_H
#include "ti_msp_dl_config.h"

/* key() 返回值：KEY_NONE = 本次无新按下，1~4 = 对应按键新按下 */
#define KEY_NONE             0u
#define KEY_MODE1            1u
#define KEY_MODE2            2u
#define KEY_MODE3            3u
#define KEY_MODE4            4u

#define KEY_SCAN_PERIOD_MS   20u   /* 采样周期，同时充当消抖窗口 */

/* 系统滴答，由 empty.c 中的 SysTick_Handler 每 1ms 递增 */
extern volatile uint32_t uwTick;

uint16_t key_scan(void); 
uint16_t key(void);
#endif