/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "ti_msp_dl_config.h"
#include "BUZZER.h"
#include "key.h"
#include "encoder.h"
#include "track.h"
#include "tb6612.h"
#include "imu.h"
#include "PID.h"
#include "main.h"
volatile uint32_t uwTick = 0;
uint16_t current_count_last=0;
uint8_t PID_Control_Flag=0;
uint8_t mode = 0;      /* 车辆当前状态: 按键切换并锁存 */
#define MODE_ONE 1
#define MODE_TWO 2
#define MODE_THREE 3
#define MODE_FOUR 4

void SysTick_Handler(void)
{
    uwTick++;
}

void search(void)
{
    /* TIMG7 时基 32MHz/32 = 1MHz, 1 个计数 = 1us;
       计数器只有 16 位(每 65.536ms 回绕一次), 用 uint16 减法才能算出正确间隔 */
    uint16_t current_count = (uint16_t) DL_TimerG_getTimerCount(TIMER_0_INST);
    if((uint16_t)(current_count-current_count_last)>TRACK_PERIOD_US)
    {
        current_count_last=current_count;
        PID_Control_Flag=1;
    }
}


int main(void)
{
    SYSCFG_DL_init();
    DL_SYSTICK_config(32000);   /* 32MHz / 1000 = 1ms 一次中断 */
    tb6612_init();              /* 启动 PWM 计数器并设置 TB6612 方向脚, 不调用电机不会转 */
    encoder_init();             /* TIMG0 双通道输入捕获: A相=PA23(CCP0), B相=PA24(CCP1) */
   /* 代码OLED 
    OLED_Init();
    OLED_NewFrame();
    OLED_PrintString(0, 0, "abc", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();*/
    (void) imu_init();          /* MPU6050 初始化 + 零偏校准; 车必须静止, 失败则偏航角不可信 */

    while (1) 
    {
       /* 偏航角要在所有模式下持续更新, 不能只在某一个模式里调用 */
       if (imu_update_ready()) 
       {imu_update();}

       /* 提示音计时: 到点自动关, 这样中途提示也不会卡住电机控制 */
       beep_task();

       /* key() 只在"新按下"时返回 1~4, 其余返回 KEY_NONE(0); 若写成 mode = key(),
          会被 0 覆盖导致模式只生效一拍, 所以只在有按键事件时更新 */
       uint16_t k = key();
       if (k != KEY_NONE)
       {
           mode = (uint8_t) k;          /* 按键 = 切换状态, 并锁存 */
           if (mode == MODE_ONE)
           {
               /* 进入状态1: 重置航向环/速度环/停车状态 */
               imu_reset_yaw();
               pid_one.target   = 0.0f;
               pid_one.last_err = 0.0f;
               track_index  = 0;
               track_stopped = 0;
               pid_motor_left.integral  = 0.0f;
               pid_motor_left.last_err  = 0.0f;
               pid_motor_right.integral = 0.0f;
               pid_motor_right.last_err = 0.0f;
           }
           else if (mode == MODE_TWO)
           {
               /* 进入状态2: 一切都从头开始 */
               m2_state  = M2_STRAIGHT;
               m2_round  = 0;
               m2_seen   = 0;
               m2_lost   = 0;
               m2_finish = 0;
               m2_tick   = uwTick;

               mode_two_clear_pid();    /* 以当前朝向作为直行方向 */
               track_index   = 0;
               track_stopped = 0;
           }

           else if (mode == MODE_THREE)
           {
               /* 进入状态3: 一切都从头开始 */
               m3_state  = M3_STRAIGHT;
               m3_round  = 0;
               m3_seen   = 0;
               m3_lost   = 0;
               m3_finish = 0;
               m3_tick   = uwTick;

               mode_three_clear_pid();    /* 以当前朝向作为直行方向 */
               track_index   = 0;
               track_stopped = 0;
           }
       }

       switch(mode)
       {
           case MODE_ONE:
           {
               static uint32_t stop_tick = 0;

               NO_track_mode(&pid_one);   /* 无黑线走直线(内部自带 10ms 节拍) */

               /* 停车检测也要 10ms 一次, 否则 track_index 几微秒就冲到阈值 */
               if (!track_stopped &&
                   (uint32_t)(uwTick - stop_tick) >= CONTROL_PERIOD_MS)
               {
                   stop_tick = uwTick;
                   if (track_on_or_off())
                   {
                       track_index++;
                       if (track_index > MODE1_STOP_LINE_COUNT)
                       {
                           track_stopped = 1;
                           left_motor_speed(0);
                           right_motor_speed(0);
                           beep_request(BEEP_STOP_MS);   /* 到 B 点停车: 声光提示 */
                       }
                   }
                   else
                   {
                       track_index = 0;
                   }
               }
               break;
           }
           case MODE_TWO:
               mode_two_task();     /* 直行 -> 见线循迹, 循环两次 */
               break;
           case MODE_THREE:
               mode_three_task();   /* 直行 -> 见线循迹, 循环两次 */
               break;
           case MODE_FOUR:

               break;
           default:
               break;
       }
        
        
    }
}
