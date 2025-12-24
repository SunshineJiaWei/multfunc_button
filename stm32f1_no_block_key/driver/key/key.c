#include <string.h>
#include "key.h"


#define KEY1_PORT   GPIOB
#define KEY1_PIN    GPIO_Pin_0
#define KEY2_PORT   GPIOB
#define KEY2_PIN    GPIO_Pin_10


key_t key1 = {(uint32_t)KEY1_PORT, KEY1_PIN, 0, 1, 0, 0};
key_t key2 = {(uint32_t)KEY2_PORT, KEY2_PIN, 0, 2, 0, 0};

static uint8_t g_key_num;

static uint8_t key_get_state(key_t *key)
{
    if (GPIO_ReadInputDataBit((GPIO_TypeDef *)key->port, key->pin) == 0)
    {
        return key->id;
    }
    return 0;
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

uint8_t key_get_num(void)
{
    uint8_t temp = g_key_num;
    g_key_num = 0;
    return temp;
}

static void key_ctrl(key_t *key)
{
    key->tick ++;

    if (key->tick >= 20)
    {
        key->tick = 0;

        // 按键松手逻辑
        key->prev_state = key->cur_state;
        key->cur_state = key_get_state(key);

        if (key->cur_state == 0 && key->prev_state != 0)
        {
            g_key_num = key->prev_state;
        }
    }
}

void key_tick(void)
{
    key_ctrl(&key1);
    key_ctrl(&key2);
}
