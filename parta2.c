#include <avr/io.h>
#include <util/delay.h>

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13
void setup(void)
{
  DDRD &= ~(1 << PORTD7);
  DDRB |= (1 << PORTB1);
  DDRB |= (1 << PORTB2);
  DDRB |= (1 << PORTB3);
  DDRB |= (1 << PORTB4);

  PORTD &= ~(1 << PORTD7);
}

void loop(void)
{
  while(1){
    PORTB |= (1 << PORTB1);
    while (!(PIND & (1 << PORTD7)))
    {
    }
    PORTD &= ~(1 << PORTD7);
    PORTB &= ~(1 << PORTB1);
    PORTB |= (1 << PORTB2);
    _delay_ms(1000);

    while (!(PIND & (1 << PORTD7)))
    {
    }
    PORTD &= ~(1 << PORTD7);
    PORTB &= ~(1 << PORTB2);
    PORTB |= (1 << PORTB3);
    _delay_ms(1000);

    while (!(PIND & (1 << PORTD7)))
    {
    }
    PORTD &= ~(1 << PORTD7);
    PORTB &= ~(1 << PORTB3);
    PORTB |= (1 << PORTB4);
    _delay_ms(1000);

    while (!(PIND & (1 << PORTD7)))
    {
    }
    PORTD &= ~(1 << PORTD7);
    PORTB &= ~(1 << PORTB4);
    PORTB |= (1 << PORTB1);
    _delay_ms(1000);
  }
}

int main(void){
  setup();
  loop();
  while (1)
  {}
  return 0;
}