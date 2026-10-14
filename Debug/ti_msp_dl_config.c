/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gTIMA0_C2Backup;
DL_TimerA_backupConfig gTIMA1_C1Backup;
DL_TimerG_backupConfig gTIMGBBackup;
DL_TimerG_backupConfig gTIMER_0Backup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_TIMA0_C2_init();
    SYSCFG_DL_TIMA1_C1_init();
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_TIMGA_init();
    SYSCFG_DL_TIMGB_init();
    SYSCFG_DL_TIMER_0_init();
    SYSCFG_DL_I2C_oled_init();
    SYSCFG_DL_MPU6050_I2C_init();
    /* Ensure backup structures have no valid state */
	gTIMA0_C2Backup.backupRdy 	= false;
	gTIMA1_C1Backup.backupRdy 	= false;
	gTIMGBBackup.backupRdy 	= false;
	gTIMER_0Backup.backupRdy 	= false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(TIMA0_C2_INST, &gTIMA0_C2Backup);
	retStatus &= DL_TimerA_saveConfiguration(TIMA1_C1_INST, &gTIMA1_C1Backup);
	retStatus &= DL_TimerG_saveConfiguration(TIMGB_INST, &gTIMGBBackup);
	retStatus &= DL_TimerG_saveConfiguration(TIMER_0_INST, &gTIMER_0Backup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(TIMA0_C2_INST, &gTIMA0_C2Backup, false);
	retStatus &= DL_TimerA_restoreConfiguration(TIMA1_C1_INST, &gTIMA1_C1Backup, false);
	retStatus &= DL_TimerG_restoreConfiguration(TIMGB_INST, &gTIMGBBackup, false);
	retStatus &= DL_TimerG_restoreConfiguration(TIMER_0_INST, &gTIMER_0Backup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(TIMA0_C2_INST);
    DL_TimerA_reset(TIMA1_C1_INST);
    DL_TimerG_reset(TIMGA_INST);
    DL_TimerG_reset(TIMGB_INST);
    DL_TimerG_reset(TIMER_0_INST);
    DL_I2C_reset(I2C_oled_INST);
    DL_I2C_reset(MPU6050_I2C_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(TIMA0_C2_INST);
    DL_TimerA_enablePower(TIMA1_C1_INST);
    DL_TimerG_enablePower(TIMGA_INST);
    DL_TimerG_enablePower(TIMGB_INST);
    DL_TimerG_enablePower(TIMER_0_INST);
    DL_I2C_enablePower(I2C_oled_INST);
    DL_I2C_enablePower(MPU6050_I2C_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_TIMA0_C2_C2_IOMUX,GPIO_TIMA0_C2_C2_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_TIMA0_C2_C2_PORT, GPIO_TIMA0_C2_C2_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_TIMA1_C1_C1_IOMUX,GPIO_TIMA1_C1_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_TIMA1_C1_C1_PORT, GPIO_TIMA1_C1_C1_PIN);

    DL_GPIO_initPeripheralInputFunction(GPIO_TIMGA_C0_IOMUX,GPIO_TIMGA_C0_IOMUX_FUNC);
    DL_GPIO_initPeripheralInputFunction(GPIO_TIMGA_C1_IOMUX,GPIO_TIMGA_C1_IOMUX_FUNC);
    DL_GPIO_initPeripheralInputFunction(GPIO_TIMGB_C0_IOMUX,GPIO_TIMGB_C0_IOMUX_FUNC);
    DL_GPIO_initPeripheralInputFunction(GPIO_TIMGB_C1_IOMUX,GPIO_TIMGB_C1_IOMUX_FUNC);

    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_oled_IOMUX_SDA,
        GPIO_I2C_oled_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_oled_IOMUX_SCL,
        GPIO_I2C_oled_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_I2C_oled_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_oled_IOMUX_SCL);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_MPU6050_I2C_IOMUX_SDA,
        GPIO_MPU6050_I2C_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_MPU6050_I2C_IOMUX_SCL,
        GPIO_MPU6050_I2C_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_MPU6050_I2C_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_MPU6050_I2C_IOMUX_SCL);

    DL_GPIO_initDigitalOutput(BUZZER_PIN_0_IOMUX);

    DL_GPIO_initDigitalInputFeatures(KEY_MODE1_PIN_2_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(KEY_MODE2_PIN_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(KEY_MODE3_PIN_3_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(KEY_MODE4_PIN_4_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(TB6612_AIN2_PIN_6_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_AIN1_PIN_5_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_BIN1_PIN_7_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_BIN2_PIN_8_IOMUX);

    DL_GPIO_initDigitalInputFeatures(B08_PIN_9_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B27_PIN_10_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B09_PIN_11_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B26_PIN_12_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B21_PIN_13_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B19_PIN_14_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B20_PIN_15_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(B22_PIN_16_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIOA, BUZZER_PIN_0_PIN |
		TB6612_AIN2_PIN_6_PIN |
		TB6612_BIN1_PIN_7_PIN);
    DL_GPIO_setPins(GPIOA, TB6612_AIN1_PIN_5_PIN |
		TB6612_BIN2_PIN_8_PIN);
    DL_GPIO_enableOutput(GPIOA, BUZZER_PIN_0_PIN |
		TB6612_AIN2_PIN_6_PIN |
		TB6612_AIN1_PIN_5_PIN |
		TB6612_BIN1_PIN_7_PIN |
		TB6612_BIN2_PIN_8_PIN);

}


/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   8000000 Hz = 32000000 Hz / (1 * (3 + 1))
 */
static const DL_TimerA_ClockConfig gTIMA0_C2ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 3U
};

static const DL_TimerA_PWMConfig gTIMA0_C2Config = {
    .pwmMode = DL_TIMER_PWM_MODE_CENTER_ALIGN,
    .period = 400,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMA0_C2_init(void) {

    DL_TimerA_setClockConfig(
        TIMA0_C2_INST, (DL_TimerA_ClockConfig *) &gTIMA0_C2ClockConfig);

    DL_TimerA_initPWMMode(
        TIMA0_C2_INST, (DL_TimerA_PWMConfig *) &gTIMA0_C2Config);

    // Set Counter control to the smallest CC index being used
    DL_TimerA_setCounterControl(TIMA0_C2_INST,DL_TIMER_CZC_CCCTL2_ZCOND,DL_TIMER_CAC_CCCTL2_ACOND,DL_TIMER_CLC_CCCTL2_LCOND);

    DL_TimerA_setCaptureCompareOutCtl(TIMA0_C2_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_2_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(TIMA0_C2_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_2_INDEX);
    DL_TimerA_setCaptureCompareValue(TIMA0_C2_INST, 200, DL_TIMER_CC_2_INDEX);

    DL_TimerA_enableClock(TIMA0_C2_INST);


    
    DL_TimerA_setCCPDirection(TIMA0_C2_INST , DL_TIMER_CC2_OUTPUT );


}
/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   8000000 Hz = 32000000 Hz / (1 * (3 + 1))
 */
static const DL_TimerA_ClockConfig gTIMA1_C1ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 3U
};

static const DL_TimerA_PWMConfig gTIMA1_C1Config = {
    .pwmMode = DL_TIMER_PWM_MODE_CENTER_ALIGN,
    .period = 400,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMA1_C1_init(void) {

    DL_TimerA_setClockConfig(
        TIMA1_C1_INST, (DL_TimerA_ClockConfig *) &gTIMA1_C1ClockConfig);

    DL_TimerA_initPWMMode(
        TIMA1_C1_INST, (DL_TimerA_PWMConfig *) &gTIMA1_C1Config);

    // Set Counter control to the smallest CC index being used
    DL_TimerA_setCounterControl(TIMA1_C1_INST,DL_TIMER_CZC_CCCTL1_ZCOND,DL_TIMER_CAC_CCCTL1_ACOND,DL_TIMER_CLC_CCCTL1_LCOND);

    DL_TimerA_setCaptureCompareOutCtl(TIMA1_C1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_1_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(TIMA1_C1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_1_INDEX);
    DL_TimerA_setCaptureCompareValue(TIMA1_C1_INST, 200, DL_TIMER_CC_1_INDEX);

    DL_TimerA_enableClock(TIMA1_C1_INST);


    
    DL_TimerA_setCCPDirection(TIMA1_C1_INST , DL_TIMER_CC1_OUTPUT );


}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 32000000 Hz / (1 * (31 + 1))
 */
static const DL_TimerG_ClockConfig gTIMGAClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 31U
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMGA_INST_LOAD_VALUE = (65.53 ms * 1000000 Hz) - 1
 */

SYSCONFIG_WEAK void SYSCFG_DL_TIMGA_init(void) {

    DL_TimerG_setClockConfig(TIMGA_INST,
        (DL_TimerG_ClockConfig *) &gTIMGAClockConfig);

    DL_TimerG_setLoadValue(TIMGA_INST,65529);

    DL_TimerG_setCounterMode(TIMGA_INST,DL_TIMER_COUNT_MODE_UP);

    DL_TimerG_setCounterRepeatMode(TIMGA_INST,DL_TIMER_REPEAT_MODE_ENABLED);

    DL_TimerG_setCounterValueAfterEnable(TIMGA_INST,DL_TIMER_COUNT_AFTER_EN_ZERO);

    DL_TimerG_setCaptureCompareCtl(TIMGA_INST,
    DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_NONE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_EDGE),
    DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareInput(TIMGA_INST,
        DL_TIMER_CC_INPUT_INV_NOINVERT,DL_TIMER_CC_IN_SEL_CCPX, DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareInputFilter(TIMGA_INST,
        DL_TIMER_CC_INPUT_FILT_CPV_CONSEC_PER,
        DL_TIMER_CC_INPUT_FILT_FP_PER_3,
        DL_TIMER_CC_0_INDEX
    );
    DL_Timer_enableCaptureCompareInputFilter(TIMGA_INST,
        DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareCtl(TIMGA_INST,
    DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_NONE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE),
    DL_TIMER_CC_1_INDEX);

    DL_TimerG_setCaptureCompareInput(TIMGA_INST,
        DL_TIMER_CC_INPUT_INV_NOINVERT,DL_TIMER_CC_IN_SEL_CCPX, DL_TIMER_CC_1_INDEX);

    DL_TimerG_setCaptureCompareInputFilter(TIMGA_INST,
        DL_TIMER_CC_INPUT_FILT_CPV_CONSEC_PER,
        DL_TIMER_CC_INPUT_FILT_FP_PER_3,
        DL_TIMER_CC_1_INDEX
    );
    DL_Timer_enableCaptureCompareInputFilter(TIMGA_INST,
        DL_TIMER_CC_1_INDEX);


    DL_TimerG_setCounterControl(TIMGA_INST,
        DL_TIMER_CZC_CCCTL0_ZCOND,
        DL_TIMER_CAC_CCCTL0_ACOND,
        DL_TIMER_CLC_CCCTL0_LCOND
    );

    DL_TimerG_startCounter(TIMGA_INST);

    DL_TimerG_enableInterrupt(TIMGA_INST , DL_TIMERG_INTERRUPT_CC0_UP_EVENT |
		DL_TIMERG_INTERRUPT_CC1_UP_EVENT);

    DL_TimerG_enableClock(TIMGA_INST);

}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 32000000 Hz / (1 * (31 + 1))
 */
static const DL_TimerG_ClockConfig gTIMGBClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 31U
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMGB_INST_LOAD_VALUE = (65.53 ms * 1000000 Hz) - 1
 */

SYSCONFIG_WEAK void SYSCFG_DL_TIMGB_init(void) {

    DL_TimerG_setClockConfig(TIMGB_INST,
        (DL_TimerG_ClockConfig *) &gTIMGBClockConfig);

    DL_TimerG_setLoadValue(TIMGB_INST,65529);

    DL_TimerG_setCounterMode(TIMGB_INST,DL_TIMER_COUNT_MODE_UP);

    DL_TimerG_setCounterRepeatMode(TIMGB_INST,DL_TIMER_REPEAT_MODE_ENABLED);

    DL_TimerG_setCounterValueAfterEnable(TIMGB_INST,DL_TIMER_COUNT_AFTER_EN_ZERO);

    DL_TimerG_setCaptureCompareCtl(TIMGB_INST,
    DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_NONE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_EDGE),
    DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareInput(TIMGB_INST,
        DL_TIMER_CC_INPUT_INV_NOINVERT,DL_TIMER_CC_IN_SEL_CCPX, DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareInputFilter(TIMGB_INST,
        DL_TIMER_CC_INPUT_FILT_CPV_CONSEC_PER,
        DL_TIMER_CC_INPUT_FILT_FP_PER_3,
        DL_TIMER_CC_0_INDEX
    );
    DL_Timer_enableCaptureCompareInputFilter(TIMGB_INST,
        DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareCtl(TIMGB_INST,
    DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_NONE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE),
    DL_TIMER_CC_1_INDEX);

    DL_TimerG_setCaptureCompareInput(TIMGB_INST,
        DL_TIMER_CC_INPUT_INV_NOINVERT,DL_TIMER_CC_IN_SEL_CCPX, DL_TIMER_CC_1_INDEX);

    DL_TimerG_setCaptureCompareInputFilter(TIMGB_INST,
        DL_TIMER_CC_INPUT_FILT_CPV_CONSEC_PER,
        DL_TIMER_CC_INPUT_FILT_FP_PER_3,
        DL_TIMER_CC_1_INDEX
    );
    DL_Timer_enableCaptureCompareInputFilter(TIMGB_INST,
        DL_TIMER_CC_1_INDEX);


    DL_TimerG_setCounterControl(TIMGB_INST,
        DL_TIMER_CZC_CCCTL0_ZCOND,
        DL_TIMER_CAC_CCCTL0_ACOND,
        DL_TIMER_CLC_CCCTL0_LCOND
    );

    DL_TimerG_startCounter(TIMGB_INST);

    DL_TimerG_enableInterrupt(TIMGB_INST , DL_TIMERG_INTERRUPT_CC0_UP_EVENT |
		DL_TIMERG_INTERRUPT_CC1_UP_EVENT);

    DL_TimerG_enableClock(TIMGB_INST);

}


/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 32000000 Hz / (1 * (31 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 31U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (65.536 ms * 1000000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_0_init(void) {

    DL_TimerG_setClockConfig(TIMER_0_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_0ClockConfig);

    DL_TimerG_initTimerMode(TIMER_0_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_0TimerConfig);
    DL_TimerG_enableClock(TIMER_0_INST);





}


static const DL_I2C_ClockConfig gI2C_oledClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

SYSCONFIG_WEAK void SYSCFG_DL_I2C_oled_init(void) {

    DL_I2C_setClockConfig(I2C_oled_INST,
        (DL_I2C_ClockConfig *) &gI2C_oledClockConfig);
    DL_I2C_setAnalogGlitchFilterPulseWidth(I2C_oled_INST,
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_50NS);
    DL_I2C_enableAnalogGlitchFilter(I2C_oled_INST);

    /* Configure Controller Mode */
    DL_I2C_resetControllerTransfer(I2C_oled_INST);
    /* Set frequency to 400000 Hz*/
    DL_I2C_setTimerPeriod(I2C_oled_INST, 7);
    DL_I2C_setControllerTXFIFOThreshold(I2C_oled_INST, DL_I2C_TX_FIFO_LEVEL_EMPTY);
    DL_I2C_setControllerRXFIFOThreshold(I2C_oled_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_enableControllerClockStretching(I2C_oled_INST);

    /* Configure Target Mode */
    DL_I2C_setTargetOwnAddress(I2C_oled_INST, I2C_oled_TARGET_OWN_ADDR);
    DL_I2C_setTargetTXFIFOThreshold(I2C_oled_INST, DL_I2C_TX_FIFO_LEVEL_EMPTY);
    DL_I2C_setTargetRXFIFOThreshold(I2C_oled_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);

    DL_I2C_enableTargetClockStretching(I2C_oled_INST);

    /* Workaround for errata I2C_ERR_04 */
    DL_I2C_disableTargetWakeup(I2C_oled_INST);

    /* Enable module */
    DL_I2C_enableController(I2C_oled_INST);
    DL_I2C_enableTarget(I2C_oled_INST);


}
static const DL_I2C_ClockConfig gMPU6050_I2CClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

SYSCONFIG_WEAK void SYSCFG_DL_MPU6050_I2C_init(void) {

    DL_I2C_setClockConfig(MPU6050_I2C_INST,
        (DL_I2C_ClockConfig *) &gMPU6050_I2CClockConfig);
    DL_I2C_setAnalogGlitchFilterPulseWidth(MPU6050_I2C_INST,
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_50NS);
    DL_I2C_enableAnalogGlitchFilter(MPU6050_I2C_INST);

    /* Configure Controller Mode */
    DL_I2C_resetControllerTransfer(MPU6050_I2C_INST);
    /* Set frequency to 400000 Hz*/
    DL_I2C_setTimerPeriod(MPU6050_I2C_INST, 7);
    DL_I2C_setControllerTXFIFOThreshold(MPU6050_I2C_INST, DL_I2C_TX_FIFO_LEVEL_EMPTY);
    DL_I2C_setControllerRXFIFOThreshold(MPU6050_I2C_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_enableControllerClockStretching(MPU6050_I2C_INST);

    /* Configure Target Mode */
    DL_I2C_setTargetOwnAddress(MPU6050_I2C_INST, MPU6050_I2C_TARGET_OWN_ADDR);
    DL_I2C_setTargetTXFIFOThreshold(MPU6050_I2C_INST, DL_I2C_TX_FIFO_LEVEL_EMPTY);
    DL_I2C_setTargetRXFIFOThreshold(MPU6050_I2C_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);

    DL_I2C_enableTargetClockStretching(MPU6050_I2C_INST);

    /* Workaround for errata I2C_ERR_04 */
    DL_I2C_disableTargetWakeup(MPU6050_I2C_INST);

    /* Enable module */
    DL_I2C_enableController(MPU6050_I2C_INST);
    DL_I2C_enableTarget(MPU6050_I2C_INST);


}

