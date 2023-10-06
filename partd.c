#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
char String[25];

volatile int releaset = 0;
volatile int val_30ms = 1875;
volatile int val_200ms = 12500;
volatile int val_400ms = 25000;

void Initialize(void)
{
  cli();

  DDRB &= ~(1 << PORTB0);
  PORTB |= (1 << PORTB0);

  // Timer1 Setup
  // Set Timer 1 clock to be internal div by 256
  // 62.5kHz timer clock, 1 tick = (1/625000) second 
  TCCR1B &= ~(1<<CS10);
  TCCR1B &= ~(1<<CS11);
  TCCR1B |= (1<<CS12);
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

void print(void) {
  UART_init(BAUD_PRESCALER);

  if (TIFR1 & (1 << TOV1)){
    sprintf(String,"Error ");
    UART_putstring(String);
  }

  else if (releaset >= val_30ms && releaset <= val_200ms)
  {
    sprintf(String,"Dot ");
    UART_putstring(String);
  }

  else if (releaset > val_200ms && releaset <= val_400ms)
  {
    sprintf(String,"Dash ");
    UART_putstring(String);
  }

  else
  {
    sprintf(String,"Error ");
    UART_putstring(String);
  }
}

ISR(TIMER1_CAPT_vect)
{
  if (!(PINB & (1 << PORTB0)))
  {
    TIMSK1 &= ~(1 << TOIE1);
    TCNT1 = 0;
    TIFR1 |= (1 << TOV1);
    PINB &= ~(1 << PORTB0);
    
  }
  else if (PINB & (1 << PORTB0))
  {
    releaset = TCNT1;
    print();

    TIFR1 |= (1 << TOV1);
    TCNT1 = 40536;
    TIMSK1 |= (1 << TOIE1);

    PORTB |= (1 << PORTB0);
  }
  // Looking for rising/falling edge 
  TCCR1B ^= (1<<ICES1);
  // Clear interrupt flag 
  TIFR1 |= (1<<ICF1);
  // Enable input capture interrupt 
  TIMSK1 |=(1<<ICIE1);
}

ISR(TIMER1_OVF_vect)
{
  TIMSK1 &= ~(1 << TOIE1);

  UART_init(BAUD_PRESCALER);
  sprintf(String,"Space ");
  UART_putstring(String);
}

int main(void)
{
  Initialize();
  while(1)
  {}
  return 0;
}