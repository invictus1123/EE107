/*  File: gpio.cpp
 *  Written by Vinh Nguyen & LC Tao
 *  EE107, Sachin Katti, Fall 2016
 *  This file implements a basic GPIO driver.
 *  Used in conjunction with led.ino
 *  Functionality:
 *  -Toggle pin mode.
 *  -Digital write.
 */
#define togglePin 1<<5
#include "gpio.h"
#include "Arduino.h" //for uint8_t type
 //Board: Arduino Pro Mini (digital pins 0~19)
// commented out from gpio.ino
//void setup() {
//  gpio_set_mode(togglePin,1);
//}
//
//void loop() {
//  gpio_set_out(togglePin,1);
//  gpio_set_out(togglePin,0);
//  //For comparison
//  //digitalWrite(togglePin,HIGH);
//  //digitalWrite(togglePin,LOW);
//}

//Sets a digital pin to input or output. Input: A bitmask representing the pin on PORTB.
void gpio_set_mode(uint8_t pin, uint8_t out_or_in) { //pinMode()
      if(out_or_in) {
          DDRB |= pin;
      } else {
          DDRB &= ~pin; //write a macro for this operation instead?
      }
}
//Writes a logic 1 or 0 to the specified pin. Same as gpio_set_mode logically but with different registers being manipulated.
void gpio_set_out(uint8_t pin, uint8_t on_or_off) { //digitalWrite()
      if(on_or_off) {
          PORTB |= pin;
      } else {
          PORTB &= ~pin;
      }
}
//Flips the state of the pin.
void gpio_toggle_out(uint8_t pin) {
    PORTB ^= pin;
}
