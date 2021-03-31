#include <includes_msp.h>
#include <SPI.h>

typedef unsigned long int UINT_32;
typedef int INT_32;

typedef unsigned char UCHAR_8;
/* ----------------------------------------------------------------------------
 * Fonction d'initialisation de l'UART
 * Entree : -
 * Sorties: -
 */

void init_UART( void )
{
    P1SEL  |= (BIT1 | BIT2);                    // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= (BIT1 | BIT2);                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSWRST;                        // SOFTWARE RESET
    UCA0CTL1 |= UCSSEL_2;                       // SMCLK (2 - 3)
    UCA0BR0 = 104;                             // 104 1MHz, OSC16, 9600 (8Mhz : 52) : 8 115k - 226/12Mhz
    UCA0BR1 = 0;                                // 1MHz, OSC16, 9600 - 4/12Mhz
    UCA0MCTL = 10;
    UCA0CTL0 &= ~(UCPEN  | UCMSB | UCDORM);
    UCA0CTL0 &= ~(UC7BIT | UCSPB  | UCMODE_3 | UCSYNC); // dta:8 stop:1 usci_mode3uartmode
    UCA0CTL1 &= ~UCSWRST;                       // **Initialize USCI state machine**
    /* Enable USCI_A0 RX interrupt */
    IE2 |= UCA0RXIE;
}

/* ----------------------------------------------------------------------------
 *Envoie chaine de carac
 */
void send_UART(UCHAR_8 *msg)
{
    UINT_32 i = 0u;
    for(i= (UINT_32)0u ; msg[i] !=(UCHAR_8)0x00 ; i++)
    {
        while((IFG2 & UCA0TXIFG)!=0)
		{
    		//attente de fin du dernier envoi (UCA0TXIFG à 1 quand UCA0TXBUF vide)
		}
        UCA0TXBUF=(INT_32)msg[i];
    }
}

