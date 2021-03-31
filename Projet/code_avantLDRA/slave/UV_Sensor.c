#include <ADC.h>
#include <msp430.h>
#include <string.h>

void init_UV()
{
    P1DIR &=~ BIT0; /* met p1.0 en entrée*/
    ADC_init();     /*initialisation de l'ADC*/
}

unsigned int Read_UV(unsigned char channel)
{
    ADC_Demarrer_conversion(channel);   /*conversion de la valeur lu par l'ADC*/
    volatile unsigned int val = ADC_Lire_resultat();/*Stock la valeur lue par l'ADC*/
    if (val<20)
    {
        val = 0;
    }
    else
    {
        val = 0.05*val-1;
    }
    return val;
}

