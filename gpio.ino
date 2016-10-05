/*  File: gpio.ino
 *  Written by Vinh Nguyen & LC Tao
 *  EE107, Sachin Katti, Fall 2016
 *  This file implements a basic GPIO driver.
 *  Functionality: 
 *  -Toggle pin mode.
 *  -Digital write.
 */
 #define togglePin 3
 //Board: Arduino Pro Mini (digital pins 0~19)
void gpio_set_mode(uint8_t pin, uint8_t out_or_in);
void gpio_set_out(uint8_t pin, uint8_t on_or_off);
void gpio_toggle_out(uint8_t pin); 

void setup() {
  gpio_set_mode(togglePin,1);
}

void loop() {
  gpio_set_out(togglePin,1);
  gpio_set_out(togglePin,0);
  //For comparison
  //digitalWrite(togglePin,HIGH);
  //digitalWrite(togglePin,LOW);
}
//Sets a digital pin to input or output.
void gpio_set_mode(uint8_t pin, uint8_t out_or_in) { //pinMode()
    if(pin < 8) { //digital pins 0-7 map to port D
        if(out_or_in) {
            DDRD |= (1<<pin);//write 1 in the position of that pin in the corresponding Data Direction Register -> set that pin to output
        } else {
            DDRD &= ~(1<<pin); // write 0 in the position of the pin in the corresponding Data Direction Register -> set to input
        }
    } else if(pin < 14) {//digital pins 9-13 map to port B
        if(out_or_in) {
            DDRB |= ( 1<<(pin-8) );      
        } else {
            DDRB &= ( 1<<(pin-8) ); //write a macro for this operation instead?
        }
    } else if(pin < 20){ //analog pins A0 to A5 (numbered 14 to 19?) map to port C
        if(out_or_in) {
            DDRC |= ( B00111111 & ( 1<<(pin-14) ) );  //don't touch the 7th and 8th bit if passed
        } else {
            DDRC &= ~( B00111111 & ( 1<<(pin-14) ) );
        }
    }
}
//Writes a logic 1 or 0 to the specified pin. Same as gpio_set_mode logically but with different registers being manipulated.
void gpio_set_out(uint8_t pin, uint8_t on_or_off) { //digitalWrite()
    if(pin<8) {
        if(on_or_off) {
            PORTD |= (1 << pin); //write 1 in the position of that pin in the corresponding port -> write HIGH to that pin
        } else {
            PORTD &= ~(1 << pin);
        }
    } else if(pin < 14){
        if(on_or_off) {
            PORTB |= (1<<(pin-8));
        } else {
            PORTB &= ~( 1<<(pin-8) );
        }    
    } else if(pin < 20){
        if(on_or_off) {
            PORTC |= ( B00111111 & ( 1<<( pin-14) ) );
        } else {
            PORTC &= ~( B00111111 & ( 1<<(pin-14) ) ) ;
        }
    }
}
void gpio_toggle_out(uint8_t pin) {
    if(pin < 8) {
        PORTD ^= (1 << pin); //take the XOR of the bitmask representing the pin's position; flip the bit at the position of the pin.
    } else if (pin < 14) {
        PORTB ^= (1 << (pin-8));
    } else if (pin < 20){
        PORTC ^= ( B00111111 & (1 << (pin-14) ) );
    }
}
