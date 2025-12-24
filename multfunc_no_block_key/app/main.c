#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "OLED.h"
#include "timer.h"
#include "key.h"


static void board_lowlevel_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

static uint32_t sys_tick;
static uint16_t num1, num2;

int main(void)
{
    board_lowlevel_init();
    timer_init();
	key_init();
    OLED_Init();

    OLED_ShowString(0, 0, "systick:", OLED_8X16);
    OLED_ShowString(0, 16, "num1:", OLED_8X16);
    OLED_ShowString(0, 32, "num2:", OLED_8X16);

    while (1)
    {
        // 单击、双击、长按、组合键
        if (key_check(&key1, KEY_SINGLE))
        {
            num1 ++;
        }
        else if (key_check(&key1, KEY_DOUBLE))
        {
            num1 += 100;
        }
        else if (key_check(&key1, KEY_REPEAT))
        {
            num1 ++;
        }
        if (key_check(&key1, KEY_UP) && key_check(&key2, KEY_HOLD))
        {
            num2 ++;
        }

        OLED_ShowNum(64, 0, sys_tick, 5, OLED_8X16);
        OLED_ShowNum(40, 16, num1, 5, OLED_8X16);
        OLED_ShowNum(40, 32, num2, 5, OLED_8X16);
        OLED_Update();
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        sys_tick ++;
        key_tick();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
