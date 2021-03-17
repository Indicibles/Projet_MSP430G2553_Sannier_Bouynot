#define CMDLEN  10
#define TRUE    1
#define FALSE   0

#define LF      0x0A            // line feed or \n
#define CR      0x0D            // carriage return or \r
#define BSPC    0x08            // back space
#define DEL     0x7F            // SUPRESS
#define ESC     0x1B            // escape

#define DATA_OUT    BIT6            // DATA out
#define DATA_IN     BIT7            // DATA in


/*
 * Variables globales
 */
// static const char spi_in = 0x37;
unsigned char cmd[CMDLEN];      // tableau de caracteres lie a la commande user
unsigned char car = 0x30;       // 0
unsigned int  nb_car = 0;
unsigned char intcmd = FALSE;   // call interpreteur()
volatile unsigned char RXdata;

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
    UCB0CTL0 |= ( UCMODE_0 | UCSYNC );
    UCB0CTL0 &= ~( UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCB0CTL1 |= UCSSEL_2;
    UCB0BR0 = 0x0A;     // divide SMCLK by 10
    UCB0BR1 = 0x00;
    P1SEL  |= ( DATA_OUT | DATA_IN);
    P1SEL2 |= ( DATA_OUT | DATA_IN);
    UCB0CTL1 &= ~UCSWRST;

    __bis_SR_register(LPM4_bits | GIE); // general interrupts enable & Low Power Mode

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    while( !(UCB0RXBUF & IFG2) );   // waiting char by USI counter flag
    RXdata= UCB0RXBUF;
    if (RXdata == '1')
    {
        P1OUT |= BIT0; //turn on LED1
    }
    if (RXdata == '2')
    {
        P1OUT |= BIT6; //turn on LED2
    }
    else if (RXdata == '0')
    {
        P1OUT &= ~BIT0; //turn off LED1
        P1OUT &= ~BIT6; //turn off LED2
    }

    /*USISRL = RXdata;
    USICNT &= ~USI16B;
    USICNT = 0x08;*/
}
