#include "key.h"
uint16_t key_scan()
{
    uint16_t key_value = 0;
    if(DL_GPIO_readPins(KEY_MODE1_PORT, KEY_MODE1_PIN_2_PIN) == 0)
    {
        key_value = 1;
    }
    else if(DL_GPIO_readPins(KEY_MODE2_PORT, KEY_MODE2_PIN_1_PIN) == 0)
    {
        key_value = 2;
    }
    else if(DL_GPIO_readPins(KEY_MODE3_PORT, KEY_MODE3_PIN_3_PIN) == 0)
    {
        key_value = 3;
    }
    else if(DL_GPIO_readPins(KEY_MODE4_PORT, KEY_MODE4_PIN_4_PIN) == 0)
    {
        key_value = 4;
    }
    return key_value;
}
/* 每 KEY_SCAN_PERIOD_MS 采样一次；只在“新按下”时返回非 0，无变化或松开都返回 0 */
uint16_t key(void)
{
    static uint16_t key_old   = KEY_NONE;
    static uint32_t tick_last = 0;
    uint16_t key_num;

    if((uint32_t)(uwTick - tick_last) < KEY_SCAN_PERIOD_MS)
    {
        return KEY_NONE;
    }
    tick_last = uwTick;

    key_num = key_scan();
    if(key_num == key_old)
    {
        return KEY_NONE;
    }
    key_old = key_num;

    return key_num;   /* 松开时 key_num 为 0，调用方按“无事件”处理 */
}