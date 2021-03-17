#include <msp430.h>
#include <string.h>

void init_SPI_Slave(void)
{
    P1SEL  |= (BIT1 | BIT2 | BIT4);
    P1SEL2 |= (BIT1 | BIT2 | BIT4);
    UCA0CTL1 = UCSWRST;
    UCA0CTL0  |= (UCMODE_0 | UCSYNC );
    UCA0CTL0  &= ~(UCMST | UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCA0CTL1 &= ~UCSWRST;                     // Initialize USCI
    IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt
}
