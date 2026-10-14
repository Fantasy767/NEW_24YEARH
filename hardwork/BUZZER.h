#ifndef BUZZER_H
#define BUZZER_H
#include "ti_msp_dl_config.h"

/* 蜂鸣器和 LED 接在同一条电路上(PA12): 拉高时同时"声 + 光" */
void BUZZER_start(void);   /* 打开(立即返回, 不再阻塞) */
void BUZZER_stop(void);    /* 关闭 */

/* 非阻塞提示: 请求鸣叫 ms 毫秒, 由 beep_task() 到点自动关, 不卡住电机控制 */
void beep_request(uint32_t ms);
void beep_task(void);      /* 主循环里一直调用 */

#define BEEP_POINT_MS   150u   /* 每经过一个点的声光提示时长(ms) */
#define BEEP_STOP_MS    500u   /* 到终点停车的提示时长(ms), 长一点便于和途经点区分 */

#endif