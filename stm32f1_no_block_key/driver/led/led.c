#include <string.h>
#include "led.h"

#define LED1_PORT   GPIOA
#define LED1_PIN    GPIO_Pin_2
#define LED2_PORT   GPIOA
#define LED2_PIN    GPIO_Pin_4


led_t led1 = { (uint32_t)LED1_PORT, LED1_PIN, 0, LED_OFF };
led_t led2 = { (uint32_t)LED2_PORT, LED2_PIN, 0, LED_OFF };


static void led_instance_init(led_t *led)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = led->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)led->port, &GPIO_InitStructure);
    GPIO_SetBits((GPIO_TypeDef *)led->port, led->pin);
}

void led_on(led_t *led)
{
    GPIO_ResetBits((GPIO_TypeDef *)led->port, led->pin);
}

void led_off(led_t *led)
{
    GPIO_SetBits((GPIO_TypeDef *)led->port, led->pin);
}

static void led_ctrl(led_t *led)
{
    led->tick ++;
    switch(led->mode)
    {
        case LED_OFF:
        {
            led_off(led);
            break;
        }
        case LED_ON:
        {
            led_on(led);
            break;
        }
        case LED_SLOW_BLINK:
        {
            if (led->tick < 500){ led_on(led); }
            else{ led_off(led); }
            led->tick %= 1000;
            break;
        }
        case LED_FAST_BLINK:
        {
            if (led->tick < 50){ led_on(led); }
            else{ led_off(led); }
            led->tick %= 100;
            break;
        }
        case LED_DOT_BLINK:
        {
            if (led->tick < 100) { led_on(led); }
            else{ led_off(led); }
            led->tick %= 1000;
            break;
        }
    }
}

void led_init(void)
{
    led_instance_init(&led1);
    led_instance_init(&led2);
}

void led_set_mode(led_t *led)
{
    led->mode ++;
    led->mode %= LED_MODE_MAX;
}

void led_tick(void)
{
    led_ctrl(&led1);
    led_ctrl(&led2);
}
