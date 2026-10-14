#ifndef TRACK_H
#define TRACK_H
#include "ti_msp_dl_config.h"

/* ==================== 8路循迹引脚 ==================== */
/*
        左                         右

        T0      T1      T2      T3      T4      T5       T6         T7 

        B08     B27    B09     B26    B21      B19       B20        B22
*/

#define TRACK_T0_PORT   B08_PORT 
#define TRACK_T0_PIN    B08_PIN_9_PIN

#define TRACK_T1_PORT   B27_PORT 
#define TRACK_T1_PIN    B27_PIN_10_PIN

#define TRACK_T2_PORT   B09_PORT 
#define TRACK_T2_PIN    B09_PIN_11_PIN

#define TRACK_T3_PORT   B26_PORT 
#define TRACK_T3_PIN    B26_PIN_12_PIN

#define TRACK_T4_PORT   B21_PORT 
#define TRACK_T4_PIN    B21_PIN_13_PIN

#define TRACK_T5_PORT   B19_PORT 
#define TRACK_T5_PIN    B19_PIN_14_PIN

#define TRACK_T6_PORT   B20_PORT 
#define TRACK_T6_PIN    B20_PIN_15_PIN

#define TRACK_T7_PORT   B22_PORT 
#define TRACK_T7_PIN    B22_PIN_16_PIN

#define READD_T0  (DL_GPIO_readPins(TRACK_T0_PORT, TRACK_T0_PIN)!=0)
#define READD_T1  (DL_GPIO_readPins(TRACK_T1_PORT, TRACK_T1_PIN)!=0)
#define READD_T2  (DL_GPIO_readPins(TRACK_T2_PORT, TRACK_T2_PIN)!=0)
#define READD_T3  (DL_GPIO_readPins(TRACK_T3_PORT, TRACK_T3_PIN)!=0)
#define READD_T4  (DL_GPIO_readPins(TRACK_T4_PORT, TRACK_T4_PIN)!=0)
#define READD_T5  (DL_GPIO_readPins(TRACK_T5_PORT, TRACK_T5_PIN)!=0)
#define READD_T6  (DL_GPIO_readPins(TRACK_T6_PORT, TRACK_T6_PIN)!=0)
#define READD_T7  (DL_GPIO_readPins(TRACK_T7_PORT, TRACK_T7_PIN)!=0)
/* 循迹周期: TIMG7 时基 = 32MHz/32 = 1MHz, 1 个计数 = 1us */
#define TRACK_PERIOD_US   5000U   /* 5ms */
extern int Base_Speed;      /* 基础速度(%), 定义在 track.c */

#define SPEED_DIFF_PER_ERROR 80.0f
int trackc_get_error(void);
void track_work(void);
uint8_t track_on_or_off(void);
#endif