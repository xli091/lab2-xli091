#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void Initialize(void)
{
  cli();

  DDRB &= ~(1 << PORTB0);
  DDRB |= (1 << PORTB5);


  // Timer1 Setup
  // Set Timer 1 clock to be internal div by 8
  // 2MHz timer clock, 1 tick = (1/2M) second 
  TCCR1B &= ~(1<<CS10);
  TCCR1B |=(1<<CS11);
  TCCR1B &= ~(1<<CS12);
  // Set Timer 1 to Normal 
  TCCR1A &= ~(1<<WGM10);
  TCCR1A &= ~(1<<WGM11);
  TCCR1B &= ~(1<<WGM12);
  TCCR1B &= ~(1<<WGM13);

  // Looking for falling edge 
  TCCR1B &= ~(1<<ICES1);
  // Clear interrupt flag 
  TIFR1 |= (1<<ICF1);
  // Enable input capture interrupt 
  TIMSK1 |=(1<<ICIE1);

  // Enable global interrupts
  sei();
}

ISR(TIMER1_CAPT_vect)
{
  if (!(PINB & (1 << PORTB0)))
  {
    PORTB |= (1 << PORTB5);
    // Looking for rising edge 
    TCCR1B |= (1<<ICES1);
    // Clear interrupt flag 
    TIFR1 |= (1<<ICF1);
    // Enable input capture interrupt 
    TIMSK1 |=(1<<ICIE1);
  }

  else if (PINB & (1 << PORTB0))
  {
    PORTB &= ~(1 << PORTB5);
    // Looking for rising edge 
    TCCR1B &= ~(1<<ICES1);
    // Clear interrupt flag 
    TIFR1 |= (1<<ICF1);
    // Enable input capture interrupt 
    TIMSK1 |=(1<<ICIE1);
  }
}

int main(void)
{
  Initialize();
  while(1)
  {}
  return 0;
}