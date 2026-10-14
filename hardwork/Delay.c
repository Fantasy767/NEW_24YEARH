#include "Delay.h"
#include "ti_msp_dl_config.h"

/*
 * CPUCLK = 32MHz（SYSCTL.forceDefaultClkConfig = true，SYSOSC 32MHz）
 * 用 delay_cycles 实现，不依赖 SysTick，初始化阶段也能调用
 */
void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        delay_cycles(32000u);
    }
}

void delay_us(uint32_t us)
{
    if (us == 0u)
    {
        return;
    }
    delay_cycles(us * 32u);
}
