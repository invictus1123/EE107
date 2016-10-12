#ifndef BLINK_H
#define BLINK_H
#include "gpio.h"
#include "led.h"
#include "timer.h"

void blink_init();
void blink_set(uint16_t interval_ms);
void blink_ms_timer_update();
#endif // BLINK_H
