#include <includes_msp.c>
#include <SPI.h>
#define PROMPT  "\r\nmaster>"
#define CMDLEN  10
unsigned char cmd[CMDLEN];      // tableau de caracteres lie a la commande user
#define RELEASE "\r\t\tSPI-rIII162018"


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
void send_UART(unsigned char *msg)
{
    unsigned int i = 0;
    for(i=0 ; msg[i] != 0x00 ; i++)
    {
        while(!(IFG2 & UCA0TXIFG));    //attente de fin du dernier envoi (UCA0TXIFG à 1 quand UCA0TXBUF vide)
        UCA0TXBUF=msg[i];
    }
}


/* ----------------------------------------------------------------------------
 * Fonction d'interpretation des commandes utilisateur
 * Entrees: -
 * Sorties:  -
 */
void interpreteur( void )
{
    if(strcmp((const char *)cmd, "h") == 0)          //----------------------------------- help
    {
        send_UART("\r\nCommandes :");
        send_UART("\r\n'ver' : version");
        send_UART("\r\n'0' : LED off");
        send_UART("\r\n'1' : LED on");
        send_UART("\r\n'h' : help\r\n");
    }
    else if (strcmp((const char *)cmd, "0") == 0)
    {
        send_UART("\r\n");
        send_UART((unsigned char *)cmd);
        send_UART("->");
        Send_SPI(0x30); // Send '0' over SPI to Slave
        send_UART("\r\n");
    }
    else if (strcmp((const char *)cmd, "1") == 0)
    {
        send_UART("\r\n");
        send_UART((unsigned char *)cmd);
        send_UART("->");
        Send_SPI(0x31); // Send '1' over SPI to Slave
        send_UART("\r\n");
    }

    else if (strcmp((const char *)cmd, "ver") == 0)
    {
        send_UART("\r\n");
        send_UART(RELEASE);
        send_UART("\r\n");
    }
    else if (strcmp((const char *)cmd, "2") == 0)
    {
        send_UART("\r\n");
        send_UART((unsigned char *)cmd);
        send_UART("->");
        Send_SPI(0x32); // Send '2' over SPI to Slave
        send_UART("\r\n");
    }
    else if (strcmp((const char *)cmd, "3") == 0)
    {
        send_UART("\r\n");
        send_UART((unsigned char *)cmd);
        send_UART("->");
        Send_SPI(0x33); // Send '2' over SPI to Slave
        send_UART("\r\n");
    }
    else                          //---------------------------- default choice
    {
        send_UART("\r\n ?");
        send_UART((unsigned char *)cmd);
    }
    send_UART(PROMPT);        //---------------------------- command prompt
}
