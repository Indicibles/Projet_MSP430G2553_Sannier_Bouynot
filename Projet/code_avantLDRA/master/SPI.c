#include <includes_msp.c>
#include <msp430.h>
#include <string.h>
#define SCK         BIT5            // Serial Clock
#define DATA_OUT    BIT6            // DATA out
#define DATA_IN     BIT7            // DATA in
#define CMDLEN  10
unsigned char cmd[CMDLEN];      // tableau de caracteres lie a la commande user

/* ----------------------------------------------------------------------------
 * Fonction d'initialisation de l'USCI POUR SPI SUR UCB0
 * Entree : -
 * Sorties: -
 */
void init_SPI( void )
{
    // attente de sync avec le slave
    __delay_cycles(250);

    /* SOFTWARE RESET - mode configuration*/
    UCB0CTL0 = 0;
    UCB0CTL1 = (0 + UCSWRST*1 );

    UCB0CTL0 |= ( UCMST | UCMODE_0 | UCSYNC );
    UCB0CTL0 &= ~( UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCB0CTL1 |= UCSSEL_2;

    UCB0BR0 = 0x0A;     // divise SMCLK par 10
    UCB0BR1 = 0x00;

    P1SEL  |= ( SCK | DATA_OUT | DATA_IN);
    P1SEL2 |= ( SCK | DATA_OUT | DATA_IN);

    UCB0CTL1 &= ~UCSWRST;                                /*activation USCI*/
}

/* ----------------------------------------------------------------------------
 * Fonction d'envoie d'un caract�re sur USCI en SPI 3 fils MASTER Mode
 * Entree : Caract�re � envoyer
 * Sorties: /
 */
void Send_SPI(unsigned char carac)
{
    while ((UCB0STAT & UCBUSY));   /* attend que USCI_SPI soit dispo.*/
    while(!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = carac;              /* met un caract�re dans le buffer de transmition*/
}

