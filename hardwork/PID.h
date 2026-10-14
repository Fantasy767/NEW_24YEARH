#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct
{
    float kp;
    float ki;
    float kd;
    float err;
    float last_err;
    float target;
    float now;
} MODE_ONE_PID;

/* 速度闭环 PID: 反馈用编码器计数/秒 */
typedef struct
{
    float kp;
    float ki;
    float kd;
    float err;
    float last_err;
    float integral;   /* 积分项, 带限幅抗饱和 */
    float target;
    float now;
} motor_PID;

extern MODE_ONE_PID pid_one;
extern motor_PID   pid_motor_left;
extern motor_PID   pid_motor_right;

void    NO_track_mode(MODE_ONE_PID* pid_mode);
int32_t left_motor_pid_speed(motor_PID* pid_motor);
int32_t right_motor_pid_speed(motor_PID* pid_motor);

/* 停车消抖计数/锁存标志, 定义在 PID.c, 供 empty.c 复位 */
extern uint8_t track_index;
extern uint8_t track_stopped;

/* ===== 控制周期(ms): 航向环和速度环共用 ===== */
#define CONTROL_PERIOD_MS    10u
/* 连续检测到黑线的次数, 超过就停车(防误判) */
#define MODE1_STOP_LINE_COUNT 10u

/* ===== 模式2: 直行 -> 见线循迹, 循环 MODE2_CYCLE_MAX 次 ===== */
#define MODE2_CYCLE_MAX        2u    /* 循环次数: (直行+循迹) 做两轮 */
#define MODE2_ACQ_LINE_COUNT   3u    /* 连续检测到线 3 次(30ms)才进入循迹, 防误判 */
#define MODE2_LOST_LINE_COUNT  30u   /* 连续丢线 30 次(300ms)才算一段循迹结束 */

/* ===== 内环(速度环)参数 ===== */
/* 基础速度: 编码器计数/秒 = 目标圈/秒 × ppr × 2 × ratio
   例: 5 圈/秒 × 13 × 2 × 30 = 3900 (改成你的实际值) */
#define BASE_SPEED_CPS      3900.0f
/* 每 1° 航向偏差产生的左右轮速度差(计数/秒), 需整定 */
#define SPEED_DIFF_PER_DEG  80.0f

/* ============ 模式2: 先直着走, 看到黑线就循迹, 走两轮, 然后停车 ============ */
#define M2_STRAIGHT  0     /* 状态0: 直着走, 顺便找黑线 */
#define M2_TRACK     1     /* 状态1: 压着黑线走 */
extern uint8_t  m2_state;    /* 现在在哪个状态 */
extern uint8_t  m2_round;    /* 已经走完几轮 */
extern uint8_t  m2_seen;     /* 连续看见黑线几次 */
extern uint8_t  m2_lost;     /* 连续看不见黑线几次 */
extern uint8_t  m2_finish;   /* 两轮都走完了吗 */
extern uint32_t m2_tick;     /* 上一次干活的时间(ms) */

void mode_two_clear_pid(void);   /* 清航向环/速度环, 以当前朝向作新的直行方向 */
void mode_two_task(void);        /* 主循环一直调用, 按 m2_state 自己干活 */

/* ===== 模式3: 每轮先朝"当时车头 +38.66°"的方向直行, 见到黑线就循迹, 循环 MODE3_CYCLE_MAX 次 ===== */
/* 场地几何: A(0,0) B(100,0) C(100,-80) D(0,-80), 左右半圆 R=40 与两条直线相切。
   第1轮: A->C 与 AB 方向夹角 = atan(80/100) = 38.66°, 车按 AB 摆正 -> 右转 38.66° 对准 C。
   第2轮: 走完右弧到 B 点后车头朝西, 转到 B->D 方向也是 38.66° -> 左转 38.66°。
   注意: 若摆车时车头朝 D(竖直向下)而不是朝 B, 第1轮应改用 51.34°(同一个角的余角)。 */
#define MODE3_HEADING_DEG      38.66f  /* 第1轮相对车头要转的角度(度); 第2轮取负号 = 反方向 */
#define MODE3_CYCLE_MAX        2u      /* 循环次数: (直行+循迹) 做两轮 */
#define MODE3_ACQ_LINE_COUNT   3u      /* 连续检测到线 3 次(≈40ms)才进入循迹, 防误判 */
#define MODE3_LOST_LINE_COUNT  30u     /* 连续丢线 30 次(≈310ms)才算一段循迹结束 */

#define M3_STRAIGHT  0     /* 状态0: 朝 +MODE3_HEADING_DEG 方向直着走, 顺便找黑线 */
#define M3_TRACK     1     /* 状态1: 压着黑线走 */
extern uint8_t  m3_state;    /* 现在在哪个状态 */
extern uint8_t  m3_round;    /* 已经走完几轮 */
extern uint8_t  m3_seen;     /* 连续看见黑线几次 */
extern uint8_t  m3_lost;     /* 连续看不见黑线几次 */
extern uint8_t  m3_finish;   /* 两轮都走完了吗 */
extern uint32_t m3_tick;     /* 上一次干活的时间(ms) */

void mode_three_clear_pid(void);  /* 每轮开始: 以当前车头为 0°, 目标航向 = +MODE3_HEADING_DEG */
void mode_three_task(void);       /* 主循环一直调用, 按 m3_state 自己干活 */



#endif /* PID_H */