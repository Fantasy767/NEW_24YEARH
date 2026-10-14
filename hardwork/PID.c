#include "track.h"
#include "imu.h"
#include "PID.h"
#include "encoder.h"
#include "tb6612.h"
#include "BUZZER.h"
#include "main.h"   /* uwTick / PID_Control_Flag / search() */

/* 模式1 航向环限幅 */
#define PID_OUTPUT_MAX     20.0f

/* 速度环参数 */
#define MOTOR_OUT_MAX      100.0f   /* 速度环输出上限(%占空比) */
#define MOTOR_I_MAX        50.0f    /* 积分限幅(防饱和) */

MODE_ONE_PID pid_one = { .kp = 2.3f, .ki = 0.1f, .kd = 0.3f, .target = 0.0f };
motor_PID   pid_motor_left  = { .kp = 1.5f, .ki = 0.05f, .kd = 0.2f, .target = 0.0f };
motor_PID   pid_motor_right = { .kp = 1.4f, .ki = 0.05f, .kd = 0.2f, .target = 0.0f };

uint8_t track_index = 0;
uint8_t track_stopped = 0;

uint8_t  m2_state  = M2_STRAIGHT;  /* 现在在哪个状态 */
uint8_t  m2_round  = 0;            /* 已经走完几轮 */
uint8_t  m2_seen   = 0;            /* 连续看见黑线几次 */
uint8_t  m2_lost   = 0;            /* 连续看不见黑线几次 */
uint8_t  m2_finish = 0;            /* 两轮都走完了吗 */
uint32_t m2_tick   = 0;            /* 上一次干活的时间(ms) */


uint8_t  m3_state  = M3_STRAIGHT;  /* 现在在哪个状态 */
uint8_t  m3_round  = 0;            /* 已经走完几轮 */
uint8_t  m3_seen   = 0;            /* 连续看见黑线几次 */
uint8_t  m3_lost   = 0;            /* 连续看不见黑线几次 */
uint8_t  m3_finish = 0;            /* 两轮都走完了吗 */
uint32_t m3_tick   = 0;            /* 上一次干活的时间(ms) */

