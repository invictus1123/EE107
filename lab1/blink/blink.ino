/* File: blink.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file implements a simple library for blinking the onboard LED
 * at a certain interval.
 * Functionality:
 * -Initialize LED and timer.
 * -Set or change a period for blinking the LED. max period is 524ms, min period 8us. (Prescaler 64, 16-bit timer).
 */
#include "gpio.h"
#include "led.h"
#include "timer.h"

void blink_init();
void blink_set(uint16_t interval_ms);
void blink_timer_ms_update();

void setup() {
    blink_init();
    blink_set(1);
}
ISR(TIMER1_COMPA_vect) {
    blink_timer_ms_update();
}
void loop() {
}
void blink_init() {
    led_init();
    timer_init();
}
void blink_set(uint16_t interval_ms) {
    timer_set(interval_ms);
}

void blink_timer_ms_update() {
    led_toggle();
}
