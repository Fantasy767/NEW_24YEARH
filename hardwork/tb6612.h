#ifndef TB6612_H
#define TB6612_H
#include "ti_msp_dl_config.h"
void tb6612_init(void);
void left_motor_speed(uint32_t percent);
void right_motor_speed(uint32_t percent);
#endif