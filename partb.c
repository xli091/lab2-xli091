#include <avr/io.h>
#include <util/delay.h>

void setup(void)
{
    DDRB &= ~(1 << PORTB0);
    DDRB |= (1 << PORTB5);
}

void loop(void)
{
    while(1)
    {
        while (PINB & (1 << PORTB0))
        {
            PORTB &= ~(1 << PORTB5);
        }
        PORTB |= (1 << PORTB5);
    }
}

int main(void)
{
    setup();
    loop();
    return 0;
}