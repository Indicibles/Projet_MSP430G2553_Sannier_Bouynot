#include <msp430.h>
#include <string.h>

void init_I2C(void)
{
    P1SEL  |= (BIT6 | BIT7);
    P1SEL2 |= (BIT6 | BIT7);
}
