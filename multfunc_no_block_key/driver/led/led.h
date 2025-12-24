#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

typedef enum
{
    LED_OFF,
    LED_ON,
    LED_SLOW_BLINK,
    LED_FAST_BLINK,
    LED_DOT_BLINK,
    LED_MODE_MAX
} led_mode_t;

typedef struct
{
    uint32_t port;
    uint32_t pin;
    uint32_t tick;
    led_mode_t mode;
} led_t;

extern led_t led1;
extern led_t led2;


void led_init(void);
void led_tick(void);
void led_set_mode(led_t *led);

void led_on(led_t *led);

void led_off(led_t *led);

#endif /* __LED_H */
