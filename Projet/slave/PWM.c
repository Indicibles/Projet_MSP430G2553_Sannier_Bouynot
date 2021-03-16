#include <msp430.h>
#include <string.h>

void init_pwm()
{
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR |= BIT1 | BIT2 | BIT4 | BIT5;
    P2DIR &= ~(BIT0 | BIT3);
    BCSCTL1 = CALBC1_1MHZ;      // choix d'horloge à 1Mhz
    DCOCTL = CALDCO_1MHZ;
    P2OUT |= BIT5;
    P2OUT &= ~BIT1;
    P2DIR |= BIT2;     //sortie des timers
    P2SEL |= BIT2;
    P2SEL2 &= BIT2;
    TA1CTL = TASSEL_2 | MC_1 | ID_1 | ID_2;   // mode up
    TA1CCTL1 |= OUTMOD_7;       // activation mode de sortie n°7
    TA1CCR0 = 2500;             //timer
    TA1CCR1 = 0;
}
