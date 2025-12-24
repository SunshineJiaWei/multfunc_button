#include <string.h>
#include "key.h"


#define KEY1_PORT                   GPIOB
#define KEY1_PIN                    GPIO_Pin_0
#define KEY2_PORT                   GPIOB
#define KEY2_PIN                    GPIO_Pin_10

#define KEY_PRESSED                 1
#define KEY_UNPRESSED               0
#define KEY_LONG_PRESSED_TIME       1600
#define KEY_DOUBLE_PRESSED_TIME     200
#define KEY_REPEAT_TIME             100


key_t key1 = {(uint32_t)KEY1_PORT, KEY1_PIN, 0, 1, 0, 0, KEY_IDLE_STAGE, 0};
key_t key2 = {(uint32_t)KEY2_PORT, KEY2_PIN, 0, 2, 0, 0, KEY_IDLE_STAGE, 0};


static uint8_t key_get_state(key_t *key)
{
    if (GPIO_ReadInputDataBit((GPIO_TypeDef *)key->port, key->pin) == 0)
    {
        return KEY_PRESSED;
    }
    return KEY_UNPRESSED;
}

static void key_instance_init(key_t *key)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = key->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)key->port, &GPIO_InitStructure);
}

void key_init(void)
{
    key_instance_init(&key1);
    key_instance_init(&key2);
}

static void key_ctrl(key_t *key)
{
    key->tick ++;
    if (key->time > 0)
    {
        key->time --;
    }

    if (key->tick >= 20)
    {
        key->tick = 0;

        // 按键松手逻辑
        key->prev_state = key->cur_state;
        key->cur_state = key_get_state(key);

        if (key->cur_state == KEY_PRESSED)
        {
            key->flag |= KEY_HOLD;
        }
        else
        {
            key->flag &= ~KEY_HOLD;
        }
        if (key->cur_state == KEY_PRESSED && key->prev_state == KEY_UNPRESSED)
        {
            key->flag |= KEY_DOWN;
        }
        if (key->cur_state == KEY_UNPRESSED && key->prev_state == KEY_PRESSED)
        {
            key->flag |= KEY_UP;
        }

        switch (key->sm)
        {
            case KEY_IDLE_STAGE:
            {
                if (key->cur_state == KEY_PRESSED)
                {
                    key->time = KEY_LONG_PRESSED_TIME;
                    key->sm = KEY_PRESSED_STAGE;
                }
                break;
            }
            case KEY_PRESSED_STAGE:
            {
                if (key->cur_state == KEY_UNPRESSED)
                {
                    key->time = KEY_DOUBLE_PRESSED_TIME;
                    key->sm = KEY_RELEASED_STAGE;
                }
                else if (key->time == 0)
                {
                    key->flag |= KEY_LONG;
                    key->time = KEY_REPEAT_TIME;
                    key->sm = KEY_LONG_PRESSED_STAGE;
                }
                break;
            }
            case KEY_RELEASED_STAGE:
            {
                if (key->cur_state == KEY_PRESSED)
                {
                    key->flag |= KEY_DOUBLE;
                    key->sm = KEY_DOUBLE_PRESSED_STAGE;
                }
                else if (key->time == 0)
                {
                    key->flag |= KEY_SINGLE;
                    key->sm = KEY_IDLE_STAGE;
                }
                break;
            }
            case KEY_DOUBLE_PRESSED_STAGE:
            {
                if (key->cur_state == KEY_UNPRESSED)
                {
                    key->sm = KEY_IDLE_STAGE;
                }
                break;
            }
            case KEY_LONG_PRESSED_STAGE:
            {
                if (key->cur_state == KEY_UNPRESSED)
                {
                    key->sm = KEY_IDLE_STAGE;
                }
                else if (key->time == 0)
                {
                    key->flag |= KEY_REPEAT;
                    key->time = KEY_REPEAT_TIME;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

}

uint8_t key_check(key_t *key, key_event_t event)
{
    if (key->flag & event)
    {
        if (event != KEY_HOLD)
        {
            key->flag &= ~event;
        }
        return 1;
    }
    return 0;
}

void key_tick(void)
{
    key_ctrl(&key1);
    key_ctrl(&key2);
}
