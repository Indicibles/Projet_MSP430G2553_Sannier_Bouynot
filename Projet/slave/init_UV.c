#include <ADC.h>
#include <msp430.h>
#include <string.h>

void init_UV()
{
    P1DIR &=~ BIT0; //p1.0 as input
    ADC_init();
}

unsigned int Read_UV(unsigned char channel)
{
    ADC_Demarrer_conversion(channel);
    volatile unsigned int val = ADC_Lire_resultat();
    return val;
}
