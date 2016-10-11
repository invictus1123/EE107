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
static const float CLOCK = 8E6;
static const int PRESCALER = 64;
static const float CNTR_FREQ = CLOCK/PRESCALER;
static const int CNTR_MAX = 256;
volatile uint8_t counter;
volatile uint16_t cycle_count;

ISR(TIMER0_COMPA_vect){
//    counter += 1;
      cycle_count++;
}

void setup() {
    Serial.begin(9600);
    timer_init();
    timer_set(1);
}

void loop() {
    Serial.println(cycle_count);
//Serial.println((uint16_t) CNTR_FREQ/1000);
//    Serial.println(TCNT0);
}

void timer_init(){
    TIMSK0 = B00000010;
    // Toggle OC0A on match, CTC mode
    TCCR0A = B01000010;
    // Select clock with 1024 prescaler
    TCCR0B = B00000011;
    // Reset timer counter
    TCNT0 = B00000000;
}

void timer_reset(){
    // Reset timer counter
    TCNT0 = B00000000;
}

void timer_set(uint16_t period){
    OCR0A = CNTR_FREQ*period/1000; //left as a float so that the math with CNTR_FREQ does not overflow an integer
//    counter_max = period;
}
