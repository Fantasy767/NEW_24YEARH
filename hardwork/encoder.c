#include "encoder.h"
#include "ti_msp_dl_config.h"
#include "Delay.h"

/*
 * 两路 TIMG 输入捕获, 时基均为: BUSCLK 32MHz / prescale 32 = 1MHz, 1 tick = 1us
 * 计数器自由运行递增 (0 ~ 65529), 不因捕获清零, CC0/CC1 各自记录边沿时刻
 */
#define ENCODER_TICK_HZ     1000000u
/* 超过该时间没有 A 相边沿, 认为电机已停转 */
#define ENCODER_TIMEOUT_MS  100u

typedef struct {
    GPTIMER_Regs     *inst;
    volatile int32_t  count;     /* 累计计数, 每个 A 相边沿按方向 ±1 */
    volatile int8_t   dir;       /* 当前方向 */
    volatile uint32_t period;    /* A 相相邻边沿间隔 (us) */
    volatile uint32_t edge_tick; /* 最近一次 A 相边沿的系统时刻 (ms) */
    volatile uint16_t last_a;    /* 最近一次 A 相捕获值 */
    volatile uint8_t  a_rising;  /* 最近一次 A 相边沿是否为上升沿 */
} encoder_ctx_t;

static encoder_ctx_t s_enc[ENCODER_COUNT] = {
    { .inst = TIMGA_INST },   /* ENCODER_1 : TIMG0, A=PA23, B=PA24 */
    { .inst = TIMGB_INST },   /* ENCODER_2 : TIMG6, A=PA21, B=PA22 */
};

void encoder_init(void)
{
    uint32_t i;

    for (i = 0u; i < (uint32_t) ENCODER_COUNT; i++) {
        encoder_ctx_t *e = &s_enc[i];

        e->count     = 0;
        e->dir       = 1;
        e->period    = 0u;
        e->last_a    = (uint16_t) DL_TimerG_getCaptureCompareValue(
                           e->inst, DL_TIMER_CC_0_INDEX);
        /* 边沿交替出现: 每捕获一次取反, 0 起始则第 1 次(上升沿)得 1 */
        e->a_rising  = 0u;
        e->edge_tick = uwTick;
    }

    NVIC_ClearPendingIRQ(TIMGA_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMGA_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(TIMGB_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMGB_INST_INT_IRQN);
}

static void encoder_isr(encoder_ctx_t *e)
{
    switch (DL_TimerG_getPendingInterrupt(e->inst)) {
        case DL_TIMER_IIDX_CC0_UP: {
            /* A 相边沿(上升+下降都捕获), 用于测速和计数 */
            uint16_t now = (uint16_t) DL_TimerG_getCaptureCompareValue(
                               e->inst, DL_TIMER_CC_0_INDEX);

            e->period    = (uint32_t) (uint16_t) (now - e->last_a);
            e->last_a    = now;
            e->a_rising ^= 1u;   /* 方波边沿必然交替, 翻转即得极性 */
            e->edge_tick = uwTick;
            e->count    += e->dir;
            break;
        }
        case DL_TIMER_IIDX_CC1_UP:
            /* B 相上升沿判向: 正转时它紧跟 A 相上升沿之后;
               反转时它出现在 A 相下降沿之后 */
            e->dir = e->a_rising ? 1 : -1;
            break;
        default:
            break;
    }
}

void TIMGA_INST_IRQHandler(void)
{
    encoder_isr(&s_enc[ENCODER_1]);
}

void TIMGB_INST_IRQHandler(void)
{
    encoder_isr(&s_enc[ENCODER_2]);
}

static encoder_ctx_t *encoder_ctx(encoder_id_t id)
{
    return &s_enc[(id == ENCODER_2) ? (uint32_t) ENCODER_2 : 0u];
}

static uint8_t encoder_is_stale(const encoder_ctx_t *e)
{
    return (uint8_t) ((uwTick - e->edge_tick) >= ENCODER_TIMEOUT_MS);
}

int32_t encoder_get_count(encoder_id_t id)
{
    return encoder_ctx(id)->count;
}

void encoder_reset_count(encoder_id_t id)
{
    encoder_ctx(id)->count = 0;
}

int8_t encoder_get_direction(encoder_id_t id)
{
    encoder_ctx_t *e = encoder_ctx(id);

    return encoder_is_stale(e) ? 0 : e->dir;
}

uint32_t encoder_get_period_us(encoder_id_t id)
{
    encoder_ctx_t *e = encoder_ctx(id);

    return encoder_is_stale(e) ? 0u : e->period;
}

int32_t encoder_get_speed_cps(encoder_id_t id)
{
    encoder_ctx_t *e      = encoder_ctx(id);
    uint32_t       period = encoder_get_period_us(id);

    if (period == 0u) {
        return 0;
    }
    /* A 相边沿每秒出现 ENCODER_TICK_HZ/period 次, 每次计数 +1 */
    return (int32_t) (ENCODER_TICK_HZ / period) * (int32_t) e->dir;
}

int32_t encoder_get_rpm(encoder_id_t id, uint16_t ppr, uint16_t ratio)
{
    uint32_t counts_per_rev = (uint32_t) ppr * 2u * (uint32_t) ratio;

    if (counts_per_rev == 0u) {
        return 0;
    }
    return (int32_t) (((int32_t) encoder_get_speed_cps(id) * 60) / (int32_t) counts_per_rev);
}

int32_t encoder_get_s(encoder_id_t id, uint16_t ppr, uint16_t ratio)
{
    uint32_t counts_per_rev = (uint32_t) ppr * 2u * (uint32_t) ratio;

    if (counts_per_rev == 0u) {
        return 0;
    }
    /* 每秒圈数 = 计数/秒 ÷ 每圈计数 */
    return encoder_get_speed_cps(id) / (int32_t) counts_per_rev;
}
