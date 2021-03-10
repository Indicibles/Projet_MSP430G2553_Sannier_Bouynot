#include <msp430.h>
Send_SPI(unsigned char sendCarac)
{
    while ((UCB0STAT & UCBUSY));            // Wait USCI_SPI
        while(!(IFG2 & UCB0TXIFG));
        UCB0TXBUF = sendCarac;              // Put character in transmit buffer
}



int main(void)
{
P1DIR |= BIT0 + BIT6; // P1.0 and P1.6 output
P1OUT &= ~BIT0 + BIT6; // P1.0 and P1.6 = 0
WDTCTL = WDTPW + WDTHOLD; // Stop WDT
if (CALBC1_1MHZ==0xFF) // If calibration constant erased
{
while(1); // do not load, trap CPU!!
}
DCOCTL = 0; // Select lowest DCOx and MODx settings
BCSCTL1 = CALBC1_1MHZ; // Set DCO
DCOCTL = CALDCO_1MHZ;
P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
UCA0CTL1 |= UCSSEL_2; // SMCLK
UCA0BR0 = 104; // 1MHz 9600
UCA0BR1 = 0; // 1MHz 9600
UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt


   __delay_cycles(250);
   UCB0CTL0 = 0;
   UCB0CTL1 = (0 + UCSWRST*1 );
   IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);
   UCB0CTL0 |= ( UCMST | UCMODE_0 | UCSYNC );
   UCB0CTL0 &= ~( UCCKPH | UCCKPL | UCMSB | UC7BIT );
   UCB0CTL1 |= UCSSEL_2;
   UCB0BR0 = 0x0A;     // divide SMCLK by 10
   UCB0BR1 = 0x00;
   P1SEL  |= ( SCK | DATA_OUT | DATA_IN);
   P1SEL2 |= ( SCK | DATA_OUT | DATA_IN);
   UCB0CTL1 &= ~UCSWRST;

__bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}



// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
if (UCA0RXBUF == 'h') // 'h' received?
{
    int i=0;
    char chaine[]="Press Y to turn ON the LED\nPress N to turn OFF the LED\n";

     for(i=0; i<sizeof(chaine); i++)
    {
        UCA0TXBUF = chaine[i];
    }

}
if (UCA0RXBUF == 'e') // 'e' received?
{
    int i=0;
    char chaine[]="Press e to close help";

     for(i=0; i<sizeof(chaine); i++)
    {
        UCA0TXBUF = chaine[i];
    }

}

//"Press Y to turn ON the LED Press N to turn OFF the LED\n";
if (UCA0RXBUF == 'Y') // 'Y' received?
{
P1OUT |= BIT6; // light up P1.6 LED
}
if (UCA0RXBUF == 'N') // 'N' received?
{
P1OUT &=~ BIT6; // light down P1.6 LED
}
if (UCA0RXBUF == '1') // '1' received?
{
    Send_SPI('1');
    char chaine[]="Led 1 turned on";
    for(i=0; i<sizeof(chaine); i++)
      {
          UCA0TXBUF = chaine[i];
      }
}
if (UCA0RXBUF == '2') // '2' received?
{
    Send_SPI('2');
    char chaine[]="Led 2 turned on";
    for(i=0; i<sizeof(chaine); i++)
      {
          UCA0TXBUF = chaine[i];
      }
}
if (UCA0RXBUF == '0') // '0' received?
{
    Send_SPI('0');
    char chaine[]="Led 1 and 2 turned off";
    for(i=0; i<sizeof(chaine); i++)
      {
          UCA0TXBUF = chaine[i];
      }
}
}
