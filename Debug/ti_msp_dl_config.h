/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


/* Defines for TIMA0_C2 */
#define TIMA0_C2_INST                                                      TIMA0
#define TIMA0_C2_INST_IRQHandler                                TIMA0_IRQHandler
#define TIMA0_C2_INST_INT_IRQN                                  (TIMA0_INT_IRQn)
#define TIMA0_C2_INST_CLK_FREQ                                           8000000
/* GPIO defines for channel 2 */
#define GPIO_TIMA0_C2_C2_PORT                                              GPIOA
#define GPIO_TIMA0_C2_C2_PIN                                       DL_GPIO_PIN_7
#define GPIO_TIMA0_C2_C2_IOMUX                                   (IOMUX_PINCM14)
#define GPIO_TIMA0_C2_C2_IOMUX_FUNC                  IOMUX_PINCM14_PF_TIMA0_CCP2
#define GPIO_TIMA0_C2_C2_IDX                                 DL_TIMER_CC_2_INDEX

/* Defines for TIMA1_C1 */
#define TIMA1_C1_INST                                                      TIMA1
#define TIMA1_C1_INST_IRQHandler                                TIMA1_IRQHandler
#define TIMA1_C1_INST_INT_IRQN                                  (TIMA1_INT_IRQn)
#define TIMA1_C1_INST_CLK_FREQ                                           8000000
/* GPIO defines for channel 1 */
#define GPIO_TIMA1_C1_C1_PORT                                              GPIOB
#define GPIO_TIMA1_C1_C1_PIN                                       DL_GPIO_PIN_1
#define GPIO_TIMA1_C1_C1_IOMUX                                   (IOMUX_PINCM13)
#define GPIO_TIMA1_C1_C1_IOMUX_FUNC                  IOMUX_PINCM13_PF_TIMA1_CCP1
#define GPIO_TIMA1_C1_C1_IDX                                 DL_TIMER_CC_1_INDEX



#define CPUCLK_FREQ                                                     32000000



/* Defines for TIMGA */
#define TIMGA_INST                                                       (TIMG0)
#define TIMGA_INST_IRQHandler                                   TIMG0_IRQHandler
#define TIMGA_INST_INT_IRQN                                     (TIMG0_INT_IRQn)
#define TIMGA_INST_LOAD_VALUE                                           (65529U)
/* GPIO defines for channel 0 */
#define GPIO_TIMGA_C0_PORT                                                 GPIOA
#define GPIO_TIMGA_C0_PIN                                         DL_GPIO_PIN_23
#define GPIO_TIMGA_C0_IOMUX                                      (IOMUX_PINCM53)
#define GPIO_TIMGA_C0_IOMUX_FUNC                     IOMUX_PINCM53_PF_TIMG0_CCP0
/* GPIO defines for channel 1 */
#define GPIO_TIMGA_C1_PORT                                                 GPIOA
#define GPIO_TIMGA_C1_PIN                                         DL_GPIO_PIN_24
#define GPIO_TIMGA_C1_IOMUX                                      (IOMUX_PINCM54)
#define GPIO_TIMGA_C1_IOMUX_FUNC                     IOMUX_PINCM54_PF_TIMG0_CCP1

/* Defines for TIMGB */
#define TIMGB_INST                                                       (TIMG6)
#define TIMGB_INST_IRQHandler                                   TIMG6_IRQHandler
#define TIMGB_INST_INT_IRQN                                     (TIMG6_INT_IRQn)
#define TIMGB_INST_LOAD_VALUE                                           (65529U)
/* GPIO defines for channel 0 */
#define GPIO_TIMGB_C0_PORT                                                 GPIOA
#define GPIO_TIMGB_C0_PIN                                         DL_GPIO_PIN_21
#define GPIO_TIMGB_C0_IOMUX                                      (IOMUX_PINCM46)
#define GPIO_TIMGB_C0_IOMUX_FUNC                     IOMUX_PINCM46_PF_TIMG6_CCP0
/* GPIO defines for channel 1 */
#define GPIO_TIMGB_C1_PORT                                                 GPIOA
#define GPIO_TIMGB_C1_PIN                                         DL_GPIO_PIN_22
#define GPIO_TIMGB_C1_IOMUX                                      (IOMUX_PINCM47)
#define GPIO_TIMGB_C1_IOMUX_FUNC                     IOMUX_PINCM47_PF_TIMG6_CCP1





/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG7)
#define TIMER_0_INST_IRQHandler                                 TIMG7_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG7_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (65535U)




