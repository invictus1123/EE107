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
volatile uint8_t counter;
volatile uint16_t cycle_count;
#define PRESCALER 8 //prescaler for the timer.
#define CLOCK 16E6 //Hz. 8MHz nominal (Pro Mini 8MHz), 16MHz for Arduino Uno debugging.
#define CNTR_MAX 65535 //the maximum value the counter can take before overflowing. 16 bit = 2^16-1; 8 bit = 2^8-1.
static const float CNTR_FREQ = CLOCK/(PRESCALER * CNTR_MAX);//the frequency in Hz of the counter overflows (how many times it goes from 0 to MAX per second).

//macro for running an atomic operation. Saves the global interrupt flag (whether interrupts are on or off), turns interrupts off, runs the atomic code, then rewrites interrupt flag.

ISR(TIMER1_COMPA_vect){
      cycle_count++;
}

void setup() {
    Serial.begin(9600);
    timer_set(1);
    timer_init();
}

void loop() {
  Serial.println(cycle_count);
}

void timer_init(){         
    // Reset timer counter
    timer_reset();  
    //toggle compare output mode on
    TCCR1A |= B11000000;
    // Use CTC mode (bits 5 and 4 from left set to 0 1) and set prescaler to 8 (bits 3,2,and 1 from left set to 0 1 0)
    TCCR1B |= B00001010;
  // Enable interrupts; Toggle an interrupt on match with value of OCR1A
    TIMSK1 |= B00000010;  
}

void timer_reset(){
  //do this atomically so that things don't explode if an interrupt triggers while we are accessing TCNT1.
    unsigned char sreg;
    sreg=SREG;
    cli();
    TCNT1 = 0x0000; //16bit register
    SREG = sreg;
}

void timer_set(uint16_t period){
    OCR1A =  (uint16_t)(CNTR_MAX*period)/CNTR_FREQ ;
}