/* 单电机速度闭环(PI): feedback = 编码器计数/秒, target 单位同计数/秒 */
int32_t left_motor_pid_speed(motor_PID* pid_motor)
{
    /* 计数/秒精度高且不依赖 ppr/ratio;
       设目标: target = 目标圈/秒 × ppr × 2 × ratio */
    pid_motor->now  = encoder_get_speed_cps(ENCODER_1);
    pid_motor->err  = pid_motor->target - pid_motor->now;

    pid_motor->integral += pid_motor->err;
    if (pid_motor->integral >  MOTOR_I_MAX) pid_motor->integral =  MOTOR_I_MAX;
    if (pid_motor->integral < -MOTOR_I_MAX) pid_motor->integral = -MOTOR_I_MAX;

    float out = pid_motor->kp * pid_motor->err
              + pid_motor->ki * pid_motor->integral
              + pid_motor->kd * (pid_motor->err - pid_motor->last_err);
    pid_motor->last_err = pid_motor->err;

    /* 电机不可反转: 输出夹到 0~100 */
    if (out >  MOTOR_OUT_MAX) out = MOTOR_OUT_MAX;
    if (out <  0.0f)           out = 0.0f;

    
    return (int32_t) out;
}
int32_t right_motor_pid_speed(motor_PID* pid_motor)
{
    /* 计数/秒精度高且不依赖 ppr/ratio;
       设目标: target = 目标圈/秒 × ppr × 2 × ratio */
    pid_motor->now  = encoder_get_speed_cps(ENCODER_2);
    pid_motor->err  = pid_motor->target - pid_motor->now;

    pid_motor->integral += pid_motor->err;
    if (pid_motor->integral >  MOTOR_I_MAX) pid_motor->integral =  MOTOR_I_MAX;
    if (pid_motor->integral < -MOTOR_I_MAX) pid_motor->integral = -MOTOR_I_MAX;

    float out = pid_motor->kp * pid_motor->err
              + pid_motor->ki * pid_motor->integral
              + pid_motor->kd * (pid_motor->err - pid_motor->last_err);
    pid_motor->last_err = pid_motor->err;

    /* 电机不可反转: 输出夹到 0~100 */
    if (out >  MOTOR_OUT_MAX) out = MOTOR_OUT_MAX;
    if (out <  0.0f)           out = 0.0f;

    
    return (int32_t) out;
}
void NO_track_mode(MODE_ONE_PID* pid_mode)
{
    static uint32_t tick_last = 0;

    /* 固定节拍: 给 D 项和 track_index 消抖一个确定的 dt */
    if ((uint32_t)(uwTick - tick_last) < CONTROL_PERIOD_MS) return;
    tick_last = uwTick;

    if (track_stopped)
    {
        left_motor_speed(0);
        right_motor_speed(0);
        return;
    }

    /* 读传感器消抖: 连续 MODE1_STOP_LINE_COUNT 次检测到黑线才停车 */
    
    
    /* 外环: 航向闭环, err>0 表示当前朝向偏右, 需要左转(左轮慢) */
    pid_mode->now = imu_get_yaw();
    pid_mode->err = pid_mode->target - pid_mode->now;

    float P = pid_mode->kp * pid_mode->err;
    float D = pid_mode->kd * (pid_mode->err - pid_mode->last_err);
    float output = P + D;               /* 单位: 度 */
    pid_mode->last_err = pid_mode->err;

    if (output >  PID_OUTPUT_MAX) output =  PID_OUTPUT_MAX;
    if (output < -PID_OUTPUT_MAX) output = -PID_OUTPUT_MAX;

    /* 航向偏差转成左右轮速度差(计数/秒), 作为内环速度目标 */
    float diff = output * SPEED_DIFF_PER_DEG;
    pid_motor_left.target  = BASE_SPEED_CPS - diff;   /* 左转: 左轮慢 */
    pid_motor_right.target = BASE_SPEED_CPS + diff;

    /* 内环: 每轮各自的速度 PI 闭环 */
    left_motor_speed(left_motor_pid_speed(&pid_motor_left));
    right_motor_speed(right_motor_pid_speed(&pid_motor_right));
}

