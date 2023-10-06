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

volatile int input = 0;

void Initialize(void)
{
  cli();

  DDRB &= ~(1 << PORTB0);
  PORTB |= (1 << PORTB0);
  DDRB |= (1 << PORTB1);
  DDRB |= (1 << PORTB2);

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

void calcu(void)
{
   if (TIFR1 & (1 << TOV1))
   {
      input = 0;
   }
   
   else if (releaset >= val_30ms && releaset <= val_200ms)
   {
      input = input * 10 + 1;
      PORTB |= (1 << PORTB1);
      _delay_ms(50);
      PORTB &= ~(1 << PORTB1);
   }

   else if (releaset > val_200ms && releaset <= val_400ms)
   {
      input = input * 10 + 3;
      PORTB |= (1 << PORTB2);
      _delay_ms(50);
      PORTB &= ~(1 << PORTB2);
   }

   else
   {
      input = 0;
   }
}

void translate(void)
{
  int output = 0;

  switch (input)
  {
  case 13:
    output = 65;
    break;

  case 3111:
    output = 66;
    break;

  case 3131:
    output = 67;
    break;

  case 311:
    output = 68;
    break;

  case 1:
    output = 69;
    break;

  case 1131:
    output = 70;
    break;

  case 331:
    output = 71;
    break;

  case 1111:
    output = 72;
    break;

  case 11:
    output = 73;
    break;

  case 1333:
    output = 74;
    break;

  case 313:
    output = 75;
    break;

  case 1311:
    output = 76;
    break;

  case 33:
    output = 77;
    break;

  case 31:
    output = 78;
    break;

  case 333:
    output = 79;
    break;

  case 1331:
    output = 80;
    break;

  case 3313:
    output = 81;
    break;

  case 131:
    output = 82;
    break;

  case 111:
    output = 83;
    break;

  case 3:
    output = 84;
    break;

  case 113:
    output = 85;
    break;

  case 1113:
    output = 86;
    break;

  case 133:
    output = 87;
    break;

  case 3113:
    output = 88;
    break;

  case 3133:
    output = 89;
    break;

  case 3311:
    output = 90;
    break;

  case 13333:
    output = 49;
    break;

  case 11333:
    output = 50;
    break;

  case 11133:
    output = 51;
    break;

  case 11113:
    output = 52;
    break;

  case 11111:
    output = 53;
    break;

  case 31111:
    output = 54;
    break;

  case 33111:
    output = 55;
    break;

  case 33311:
    output = 56;
    break;

  case 33331:
    output = 57;
    break;

  case 33333:
    output = 48;
    break;

  default:
    break;
  }

  UART_init(BAUD_PRESCALER);
  sprintf(String,"%c",output);
  UART_putstring(String);
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
    calcu();

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

  translate();
  input = 0;
}

int main(void)
{
  Initialize();
  while(1)
  {}
  return 0;
}