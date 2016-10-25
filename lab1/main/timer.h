#ifndef TIMER_H
#define TIMER_H
#include "Arduino.h"
void timer_init();
void timer_reset();
void timer_set(uint16_t period);
static const float PRESCALER = 64; //prescaler for the timer.
static const float CLOCK = 8E6; //Hz. 8MHz nominal (Pro Mini 8MHz)
static const float CNTR_FREQ = CLOCK/(PRESCALER);//the frequency in Hz of the peripheral clock/ how fast the hardware counter ticks
#endif // TIMER_H
