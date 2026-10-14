#include"tb6612.h"

/* 周期值必须与 SysConfig 里对应 PWM 的 timerCount 一致 */
#define TIMA0_C2_PERIOD  400    /* TIMA0_C2: 中心对齐, 8MHz/400 = 20kHz */
#define TIMA1_C1_PERIOD  400    /* TIMA1_C1: 中心对齐, 8MHz/400 = 20kHz */

void tb6612_init(void)
{
    /* 中心对齐下占空比与 CCR 成反比: CCR = period/2 才是 0%(停转)。
       先设成 0% 再启动定时器, 避免上电瞬间冲车 */
    DL_TimerA_setCaptureCompareValue(TIMA0_C2_INST, TIMA0_C2_PERIOD / 2, DL_TIMER_CC_2_INDEX);
    DL_TimerA_setCaptureCompareValue(TIMA1_C1_INST, TIMA1_C1_PERIOD / 2, DL_TIMER_CC_1_INDEX);

    /* SysConfig 里这两个定时器的 startTimer = DL_TIMER_STOP,
       必须手动启动计数器, 否则 PWM 不会有输出 */
    DL_TimerA_startCounter(TIMA0_C2_INST);
    DL_TimerA_startCounter(TIMA1_C1_INST);
	DL_GPIO_setPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN_6_PIN);
    DL_GPIO_clearPins(TB6612_AIN1_PORT,TB6612_AIN1_PIN_5_PIN);

    DL_GPIO_setPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN_8_PIN);
    DL_GPIO_clearPins(TB6612_BIN1_PORT,TB6612_BIN1_PIN_7_PIN);
    
}
void left_motor_speed(uint32_t percent)
{
    if (percent > 100) {
        percent = 100;
    }

    /* 中心对齐: duty = 1 - 2*CCR/period  =>  CCR = period*(100-percent)/200 */
    uint32_t ccr = (TIMA0_C2_PERIOD * (100 - percent)) / 200;

    DL_TimerA_setCaptureCompareValue(TIMA0_C2_INST, ccr, DL_TIMER_CC_2_INDEX);
}

void right_motor_speed(uint32_t percent)
{
    if (percent > 100) {
        percent = 100;
    }

    /* 中心对齐: duty = 1 - 2*CCR/period  =>  CCR = period*(100-percent)/200 */
    uint32_t ccr = (TIMA1_C1_PERIOD * (100 - percent)) / 200;

    DL_TimerA_setCaptureCompareValue(TIMA1_C1_INST, ccr, DL_TIMER_CC_1_INDEX);
}