#ifndef GPIO_H
#define GPIO_H
void gpio_set_mode(uint8_t pin, uint8_t out_or_in);
void gpio_set_out(uint8_t pin, uint8_t on_or_off);
void gpio_toggle_out(uint8_t pin);
#endif // GPIO_H
