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
void timer_set(uint8_t period);

int counter_max;
volatile uint8_t counter;
ISR(TIMER0_COMPA_vect){
    counter += 1;
    if (counter == counter_max){
        Serial.println("Counter max reached");
        counter = 0;
    }
}

void setup() {
    Serial.begin(9600);
    timer_init();
    timer_set(100);
}

void loop() {
    //Serial.println(counter);
    Serial.println(TCNT0);
}

void timer_init(){
    // Toggle OC0A on match, CTC mode
    TCCR0A = B01000010;
    // Select clock without prescaling
    TCCR0B = B00000001;
    // Reset timer counter
    TCNT0 = B00000000;
}

void timer_reset(){
    // Reset timer counter
    TCNT0 = B00000000;
}

void timer_set(uint8_t period){
    OCR0A = (1 << 7);
    counter_max = period;
}

