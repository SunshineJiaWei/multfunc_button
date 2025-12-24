#include <string.h>
#include "stm32f10x.h"

static void tim2_init(uint16_t period, uint16_t prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    memset(&TIM_InitStructure, 0, sizeof(TIM_InitStructure));
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = period - 1;
    TIM_InitStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    memset(&NVIC_InitStructure, 0, sizeof(NVIC_InitStructure));
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void timer_init(void)
{
    tim2_init(1000, 72);
}
