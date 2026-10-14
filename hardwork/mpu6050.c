#include "mpu6050.h"
#include "Delay.h"
#include <stdint.h>

/* MPU6050 独占 I2C0：SDA=PA28、SCL=PA31（实例名 MPU6050_I2C，见 ti_msp_dl_config.h）
   注意：宏 MPU6050_I2C_INST 由 SysConfig 生成，不要在这里重新定义 */

#define MPU_I2C_TIMEOUT_MS   20u   /* 单次 I2C 传输超时(ms) */
#define MPU6050_PERIOD   5u 
/* ======================== I2C 底层 ======================== */

/* 等待控制器空闲（总线上可能刚有别的从机在通信） */
static bool mpu_wait_idle(void)
{
    uint32_t start = uwTick;

    while (!(DL_I2C_getControllerStatus(MPU6050_I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
    {
        if ((uint32_t)(uwTick - start) > MPU_I2C_TIMEOUT_MS)
        {
            return false;
        }
    }
    return true;
}

/* 等待本次传输结束 */
static bool mpu_wait_done(void)
{
    uint32_t start = uwTick;

    while (DL_I2C_getControllerStatus(MPU6050_I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY)
    {
        if ((uint32_t)(uwTick - start) > MPU_I2C_TIMEOUT_MS)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief 向MPU6050的指定寄存器写入一个字节
 * @param reg   寄存器地址
 * @param value 要写入的数据
 */
void mpu6050_write_reg(uint8_t reg, uint8_t data)
{
    uint8_t txBuf[2] = {reg, data};

    if (!mpu_wait_idle())
    {
        return;
    }

    DL_I2C_flushControllerTXFIFO(MPU6050_I2C_INST);
    DL_I2C_flushControllerRXFIFO(MPU6050_I2C_INST);
    DL_I2C_fillControllerTXFIFO(MPU6050_I2C_INST, txBuf, 2);

    DL_I2C_startControllerTransfer(MPU6050_I2C_INST, mpu6050_address,
                                   DL_I2C_CONTROLLER_DIRECTION_TX, 2);

    mpu_wait_done();
}

/**
 * @brief 读MPU6050的一个寄存器
 */
void mpu6050_read_reg(uint8_t reg, uint8_t *data)
{
    mpu6050_read_regs(reg, data, 1);
}

/**
 * @brief 从指定寄存器开始连续读 len 个字节
 * @note  MPU6050 的寄存器地址会自动加一，所以 6 轴数据一次就能读完，
 *        比一个寄存器读一次快十几倍（读 14 字节只要两次 I2C 传输）
 */
void mpu6050_read_regs(uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint16_t i;

    if (!mpu_wait_idle())
    {
        return;
    }

    DL_I2C_flushControllerTXFIFO(MPU6050_I2C_INST);
    DL_I2C_flushControllerRXFIFO(MPU6050_I2C_INST);

    /* 阶段1：把要读的寄存器地址写下去 */
    DL_I2C_fillControllerTXFIFO(MPU6050_I2C_INST, &reg, 1);
    DL_I2C_startControllerTransfer(MPU6050_I2C_INST, mpu6050_address,
                                   DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    if (!mpu_wait_done())
    {
        return;
    }

    /* 阶段2：连读 len 个字节 */
    DL_I2C_startControllerTransfer(MPU6050_I2C_INST, mpu6050_address,
                                   DL_I2C_CONTROLLER_DIRECTION_RX, len);
    for (i = 0; i < len; i++)
    {
        uint32_t start = uwTick;

        while (DL_I2C_isControllerRXFIFOEmpty(MPU6050_I2C_INST))
        {
            if ((uint32_t)(uwTick - start) > MPU_I2C_TIMEOUT_MS)
            {
                return;
            }
        }
        buf[i] = DL_I2C_receiveControllerData(MPU6050_I2C_INST);
    }

    mpu_wait_done();
}

/* ======================== 初始化 ======================== */

/**
 * @brief 初始化MPU6050
 * @return true 成功, false 器件没应答（检查接线、AD0/地址）
 */
bool mpu6050_init(void)
{
    uint8_t id  = 0;
    uint8_t pwr = 0;
    uint32_t retry;

    /* 复位芯片，复位完成后 PWR_MGMT_1 会自动变成 0x40(SLEEP=1) */
    mpu6050_write_reg(MPU_PWR_MGMT1_REG, 0x80);
    for (retry = 0; retry < 50u; retry++)
    {
        delay_ms(2);
        pwr = 0;
        mpu6050_read_reg(MPU_PWR_MGMT1_REG, &pwr);
        if (pwr == 0x40)
        {
            break;
        }
    }
    if (pwr != 0x40)
    {
        return false;
    }

    /* 器件ID自检：MPU6050 固定读回 0x68 */
    mpu6050_read_reg(MPU_DEVICE_ID_REG, &id);
    if (id != 0x68)
    {
        return false;
    }

    mpu6050_write_reg(MPU_PWR_MGMT1_REG, 0x00);   /* 唤醒 */
    mpu6050_write_reg(MPU_GYRO_CFG_REG, 0x18);    /* 角速度量程 ±2000°/s */
    mpu6050_write_reg(MPU_ACCEL_CFG_REG, 0x00);   /* 加速度量程 ±2g */
    mpu6050_write_reg(MPU_INT_EN_REG, 0x00);      /* 不用中断 */
    mpu6050_write_reg(MPU_USER_CTRL_REG, 0x00);   /* 不用 FIFO / DMP */
    mpu6050_write_reg(MPU_SAMPLE_RATE_REG, 0x01); /* 1kHz/(1+1) = 500Hz 采样率 */
    mpu6050_write_reg(MPU_CFG_REG, 0x01);         /* 内部低通 188Hz */
    mpu6050_write_reg(MPU_PWR_MGMT1_REG, 0x01);   /* 时钟源选 X 轴陀螺 PLL */
    mpu6050_write_reg(MPU_PWR_MGMT2_REG, 0x00);   /* 六轴全部打开 */

    return true;
}

/* ======================== 读数据 ======================== */

/**
 * @brief 读 Z 轴角速度的原始值
 * @return 原始 LSB（有符号），换算成 °/s 见 imu.c
 * @note  只读 0x47、0x48 两个寄存器；以后要用加速度/俯仰角，
 *        按参考工程那样从 0x3B 连读 14 字节即可（寄存器地址会自动加一）
 */
int16_t mpu6050_get_gyro_z(void)
{
    uint8_t buf[2];

    mpu6050_read_regs(MPU_GYRO_ZOUTH_REG, buf, 2);

    return (int16_t)((buf[0] << 8) | buf[1]);
}