/* Defines for I2C_oled */
#define I2C_oled_INST                                                       I2C1
#define I2C_oled_INST_IRQHandler                                 I2C1_IRQHandler
#define I2C_oled_INST_INT_IRQN                                     I2C1_INT_IRQn
#define I2C_oled_BUS_SPEED_HZ                                             400000
#define I2C_oled_TARGET_OWN_ADDR                                            0x10
#define GPIO_I2C_oled_SDA_PORT                                             GPIOB
#define GPIO_I2C_oled_SDA_PIN                                      DL_GPIO_PIN_3
#define GPIO_I2C_oled_IOMUX_SDA                                  (IOMUX_PINCM16)
#define GPIO_I2C_oled_IOMUX_SDA_FUNC                   IOMUX_PINCM16_PF_I2C1_SDA
#define GPIO_I2C_oled_SCL_PORT                                             GPIOB
#define GPIO_I2C_oled_SCL_PIN                                      DL_GPIO_PIN_2
#define GPIO_I2C_oled_IOMUX_SCL                                  (IOMUX_PINCM15)
#define GPIO_I2C_oled_IOMUX_SCL_FUNC                   IOMUX_PINCM15_PF_I2C1_SCL

/* Defines for MPU6050_I2C */
#define MPU6050_I2C_INST                                                    I2C0
#define MPU6050_I2C_INST_IRQHandler                              I2C0_IRQHandler
#define MPU6050_I2C_INST_INT_IRQN                                  I2C0_INT_IRQn
#define MPU6050_I2C_BUS_SPEED_HZ                                          400000
#define MPU6050_I2C_TARGET_OWN_ADDR                                         0x10
#define GPIO_MPU6050_I2C_SDA_PORT                                          GPIOA
#define GPIO_MPU6050_I2C_SDA_PIN                                  DL_GPIO_PIN_28
#define GPIO_MPU6050_I2C_IOMUX_SDA                                (IOMUX_PINCM3)
#define GPIO_MPU6050_I2C_IOMUX_SDA_FUNC                 IOMUX_PINCM3_PF_I2C0_SDA
#define GPIO_MPU6050_I2C_SCL_PORT                                          GPIOA
#define GPIO_MPU6050_I2C_SCL_PIN                                  DL_GPIO_PIN_31
#define GPIO_MPU6050_I2C_IOMUX_SCL                                (IOMUX_PINCM6)
#define GPIO_MPU6050_I2C_IOMUX_SCL_FUNC                 IOMUX_PINCM6_PF_I2C0_SCL



/* Port definition for Pin Group BUZZER */
#define BUZZER_PORT                                                      (GPIOA)

/* Defines for PIN_0: GPIOA.12 with pinCMx 34 on package pin 5 */
#define BUZZER_PIN_0_PIN                                        (DL_GPIO_PIN_12)
#define BUZZER_PIN_0_IOMUX                                       (IOMUX_PINCM34)
/* Port definition for Pin Group KEY_MODE1 */
#define KEY_MODE1_PORT                                                   (GPIOB)

/* Defines for PIN_2: GPIOB.23 with pinCMx 51 on package pin 22 */
#define KEY_MODE1_PIN_2_PIN                                     (DL_GPIO_PIN_23)
#define KEY_MODE1_PIN_2_IOMUX                                    (IOMUX_PINCM51)
/* Port definition for Pin Group KEY_MODE2 */
#define KEY_MODE2_PORT                                                   (GPIOA)

/* Defines for PIN_1: GPIOA.29 with pinCMx 4 on package pin 36 */
#define KEY_MODE2_PIN_1_PIN                                     (DL_GPIO_PIN_29)
#define KEY_MODE2_PIN_1_IOMUX                                     (IOMUX_PINCM4)
/* Port definition for Pin Group KEY_MODE3 */
#define KEY_MODE3_PORT                                                   (GPIOA)

/* Defines for PIN_3: GPIOA.13 with pinCMx 35 on package pin 6 */
#define KEY_MODE3_PIN_3_PIN                                     (DL_GPIO_PIN_13)
#define KEY_MODE3_PIN_3_IOMUX                                    (IOMUX_PINCM35)
/* Port definition for Pin Group KEY_MODE4 */
#define KEY_MODE4_PORT                                                   (GPIOB)

/* Defines for PIN_4: GPIOB.6 with pinCMx 23 on package pin 58 */
#define KEY_MODE4_PIN_4_PIN                                      (DL_GPIO_PIN_6)
#define KEY_MODE4_PIN_4_IOMUX                                    (IOMUX_PINCM23)
/* Port definition for Pin Group TB6612_AIN2 */
#define TB6612_AIN2_PORT                                                 (GPIOA)

