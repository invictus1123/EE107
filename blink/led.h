#ifndef LED_H
#define LED_H
#define LED 1<<5
#include "Arduino.h"
#include "gpio.h"

void led_init();
void led_on();
void led_off();
void led_toggle();
#endif // LED_H
