#include <msp430.h>
#include <string.h>

void position0(void)
{
    TA1CCR1 = 312.5;
}

void position90(void)
{
    TA1CCR1 = 187.5;
}

void position180(void)
{
    TA1CCR1 = 62.5;
}

void stop_rotation(void)
{
    TA1CCR1 = 0;
}