/* Defines for PIN_6: GPIOA.15 with pinCMx 37 on package pin 8 */
#define TB6612_AIN2_PIN_6_PIN                                   (DL_GPIO_PIN_15)
#define TB6612_AIN2_PIN_6_IOMUX                                  (IOMUX_PINCM37)
/* Port definition for Pin Group TB6612_AIN1 */
#define TB6612_AIN1_PORT                                                 (GPIOA)

/* Defines for PIN_5: GPIOA.27 with pinCMx 60 on package pin 31 */
#define TB6612_AIN1_PIN_5_PIN                                   (DL_GPIO_PIN_27)
#define TB6612_AIN1_PIN_5_IOMUX                                  (IOMUX_PINCM60)
/* Port definition for Pin Group TB6612_BIN1 */
#define TB6612_BIN1_PORT                                                 (GPIOA)

/* Defines for PIN_7: GPIOA.16 with pinCMx 38 on package pin 9 */
#define TB6612_BIN1_PIN_7_PIN                                   (DL_GPIO_PIN_16)
#define TB6612_BIN1_PIN_7_IOMUX                                  (IOMUX_PINCM38)
/* Port definition for Pin Group TB6612_BIN2 */
#define TB6612_BIN2_PORT                                                 (GPIOA)

/* Defines for PIN_8: GPIOA.17 with pinCMx 39 on package pin 10 */
#define TB6612_BIN2_PIN_8_PIN                                   (DL_GPIO_PIN_17)
#define TB6612_BIN2_PIN_8_IOMUX                                  (IOMUX_PINCM39)
/* Port definition for Pin Group B08 */
#define B08_PORT                                                         (GPIOB)

/* Defines for PIN_9: GPIOB.8 with pinCMx 25 on package pin 60 */
#define B08_PIN_9_PIN                                            (DL_GPIO_PIN_8)
#define B08_PIN_9_IOMUX                                          (IOMUX_PINCM25)
/* Port definition for Pin Group B27 */
#define B27_PORT                                                         (GPIOB)

/* Defines for PIN_10: GPIOB.27 with pinCMx 58 on package pin 29 */
#define B27_PIN_10_PIN                                          (DL_GPIO_PIN_27)
#define B27_PIN_10_IOMUX                                         (IOMUX_PINCM58)
/* Port definition for Pin Group B09 */
#define B09_PORT                                                         (GPIOB)

/* Defines for PIN_11: GPIOB.9 with pinCMx 26 on package pin 61 */
#define B09_PIN_11_PIN                                           (DL_GPIO_PIN_9)
#define B09_PIN_11_IOMUX                                         (IOMUX_PINCM26)
/* Port definition for Pin Group B26 */
#define B26_PORT                                                         (GPIOB)

/* Defines for PIN_12: GPIOB.26 with pinCMx 57 on package pin 28 */
#define B26_PIN_12_PIN                                          (DL_GPIO_PIN_26)
#define B26_PIN_12_IOMUX                                         (IOMUX_PINCM57)
/* Port definition for Pin Group B21 */
#define B21_PORT                                                         (GPIOB)

/* Defines for PIN_13: GPIOB.21 with pinCMx 49 on package pin 20 */
#define B21_PIN_13_PIN                                          (DL_GPIO_PIN_21)
#define B21_PIN_13_IOMUX                                         (IOMUX_PINCM49)
/* Port definition for Pin Group B19 */
#define B19_PORT                                                         (GPIOB)

/* Defines for PIN_14: GPIOB.19 with pinCMx 45 on package pin 16 */
#define B19_PIN_14_PIN                                          (DL_GPIO_PIN_19)
#define B19_PIN_14_IOMUX                                         (IOMUX_PINCM45)
/* Port definition for Pin Group B20 */
#define B20_PORT                                                         (GPIOB)

/* Defines for PIN_15: GPIOB.20 with pinCMx 48 on package pin 19 */
#define B20_PIN_15_PIN                                          (DL_GPIO_PIN_20)
#define B20_PIN_15_IOMUX                                         (IOMUX_PINCM48)
/* Port definition for Pin Group B22 */
#define B22_PORT                                                         (GPIOB)

/* Defines for PIN_16: GPIOB.22 with pinCMx 50 on package pin 21 */
#define B22_PIN_16_PIN                                          (DL_GPIO_PIN_22)
#define B22_PIN_16_IOMUX                                         (IOMUX_PINCM50)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_TIMA0_C2_init(void);
void SYSCFG_DL_TIMA1_C1_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMGA_init(void);
void SYSCFG_DL_TIMGB_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_I2C_oled_init(void);
void SYSCFG_DL_MPU6050_I2C_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
