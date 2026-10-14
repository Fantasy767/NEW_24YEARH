#include "imu.h"
#include "Delay.h"

/*
 * 偏航角(Z轴)计算
 *
 * 思路和参考工程 imu.c / app_flight.c 一致：
 *   1. 角速度低通滤波       —— 参考工程 filter.c 里的一阶低通 Filter_LowPass
 *   2. 死区                 —— 参考工程 yaw_G 在 ±0.5°/s 内不积分，抑制漂移
 *   3. 对角速度积分得到角度 —— 参考工程 eulerAngle->yaw += yaw_G * dt
 *
 * 两处针对小车做了改动：
 *   1. dt 用 SysTick(uwTick) 实测，不再写死 0.01（参考工程传 0.01，
 *      但实际主循环是 6.4ms，角度会被放大约 1.5 倍）；
 *   2. 上电先测 Z 轴零偏并扣掉，再保留一个小死区，漂移小很多。
 *
 * 注意：MPU6050 没有磁力计，偏航角只能靠陀螺积分，长时间必然缓慢漂移；
 *      加速度计/卡尔曼滤波对偏航角没有帮助（它们测不到绕 Z 轴的转动）。
 *      跑直线这种几十秒的任务，做完零偏校准足够用。
 */

/* ±2000°/s 档灵敏度 16.4 LSB/(°/s) → 2000/32768 = 0.061035 °/s 每 LSB */
#define GYRO_LSB_TO_DPS   (2000.0f / 32768.0f)

/* 上电静止判据：连续 IMU_CAL_STABLE_CNT 次采样 Z 轴原始值抖动都小于 IMU_CAL_STABLE_DIFF */
#define IMU_CAL_STABLE_CNT    20u
#define IMU_CAL_STABLE_DIFF   100    /* LSB，约 6°/s：能容忍手抖和噪声，但车在动就不算静止 */

/* 零偏平均次数 */
#define IMU_CAL_SAMPLES       100u

static float s_gyro_z_dps  = 0.0f;   /* 低通后的 Z 轴角速度 */
static float s_gyro_z_bias = 0.0f;   /* 上电测得的 Z 轴零偏 */
static float s_yaw         = 0.0f;   /* 累积偏航角 */
static float s_target_yaw  = 0.0f;   /* 目标航向 */

/* ==================== 零偏校准 ==================== */

bool imu_calibrate(void)
{
    int16_t z;
    int16_t last;
    uint16_t stable = 0;
    uint32_t retry;
    uint16_t i;
    float sum = 0.0f;
    int16_t diff;

    /* 1. 先等车停稳：连续若干次采样角速度抖动都很小才算静止 */
    last = mpu6050_get_gyro_z();

    for (retry = 0; retry < 400u; retry++)
    {
        z = mpu6050_get_gyro_z();

        diff = (int16_t)(z - last);
        if (diff < 0)
        {
            diff = (int16_t)(-diff);
        }
        last = z;

        if (diff < IMU_CAL_STABLE_DIFF)
        {
            stable++;
            if (stable >= IMU_CAL_STABLE_CNT)
            {
                break;
            }
        }
        else
        {
            stable = 0u;
        }

        delay_ms(5);
    }
    if (stable < IMU_CAL_STABLE_CNT)
    {
        return false;   /* 一直没停稳，校准结果不可信 */
    }

    /* 2. 取平均作为 Z 轴零偏 */
    for (i = 0; i < IMU_CAL_SAMPLES; i++)
    {
        sum += (float)mpu6050_get_gyro_z() * GYRO_LSB_TO_DPS * IMU_YAW_SIGN;
        delay_ms(5);
    }
    s_gyro_z_bias = sum / (float)IMU_CAL_SAMPLES;

    /* 3. 校准完把当前朝向当 0° */
    s_gyro_z_dps = 0.0f;
    imu_reset_yaw();

    return true;
}

bool imu_init(void)
{
    if (!mpu6050_init())
    {
        return false;   /* 器件没应答 */
    }
    return imu_calibrate();
}

/* ==================== 采样与积分 ==================== */

bool imu_update_ready(void)
{
    static uint32_t last_tick = 0;

    if ((uint32_t)(uwTick - last_tick) < IMU_UPDATE_PERIOD_MS)
    {
        return false;
    }
    last_tick = uwTick;
    return true;
}

void imu_update(void)
{
    static uint32_t last_tick = 0;
    static bool     started = false;
    uint32_t now;
    float rate;
    float dt;

    /* 1. 读 Z 轴角速度原始值，并换算成 °/s */
    rate = (float)mpu6050_get_gyro_z() * GYRO_LSB_TO_DPS * IMU_YAW_SIGN;

    /* 3. 一阶低通（同参考工程 Filter_LowPass） */
    s_gyro_z_dps = IMU_GYRO_LPF_ALPHA * rate + (1.0f - IMU_GYRO_LPF_ALPHA) * s_gyro_z_dps;

    /* 4. 扣掉上电时测的零偏 */
    rate = s_gyro_z_dps - s_gyro_z_bias;

    /* 5. 用实测 dt 积分 */
    now = uwTick;
    if (!started)
    {
        started = true;
        last_tick = now;
        return;
    }
    dt = (float)(uint32_t)(now - last_tick) * 0.001f;
    last_tick = now;

    if (dt <= 0.0f || dt > IMU_DT_MAX_S)
    {
        return;   /* 主循环被拖太久，这次不积分 */
    }

    /* 6. 死区：太小认为是残余漂移，不积分 */
    if (rate > IMU_YAW_DEADZONE_DPS || rate < -IMU_YAW_DEADZONE_DPS)
    {
        s_yaw += rate * dt;
    }
}

/* ==================== 偏航角与航向 ==================== */

float imu_get_yaw(void)
{
    return s_yaw;
}

void imu_reset_yaw(void)
{
    s_yaw = 0.0f;
}

void imu_set_target_yaw(float deg)
{
    s_target_yaw = deg;
}

float imu_get_target_yaw(void)
{
    return s_target_yaw;
}

/**
 * @brief 当前朝向与目标航向的偏差
 * @return 范围 [-180, +180]，正数表示目标在当前朝向的左边（需要左转）
 */
float imu_heading_error(void)
{
    float err = s_target_yaw - s_yaw;

    /* 归一化，处理跨越 ±180° 的情况 */
    while (err > 180.0f)
    {
        err -= 360.0f;
    }
    while (err < -180.0f)
    {
        err += 360.0f;
    }

    return err;
}

/**
 * @brief 最简转向判断（不带 PID）
 * @return -1 = 左转, 0 = 直行, +1 = 右转
 */
int8_t imu_heading_dir(void)
{
    float err = imu_heading_error();

    if (err > IMU_HEADING_DEADBAND_DEG)
    {
        return -1;
    }
    if (err < -IMU_HEADING_DEADBAND_DEG)
    {
        return 1;
    }
    return 0;
}

float imu_get_gyro_z_dps(void)
{
    return s_gyro_z_dps - s_gyro_z_bias;
}

float imu_get_gyro_z_bias(void)
{
    return s_gyro_z_bias;
}
