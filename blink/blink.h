#ifndef BLINK_H
#define BLINK_H

#include "gpio.h"
#include "led.h"
#include "timer.h"
#include "Arduino.h"

void blink_init();
void blink_set(uint16_t interval_ms);
void blink_timer_ms_update();
#endif // BLINK_H
