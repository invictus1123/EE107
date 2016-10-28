/*  File: led.ino
 *  Written by Vinh Nguyen & LC Tao
 *  EE107, Sachin Katti, Fall 2016
 *  This file implements an interface for toggling LEDs.
 *  Functionality:
 *  -Turn LED on or off.
 *  -Toggle LED state.
 */

#define LED 1<<5
#include "gpio.h"

extern void led_init();
extern void led_on();
extern void led_off();
extern void led_toggle();

void led_init() {
    gpio_set_mode(LED,1);
    led_off(); // TODO: test if necessary. Setting to output may or may not write HIGH to the pin. pinMode() seems to do it, and we are warned in the spec.
}

void led_off() {
    gpio_set_out(LED,0);
}

void led_on() {
    gpio_set_out(LED,1);
}

void led_toggle() {
    gpio_toggle_out(LED);
}
