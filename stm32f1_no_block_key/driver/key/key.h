#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"


typedef struct
{
    uint32_t port;
    uint16_t pin;
    uint32_t tick;
    uint8_t id;
    uint8_t cur_state;
    uint8_t prev_state;
} key_t;

extern key_t key1;
extern key_t key2;

void key_tick(void);
uint8_t key_get_num(void);
void key_init(void);

#endif /* __KEY_H */

