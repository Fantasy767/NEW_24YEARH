#include "BUZZER.h"
#include "main.h"   /* uwTick */

void BUZZER_start(void)
{
    DL_GPIO_setPins(BUZZER_PORT, BUZZER_PIN_0_PIN);   /* LED 和蜂鸣器同电路, 一起亮/响 */
}

void BUZZER_stop(void)
{
    DL_GPIO_clearPins(BUZZER_PORT, BUZZER_PIN_0_PIN);
}

/* 非阻塞提示音: 记录该关的时间点, 到点由 beep_task() 关掉 */
static uint32_t beep_until = 0;
static uint8_t  beep_on    = 0;

void beep_request(uint32_t ms)
{
    BUZZER_start();
    beep_until = uwTick + ms;
    beep_on    = 1;
}

void beep_task(void)
{
    if (beep_on && (int32_t)(uwTick - beep_until) >= 0)
    {
        BUZZER_stop();
        beep_on = 0;
    }
}