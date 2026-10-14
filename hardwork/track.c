#include "track.h"
#include "tb6612.h"
#include "PID.h"
#include "main.h"
#include "mpu6050.h"
#include <stdlib.h>

#define BASE_SPEED_DEFAULT  45
#define CORRECTION_MAX      30
#define MOTOR_SPEED_MAX    100
#define ERROR_SCALE         10
#define LOST_TIMEOUT_MS     50
#define LOST_COUNT_MAX      ((LOST_TIMEOUT_MS * 1000U) / TRACK_PERIOD_US)
#define LOST_HOLD_ERROR     3

static int last_error = 0;
static int lost_count = 0;
static float Kp = 1.8f;
static float Kd = 0.75f;
static int last_error_pid = 0;
int Base_Speed = BASE_SPEED_DEFAULT;

uint8_t track_on_or_off(void)
{
    uint8_t T0=READD_T0;
    uint8_t T1=READD_T1;
    uint8_t T2=READD_T2;
    uint8_t T3=READD_T3;
    uint8_t T4=READD_T4;
    uint8_t T5=READD_T5;
    uint8_t T6=READD_T6;
    uint8_t T7=READD_T7;
    if((T0!=0)||(T1!=0)||(T2!=0)||(T3!=0)||(T4!=0)||(T5!=0)||(T6!=0)||(T7!=0))
    {
        return 1u;
    }
    return 0u;
}
int trackc_get_error(void)
{
    uint8_t T0=READD_T0;
    uint8_t T1=READD_T1;
    uint8_t T2=READD_T2;
    uint8_t T3=READD_T3;
    uint8_t T4=READD_T4;
    uint8_t T5=READD_T5;
    uint8_t T6=READD_T6;
    uint8_t T7=READD_T7;
    

    uint8_t sensors[8] = {T0, T1, T2, T3, T4, T5, T6, T7};
    int weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};

    int sum = 0, count = 0;
    for (int i = 0; i < 8; i++) 
    {
        if (sensors[i] == 1)
         {
            sum += weights[i];
            count++;
        }
    }

    if(count==0)
    {
        lost_count++;
        int abs_last = abs(last_error);
        if(lost_count > (int) LOST_COUNT_MAX)
        {
            lost_count=0;
            return 0;
        }
        else
        {
            if(abs_last > LOST_HOLD_ERROR)
            {
                return last_error;
            }
            else
            {
                return 0;
            }
        }
    }
    lost_count=0;
    int error = (sum * ERROR_SCALE) / count;
    last_error = error;
    return error;

}
void track_work(void)
{
    if(PID_Control_Flag == 1)
        {
            int Error = trackc_get_error();
            PID_Control_Flag = 0;
            
            float P = Kp * Error;
            float D = Kd * (Error - last_error_pid);
            int correction = (int)(P + D);
            last_error_pid = Error;

            // 限幅转向
            if(correction > CORRECTION_MAX) correction = CORRECTION_MAX;
            if(correction < -CORRECTION_MAX) correction = -CORRECTION_MAX;

           
             /* 外环输出 -> 左右轮目标转速差(计数/秒): 这就是串级里的"接口" */
            float diff = (float) correction * SPEED_DIFF_PER_ERROR;
            pid_motor_left.target  = BASE_SPEED_CPS + diff;   /* 误差为正=线在右: 左轮快, 车右转 */
            pid_motor_right.target = BASE_SPEED_CPS - diff;
            left_motor_speed(left_motor_pid_speed(&pid_motor_left));
            right_motor_speed(right_motor_pid_speed(&pid_motor_right));
        }
}
        