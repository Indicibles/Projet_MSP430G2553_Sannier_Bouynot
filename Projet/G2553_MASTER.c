#include <msp430.h>
#include <string.h>


/*
 * Prototypes
 */
void init_LP( void );
void init_UART( void );
void init_SPI( void );
void interpreteur( void );
void send_UART(unsigned char * );
void Send_SPI( unsigned char );

/*
 * Definitions
 */
#define RELEASE "\r\t\tSPI-rIII162018"
#define PROMPT  "\r\nmaster>"
#define CMDLEN  10

#define TRUE    1
#define FALSE   0

#define line_feed      0x0A            // line feed or \n
#define carriage_return      0x0D
#define BSPC    0x08            // back space
#define DEL     0x7F            // SUPRESS
#define ESC     0x1B            // escape

#define SCK         BIT5            // Serial Clock
#define DATA_OUT    BIT6            // DATA out
#define DATA_IN     BIT7            // DATA in

#define LED_R       BIT0            // Red LED
#define LED_G       BIT6            // Green LED

/*
 * Variables globales
 */
// static const char spi_in = 0x37;
unsigned char cmd[CMDLEN];      // tableau de caracteres lie a la commande user
unsigned char car = 0x30;       // 0
unsigned int  nb_car = 0;
unsigned char intcmd = FALSE;   // call interpreteur()

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
    else                          //---------------------------- default choice
    {
        send_UART("\r\n ?");
        send_UART((unsigned char *)cmd);
    }
    send_UART(PROMPT);        //---------------------------- command prompt
}

/* ----------------------------------------------------------------------------
 * Fonction d'initialisation de la carte TI LauchPAD
 * Entrees: -
 * Sorties:  -
 */
void init_LP( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW | WDTHOLD;

    if( (CALBC1_1MHZ==0xFF) || (CALDCO_1MHZ==0xFF) )
    {
        __bis_SR_register(LPM4_bits);
    }
    else
    {
        // Factory Set.
        DCOCTL = 0;
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = (0 | CALDCO_1MHZ);
    }

    //--------------- Secure mode
    P1SEL  = 0x00;        // GPIO
    P1SEL2 = 0x00;        // GPIO
    P2SEL  = 0x00;        // GPIO
    P2SEL2 = 0x00;        // GPIO
    P1DIR = 0x00;         // IN
    P2DIR = 0x00;         // IN

    P1SEL  &= ~LED_R;
    P1SEL2 &= ~LED_R;
    P1DIR |= LED_R ;  // LED: output
    P1OUT &= ~LED_R ;
}

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
 * Fonction d'initialisation de l'USCI POUR SPI SUR UCB0
 * Entree : -
 * Sorties: -
 */
void init_SPI( void )
{
    // Waste Time, waiting Slave SYNC
    __delay_cycles(250);

    // SOFTWARE RESET - mode configuration
    UCB0CTL0 = 0;
    UCB0CTL1 = (0 + UCSWRST*1 );

    UCB0CTL0 |= ( UCMST | UCMODE_0 | UCSYNC );
    UCB0CTL0 &= ~( UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCB0CTL1 |= UCSSEL_2;

    UCB0BR0 = 0x0A;     // divide SMCLK by 10
    UCB0BR1 = 0x00;

    // SPI : Fonctions secondaires
    // MISO-1.6 MOSI-1.7 et CLK-1.5
    // Ref. SLAS735G p48,49
    P1SEL  |= ( SCK | DATA_OUT | DATA_IN);
    P1SEL2 |= ( SCK | DATA_OUT | DATA_IN);

    UCB0CTL1 &= ~UCSWRST;                                // activation USCI
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
 * Fonction d'envoie d'un caractère sur USCI en SPI 3 fils MASTER Mode
 * Entree : Caractère à envoyer
 * Sorties: /
 */
void Send_SPI(unsigned char carac)
{
    while ((UCB0STAT & UCBUSY));   // attend que USCI_SPI soit dispo.
    while(!(IFG2 & UCB0TXIFG)); // p442
    UCB0TXBUF = carac;              // Put character in transmit buffer
    send_UART((unsigned char *)cmd);   // slave echo
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * main.c
 */
void main( void )
{
    init_LP();
    init_UART();
    init_SPI();

    send_UART("\rReady !\r\n"); // user prompt
    send_UART(PROMPT);        //---------------------------- command prompt

 while(1)
    {
        if( intcmd )
        {
            while ((UCB0STAT & UCBUSY));   // attend que USCI_SPI soit dispo.
            interpreteur();         // execute la commande utilisateur
            intcmd = FALSE;         // acquitte la commande en cours
        }
        else
        {
            __bis_SR_register(LPM4_bits | GIE); // general interrupts enable & Low Power Mode
        }
    }
}



/* ************************************************************************* */
/* INTERRUPTION USCI RX                                              */
/* ************************************************************************* */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR()
{
    //---------------- UART
    if (IFG2 & UCA0RXIFG)
    {
        while(!(IFG2 & UCA0RXIFG));
        cmd[nb_car]=UCA0RXBUF;         // lecture caractère reçu

        while(!(IFG2 & UCA0TXIFG));    // attente de fin du dernier envoi (UCA0TXIFG à 1 quand UCA0TXBUF vide) / echo
        UCA0TXBUF = cmd[nb_car];

        if( cmd[nb_car] == ESC)
        {
            nb_car = 0;
            cmd[1] = 0x00;
            cmd[0] = carriage_return;
        }

        if( (cmd[nb_car] == carriage_return) || (cmd[nb_car] == line_feed))
        {
            cmd[nb_car] = 0x00;
            intcmd = TRUE;
            nb_car = 0;
            __bic_SR_register_on_exit(LPM4_bits);   // OP mode !
        }
        else if( (nb_car < CMDLEN) && !((cmd[nb_car] == BSPC) || (cmd[nb_car] == DEL)) )
        {
            nb_car++;
        }
        else
        {
            cmd[nb_car] = 0x00;
            nb_car--;
        }
    }

    //--------------- SPI
    else if (IFG2 & UCB0RXIFG)
    {
        while( (UCB0STAT & UCBUSY) && !(UCB0STAT & UCOE) );
        while(!(IFG2 & UCB0RXIFG));
        cmd[0] = UCB0RXBUF;
        cmd[1] = 0x00;
        P1OUT ^= LED_R;
    }
}
//------------------------------------------------------------------ End ISR
