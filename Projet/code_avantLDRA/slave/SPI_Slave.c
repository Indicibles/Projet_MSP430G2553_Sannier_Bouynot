#include <msp430.h>
#include <string.h>

void init_SPI_Slave(void) /*initialisation de la communication SPI coté slave*/
{
    P1SEL  |= (BIT1 | BIT2 | BIT4);
    P1SEL2 |= (BIT1 | BIT2 | BIT4);
    UCA0CTL1 = UCSWRST;
    UCA0CTL0  |= (UCMODE_0 | UCSYNC );
    UCA0CTL0  &= ~(UCMST | UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCA0CTL1 &= ~UCSWRST;                     /* initialisation USCI*/
    IE2 |= UCA0RXIE;                          /* Enable USCI0 RX interrupt*/
}
void Send_SPI(unsigned char carac)
{
    while ((UCA0STAT & UCBUSY));   // attend que USCI_SPI soit dispo.
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = (unsigned char) carac;              // met la valeur lu dans le buffer de transmition

}


