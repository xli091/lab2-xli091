
#include <avr/io.h>
#include <util/delay.h>
#define F_CUP 16000000UL

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13
int setup(void)
{
  DDRB |= (1 << PORTB1);
  DDRB |= (1 << PORTB2);
  DDRB |= (1 << PORTB3);
  DDRB |= (1 << PORTB4);
}

int loop(void)
{
  // turn the LED on (HIGH is the voltage level)
  PORTB |= (1 << PORTB1);
  PORTB |= (1 << PORTB2);
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB4);

  // wait for a second

  return;
}

int main(void)
{
  setup();
  loop();
  while(1)
  return 0;
}