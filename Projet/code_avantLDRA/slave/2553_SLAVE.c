#include <msp430.h>
#include <string.h>
#include <ADC.h>
#include <PWM.h>
#include <SPI_Slave.h>
#include <initLP.h>
#include <UV_Sensor.h>

int main(void)
{
    volatile double sensorValue;
    init_LP_Slave();               //initialisation de la launchpad
    init_SPI_Slave();               //initialisation de la communication SPI
    init_UV();                      //initialisation du capteur UV
    init_pwm();                     //initialisation de la pwm
    __bis_SR_register(GIE);        // autorisation interruption, non fonctionnel
    while(1)
    {
    }
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR()/*interruption sur réception SPI*/
{
    while (!(IFG2 & UCA0RXIFG));/*attente de la réception complete de la commande*/
    if(UCB0RXBUF=='0')
    {
        stop_rotation(); /*appel de la fonction permettant d'arrêter le servo-moteur*/
    }
    if(UCA0RXBUF=='1')
    {
        position0(); /*appel de la fonction permettant de mettre le servo-moteur en position 0°*/
    }
    if(UCA0RXBUF=='2')
    {
        position90(); /*appel de la fonction permettant de mettre le servo-moteur en position 90°*/
    }
    if(UCA0RXBUF=='3')
    {
        position180(); /*appel de la fonction permettant de mettre le servo-moteur en position 180°*/
    }
    if(UCA0RXBUF=='4')
        {
        volatile unsigned int UV_value = Read_UV(0);
        Send_SPI(UV_value); /*envoie de la valeur lu sur le capteur UV au master*/
        }
}

