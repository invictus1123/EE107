/*  File: gpio.h
 *  Written by Vinh Nguyen & LC Tao
 *  EE107, Sachin Katti, Fall 2016
 *  This file interfaces the gpio library with led.ino.
 *  Functionality:
 *  -Toggle pin mode.
 *  -Digital write.
 */
#ifndef GPIO_H
#define GPIO_H
#include "Arduino.h"
void gpio_set_mode(uint8_t pin, uint8_t out_or_in);
void gpio_set_out(uint8_t pin, uint8_t on_or_off);
void gpio_toggle_out(uint8_t pin);
#endif // GPIO_H
