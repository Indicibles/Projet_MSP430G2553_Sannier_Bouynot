#include <includes.c>   /*permet d'inclure tous les headers du projet coté master*/


/*
 * Definitions
 */
#define line_feed      0x0A         /* saut de ligne */
#define carriage_return      0x0D   /*retour chariot*/
#define BSPC    0x08                /* retour arrière*/
#define DEL     0x7F                /* Supression*/
#define ESC     0x1B                /* echape*/


/*
 * Déclaration des variables globales
 */
unsigned char car = 0x30;   /* met le code scci de 0*/
unsigned int  nb_car = 0;
unsigned char intcmd = 0;   /* appel l'interpreteur()*/



/*
 * fonction main
 */
void main( void )
{
    init_LP();
    init_UART();
    init_SPI();

    send_UART("\rPRET !\r\n"); // user prompt

 while(1)
    {
        if( intcmd )
        {
            while ((UCB0STAT & UCBUSY));   /* attend que USCI_SPI soit dispo.*/
            interpreteur();         /* execute la commande utilisateur*/
            intcmd = FALSE;         /* acquitte la commande en cours*/
        }
        else
        {
            __bis_SR_register(LPM4_bits | GIE); /* general interrupts enable & Low Power Mode*/
        }
    }
}



/* ************************************************************************* */
/* INTERRUPTION USCI RX                                                      */
/* ************************************************************************* */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR()        /*debut de l'interruption*/
{
    //---------------- UART
    if (IFG2 & UCA0RXIFG)
    {
        while(!(IFG2 & UCA0RXIFG));
        cmd[nb_car]=UCA0RXBUF;         /* lecture caractère reçu*/

        while(!(IFG2 & UCA0TXIFG));    /* attente de fin du dernier envoi (UCA0TXIFG à 1 quand UCA0TXBUF vide) */
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
            intcmd = 1;
            nb_car = 0;
            __bic_SR_register_on_exit(LPM4_bits);
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
}

