#ifndef __IMU_H
#define __IMU_H

#include "mpu6050.h"
#include <stdbool.h>

/* ==================== 可调参数 ==================== */

/* 陀螺仪 Z 轴安装方向：车向左转(逆时针)时偏航角应该增大。
   实测反了（模块倒扣安装）就把这里改成 -1.0f */
#define IMU_YAW_SIGN             1.0f

/* 角速度一阶低通系数，和参考工程 filter.c 里的 ALPHA=0.15 一致。
   α 越小越平滑但滞后越大；100Hz 下 0.15 大约滞后 60ms，
   觉得转向反应慢就调大到 0.3~0.5 */
#define IMU_GYRO_LPF_ALPHA       0.15f

/* 角速度死区(°/s)：比这个还小就认为车没在转，不积分，抑制残余漂移。
   参考工程用 0.5，我们上电做了零偏校准，可以收紧到 0.1 */
#define IMU_YAW_DEADZONE_DPS     0.1f

/* 采样周期(ms)：100Hz */
#define IMU_UPDATE_PERIOD_MS     10u

/* 单次积分允许的最大 dt(s)：主循环被拖太久就丢弃这次积分，防止角度跳变 */
#define IMU_DT_MAX_S             0.1f

/* 航向死区(°)：偏差在这个范围内认为已经在目标方向上 */
#define IMU_HEADING_DEADBAND_DEG 3.0f

/* ==================== 接口 ==================== */

bool imu_init(void);           /* 初始化 + 零偏校准，调用时小车必须静止 */
bool imu_calibrate(void);      /* 只做零偏校准 */
bool imu_update_ready(void);   /* 到采样时间了返回 true */
void imu_update(void);         /* 读传感器 → 低通 → 积分为偏航角 */

float imu_get_yaw(void);       /* 当前偏航角(°)，向左转(逆时针)为正 */
void imu_reset_yaw(void);      /* 把当前朝向当作 0° */

void imu_set_target_yaw(float deg);  /* 设定目标航向(°) */
float imu_get_target_yaw(void);
float imu_heading_error(void); /* 与目标航向的偏差(°)，范围[-180,180]，正=需要左转 */
int8_t imu_heading_dir(void);  /* -1=左转, 0=直行, +1=右转 */

float imu_get_gyro_z_dps(void);      /* 扣除零偏后的 Z 轴角速度(°/s)，调试用 */
float imu_get_gyro_z_bias(void);     /* 上电测得的零偏(°/s)，调试用 */

#endif
