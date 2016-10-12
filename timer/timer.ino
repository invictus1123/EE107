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
volatile uint16_t cycle_count;
static const float PRESCALER = 8; //prescaler for the timer.
static const float CLOCK = 8E6; //Hz. 8MHz nominal (Pro Mini 8MHz), 16MHz for Arduino Uno.
static const float CNTR_MAX = 65536; //the maximum value the counter can take before overflowing. 16 bit = 2^16-1; 8 bit = 2^8-1.
static const float CNTR_FREQ = CLOCK/(PRESCALER);//the frequency in Hz of the peripheral clock/ how fast the hardware counter ticks
//macro for running an atomic operation. Saves the global interrupt flag (whether interrupts are on or off), turns interrupts off, runs the atomic code, then rewrites interrupt flag.

ISR(TIMER1_COMPA_vect){
      cycle_count++;
}

void setup() {
    Serial.begin(9600);
    timer_init();
    timer_set(1);
}

void loop() {
  Serial.println(cycle_count);
}

void timer_init(){    
    unsigned char sreg;
    sreg=SREG;
    cli();        
    TCCR1A = B01000000;
    // Use CTC mode (bits 5 and 4 from left set to 0 1) and set prescaler to 1024 (bits 3,2,and 1 from left set to 1 0 1)
    TCCR1B = B00001010;
  // Enable interrupts; Toggle an interrupt on match with value of OCR1A
    TIMSK1 |= B00000010;  
    sei();// Reset timer counter
    
    timer_reset();  
    //toggle compare output mode on
}

void timer_reset(){
  //do this atomically so that things don't explode if an interrupt triggers while we are accessing TCNT1.
    unsigned char sreg;
    sreg=SREG;
    cli();
    TCNT1 = 0; //16bit register
    SREG = sreg;
}

void timer_set(uint16_t period){
    OCR1A =  CNTR_FREQ*period/1000; //left as a float so that the math with CNTR_FREQ does not overflow an integer
}
