/* File: main.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file contains the main program code
 * for blinking the onboard LED at two different
 * intervals.
 */
 #include "blink.h"
 #include "timer.h"
 #include "led.h"
 #include "gpio.h"
 
void setup() {
    blink_init();
    blink_set(200);
}
ISR(TIMER1_COMPA_vect) {
    blink_timer_ms_update();
}
void loop() {
  delay(10000);
  blink_set(500);
}
