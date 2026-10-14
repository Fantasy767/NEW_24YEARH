#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

/*
 * 双电机编码器输入捕获
 *   ENCODER_1 : TIMG0  CCP0=PA23(A相)  CCP1=PA24(B相)
 *   ENCODER_2 : TIMG6  CCP0=PA21(A相)  CCP1=PA22(B相)
 */
typedef enum {
    ENCODER_1 = 0,
    ENCODER_2 = 1,
    ENCODER_COUNT
} encoder_id_t;

/* 初始化：打开两个捕获中断（定时器本身已由 SysConfig 启动） */
void encoder_init(void);

/* 累计边沿计数，每个 A 相边沿按方向 ±1（2 倍频） */
int32_t encoder_get_count(encoder_id_t id);
void    encoder_reset_count(encoder_id_t id);

/* 方向：+1 正转，-1 反转，停转返回 0 */
int8_t encoder_get_direction(encoder_id_t id);

/* 最近一次 A 相边沿到下一次 A 相边沿的时间，单位 us；
   长时间无新边沿（电机停转）返回 0 */
uint32_t encoder_get_period_us(encoder_id_t id);

/* 转速，单位：计数值/秒（带符号，正负表示方向） */
int32_t encoder_get_speed_cps(encoder_id_t id);

/*
 * 转速换算成轮子转速（转/分）
 *   ppr   : 编码器每圈脉冲数（A 相一相）
 *   ratio : 电机到轮子的减速比（直连填 1）
 * 每圈计数 = ppr * 2 * ratio （2 倍频）
 */
int32_t encoder_get_rpm(encoder_id_t id, uint16_t ppr, uint16_t ratio);

/* ===== 编码器硬件参数(改成你的实际值) ===== */
#define ENCODER_PPR    13u    /* 编码器每圈 A 相脉冲数(线数), 常见 11/13/20 */
#define ENCODER_RATIO  30u    /* 电机:轮子 减速比, 直连填 1 */

/* 每秒圈数(转/秒) = 计数/秒 ÷ 每圈计数(ppr*2*ratio) */
int32_t encoder_get_s(encoder_id_t id, uint16_t ppr, uint16_t ratio);

#endif
