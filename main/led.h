#ifndef LED_H
#define LED_H
#include "Arduino.h"
#include "gpio.h"
#define LED 1<<5
void led_init();
void led_on();
void led_off();
void led_toggle();
#endif // LED_H
