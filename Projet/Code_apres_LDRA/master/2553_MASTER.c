#include <includes.h>


/*
 * Definitions
 */
#define LED_R       BIT0            // Red LED
typedef unsigned int UINT_32;
typedef signed char SCHAR_8;
#define TRUE    1
#define FALSE   0

#define line_feed      0x0A            // line feed or \n
#define carriage_return      0x0D
#define BSPC    0x08            // back space
#define DEL     0x7F            // SUPRESS
#define ESC     0x1B            // escape





/*
 * Variables globales
 */

SCHAR_8 car = 0x30;       // 0
UINT_32 nb_car = 0u;
SCHAR_8 intcmd = FALSE;   // call interpreteur()



/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * main.c
 */
int main( void )
{
    init_LP();
    init_UART();
    init_SPI();

    send_UART("\rReady !\r\n"); // user prompt
    send_UART(PROMPT);        //---------------------------- command prompt

 while(1)
    	{
        if( intcmd == TRUE)
        {
            while ((UCB0STAT & UCBUSY))
		{
		   // attend que USCI_SPI soit dispo.
            }
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
__interrupt void USCIAB0RX_ISR(void)
{
    //---------------- UART
    if (IFG2 & UCA0RXIFG)
    {
        while(!(IFG2 & UCA0RXIFG))
		{
			/*encore un commentaire pour faire plaisir à LDRA*/
		}
        cmd[nb_car]=UCA0RXBUF;         // lecture caractère reçu

        while(!(IFG2 & UCA0TXIFG))
	{
    	// attente de fin du dernier envoi (UCA0TXIFG à 1 quand UCA0TXBUF vide) / echo
	}
        UCA0TXBUF = cmd[nb_car];

        if( cmd[nb_car] == ESC)
        {
            nb_car = 0u;
            cmd[1] = 0x00;
            cmd[0] = carriage_return;
        }

        if( (cmd[nb_car] == carriage_return) || (cmd[nb_car] == line_feed))
        {
            cmd[nb_car] = 0x00;
            intcmd = TRUE;
            nb_car = 0u;
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
        while( (UCB0STAT & UCBUSY) && !(UCB0STAT & UCOE) )
		{
			/*wait*/
		}
        while(!(IFG2 & UCB0RXIFG))
		{
			/*wait*/
		}
        cmd[0] = UCB0RXBUF;
        cmd[1] = 0x00;
        P1OUT ^= LED_R;
    }
	else
	{
		/*Nothing*/
	}
}
//------------------------------------------------------------------ End ISR

