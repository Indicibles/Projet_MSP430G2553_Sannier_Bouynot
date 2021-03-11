#include "msp430.h"
#include <string.h>

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    WDTCTL  = WDTPW + WDTHOLD;          // Stop WDT
    BCSCTL1 = CALBC1_1MHZ;              // Set DCO
    DCOCTL  = CALDCO_1MHZ;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;    // LED off

    P1SEL  |= (BIT5 | BIT6 | BIT7);
    P1SEL2 |= (BIT5 | BIT6 | BIT7);

    UCB0CTL1 = UCSWRST;
    UCB0CTL0  |= (UCMODE_0 | UCSYNC );
    UCB0CTL0  &= ~(UCMST | UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCB0CTL1 &= ~UCSWRST;                     // Initialize USCI
    IE2 |= UCB0RXIE;                          // Enable USCI0 RX interrupt

    __bis_SR_register(GIE);       // Enter LPM4, enable interrupts


    while(1);
}

#pragma vector = USCIAB0RX_VECTOR

__interrupt void USCIAB0RX_ISR()
{
          while (!(IFG2 & UCB0RXIFG));

          if(UCB0RXBUF=='1')
          {
              P1OUT |= BIT0;
          }
          if(UCB0RXBUF=='0')
          {
              P1OUT &= ~BIT0;
          }
}