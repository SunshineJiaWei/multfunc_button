#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "OLED.h"
#include "timer.h"
#include "key.h"
#include "led.h"
#include "Delay.h"


static void board_lowlevel_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

static uint32_t sys_tick = 0;

int main(void)
{
    board_lowlevel_init();
    timer_init();
    led_init();
	key_init();
    OLED_Init();

    uint8_t key_num = 0;

    while (1)
    {
        key_num = key_get_num();
        switch (key_num)
        {
            case 1:
            {
                led_set_mode(&led1);
                break;
            }
            case 2:
            {
                led_set_mode(&led2);
                break;
            }
            default:
            {
                break;
            }
        }

        OLED_ShowString(0, 0, "systick:", OLED_8X16);
        OLED_ShowNum(64, 0, sys_tick, 5, OLED_8X16);
        OLED_ShowString(0, 16, "led1 mode:", OLED_8X16);
        OLED_ShowNum(80, 16, led1.mode, 1, OLED_8X16);
        OLED_ShowString(0, 32, "led2 mode:", OLED_8X16);
        OLED_ShowNum(80, 32, led2.mode, 1, OLED_8X16);
        OLED_Update();
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        sys_tick ++;
        key_tick();
        led_tick();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
