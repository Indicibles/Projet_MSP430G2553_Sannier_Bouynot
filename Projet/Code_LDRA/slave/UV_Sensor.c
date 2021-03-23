#include <ADC.h>
#include <msp430.h>
#include <string.h>
typedef unsigned int UINT_32;
typedef unsigned char UCHAR_8;

void init_UV()
{
    P1DIR &=~ BIT0; //p1.0 as input
    ADC_init();
}

UINT_32 Read_UV(UCHAR_8 channel)
{
    ADC_Demarrer_conversion(channel);
    volatile UINT_32 val = ADC_Lire_resultat();
    if (val<(UINT_32)20u)
    {
        val = (UINT_32)0u;
    }
    else
    {
        val = (((UINT_32)0.05)*val)-(UINT_32)1u;
    }
    return val;
}