/* 把航向环和速度环清零, 让它拿现在的方向当新的直行方向 */
 void mode_two_clear_pid(void)
{
    imu_reset_yaw();
    pid_one.target   = 0.0f;
    pid_one.last_err = 0.0f;
    pid_motor_left.integral  = 0.0f;
    pid_motor_left.last_err  = 0.0f;
    pid_motor_right.integral = 0.0f;
    pid_motor_right.last_err = 0.0f;
}
/* 模式3 每一轮开始: 以当前车头为 0°, 目标航向 = +MODE3_HEADING_DEG(第1轮右转 38.66° 对准 C 点) */
void mode_three_clear_pid(void)
{
    imu_reset_yaw();
    pid_one.target   = MODE3_HEADING_DEG;
    pid_one.last_err = 0.0f;
    pid_motor_left.integral  = 0.0f;
    pid_motor_left.last_err  = 0.0f;
    pid_motor_right.integral = 0.0f;
    pid_motor_right.last_err = 0.0f;
}
void mode_three_task(void)
{
    if(m3_finish == 1)          /* 两轮都走完了: 停住别动 */
    {
        left_motor_speed(0);
        right_motor_speed(0);
        return;
    }
    /* 每 10ms 干一次活, 时间还没到就等着(计数要按时间加, 不然几下就冲到阈值) */
    if ((uint32_t)(uwTick - m3_tick) < CONTROL_PERIOD_MS) return;
    m3_tick = uwTick;

    if (m3_state == M3_STRAIGHT)
    {
        NO_track_mode(&pid_one);      /* 还没有线, 照着原方向直走 */

        if (track_on_or_off() == 1) m3_seen = m3_seen + 1;   /* 看见线, 加 1 */
        else                        m3_seen = 0;             /* 没看见, 清零 */

        if (m3_seen > MODE3_ACQ_LINE_COUNT)   /* 连着看见好几次, 才算真的找到线 */
        {
            m3_state = M3_TRACK;    /* 换状态: 开始循迹 */
            m3_lost  = 0;
            beep_request(BEEP_POINT_MS);   /* 到达 C 点(第 2 轮到 D 点): 声光提示 */
        }
    }
    else
    {
        if(track_on_or_off() == 1) m3_lost = 0;             /* 还在线上 */
        else                   m3_lost = m3_lost + 1;   /* 看不见, 加 1 */
        if(m3_lost > MODE3_LOST_LINE_COUNT)   /* 连着看不见很久, 这一轮走完了 */
        {
            m3_round = m3_round + 1;           /* 算走完一轮 */

            if (m3_round >= MODE3_CYCLE_MAX)   /* 两轮都走完了: 停车, 响一声 */
            {
                m3_finish = 1;
                left_motor_speed(0);
                right_motor_speed(0);
                beep_request(BEEP_STOP_MS);    /* 回到 A 点停车: 声光提示 */
            }
            else                               /* 还有下一轮: 回到直走状态 */
            {
                m3_state = M3_STRAIGHT;
                m3_seen  = 0;
                m3_lost  = 0;
                mode_three_clear_pid();                /* 以当前车头为 0°(里面会把 target 设成 +MODE3_HEADING_DEG) */
                pid_one.target = -MODE3_HEADING_DEG;   /* 第 2 轮左转 38.66° 对准 D 点, 必须在清环之后写 */
                beep_request(BEEP_POINT_MS);           /* 到达 B 点: 声光提示 */
            }
        }
        else
        {
            search();        /* 5ms 节拍, 置 PID_Control_Flag */
            track_work();    /* 循迹控制 */
        }
    }

}
/* 主循环里一直调用它, 它会按现在的状态自己干活 */
 void mode_two_task(void)
{
    if (m2_finish == 1)          /* 两轮都走完了: 停住别动 */
    {
        left_motor_speed(0);
        right_motor_speed(0);
        return;
    }

    /* 每 10ms 干一次活, 时间还没到就等着(计数要按时间加, 不然几下就冲到阈值) */
    if ((uint32_t)(uwTick - m2_tick) < CONTROL_PERIOD_MS) return;
    m2_tick = uwTick;

    if (m2_state == M2_STRAIGHT)
    {
        NO_track_mode(&pid_one);      /* 还没有线, 照着原方向直走 */

        if (track_on_or_off() == 1) m2_seen = m2_seen + 1;   /* 看见线, 加 1 */
        else                        m2_seen = 0;             /* 没看见, 清零 */

        if (m2_seen > MODE2_ACQ_LINE_COUNT)   /* 连着看见好几次, 才算真的找到线 */
        {
            m2_state = M2_TRACK;    /* 换状态: 开始循迹 */
            m2_lost  = 0;
            beep_request(BEEP_POINT_MS);   /* 到达 B 点(第 2 轮到 D 点): 声光提示 */
        }
    }
    else /* M2_TRACK */
    {
        if (track_on_or_off() == 1) m2_lost = 0;             /* 还在线上 */
        else                        m2_lost = m2_lost + 1;   /* 看不见, 加 1 */

        if (m2_lost > MODE2_LOST_LINE_COUNT)   /* 连着看不见很久, 这一轮走完了 */
        {
            m2_round = m2_round + 1;           /* 算走完一轮 */

            if (m2_round >= MODE2_CYCLE_MAX)   /* 两轮都走完了: 停车, 响一声 */
            {
                m2_finish = 1;
                left_motor_speed(0);
                right_motor_speed(0);
                beep_request(BEEP_STOP_MS);    /* 回到 A 点停车: 声光提示 */
            }
            else                               /* 还有下一轮: 回到直走状态 */
            {
                m2_state = M2_STRAIGHT;
                m2_seen  = 0;
                m2_lost  = 0;
                mode_two_clear_pid();
                beep_request(BEEP_POINT_MS);   /* 到达 C 点: 声光提示 */
            }
        }
        else
        {
            search();        /* 5ms 节拍, 置 PID_Control_Flag */
            track_work();    /* 循迹控制 */
        }
    }
}