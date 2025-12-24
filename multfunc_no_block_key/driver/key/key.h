#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"

typedef enum
{
    KEY_HOLD = 0x01,
    KEY_DOWN = 0x02,
    KEY_UP   = 0x04,
    KEY_SINGLE = 0x08,
    KEY_DOUBLE = 0x10,
    KEY_LONG = 0x20,
    KEY_REPEAT = 0x40
} key_event_t;

typedef enum
{
    KEY_IDLE_STAGE = 0,
    KEY_PRESSED_STAGE,
    KEY_RELEASED_STAGE,
    KEY_DOUBLE_PRESSED_STAGE,
    KEY_LONG_PRESSED_STAGE
} key_sm_t;

typedef struct
{
    uint32_t port;
    uint16_t pin;
    uint32_t tick;
    uint8_t id;
    uint8_t cur_state;
    uint8_t prev_state;
    key_sm_t sm;
    key_event_t flag;
    uint16_t time;
} key_t;


extern key_t key1;
extern key_t key2;

void key_tick(void);
void key_init(void);
uint8_t key_check(key_t *key, key_event_t event);

#endif /* __KEY_H */

