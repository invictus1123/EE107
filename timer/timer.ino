/*  File: timer.ino
 *  Written by Vinh Nguyen & LC Tao
 *  EE107, Sachin Katti, Fall 2016
 *  This file implements an interface for timer interrupts.
 *  Functionality:
 *  - Timer initialization
 *  - Timer reset
 *  - Timer set period
 *  - Timer interrupt handler
 */
void timer_init();
void timer_reset();
void timer_set(uint16_t period);

int counter_max;
volatile uint8_t counter;
volatile uint8_t cycle_count;

ISR(TIMER0_COMPA_vect){
//    counter += 1;
      cycle_count++;
}

void setup() {
    Serial.begin(9600);
    timer_init();
    timer_set(100);
}

void loop() {
    Serial.println(cycle_count);
//    Serial.println(TCNT0);
}

void timer_init(){
    TIMSK0 = B00000010;
    // Toggle OC0A on match, CTC mode
    TCCR0A = B01000010;
    // Select clock with 1024 prescaler
    TCCR0B = B00000101;
    // Reset timer counter
    TCNT0 = B00000000;
}

void timer_reset(){
    // Reset timer counter
    TCNT0 = B00000000;
}

void timer_set(uint16_t period){
    OCR0A = (period*10/1.28);
//    counter_max = period;
}
