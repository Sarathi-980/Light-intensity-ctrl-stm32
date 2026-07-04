#ifndef LED_H
#define LED_H
#include "system.h"
#include "timer.h"

void led_off();
void led_on();

void led_indicate_state(system_result_t state);
void update_led_from_daylight(timer_id_t timer_id, timer_channel_t channel);

#endif