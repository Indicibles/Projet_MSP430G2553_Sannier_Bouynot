#include <msp430.h>
#include <string.h>
#include <ADC.h>
#include <ADC.h>
#include <PWM.h>


void position0(void)

{
    TA1CCR1 = 312.5;
}

void position90(void)

{
    TA1CCR1 = 187.5;
}

void position180(void)
{
    TA1CCR1 = 62.5;
}

void stop_rotation(void)
{
    TA1CCR1 = 0;
}

unsigned int Read_UV(unsigned char channel)
{
    ADC_Demarrer_conversion(channel);
    volatile unsigned int val = ADC_Lire_resultat();
    return val;
}

int main(void)
{
    volatile unsigned int UV_analog_value =0;
    P1DIR &=~ BIT0; //p1.0 as input
    ADC_init();
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    WDTCTL  = WDTPW + WDTHOLD;          // Stop WDT
    BCSCTL1 = CALBC1_1MHZ;              // Set DCO
    DCOCTL  = CALDCO_1MHZ;
    P1SEL  |= (BIT5 | BIT6 | BIT7);
    P1SEL2 |= (BIT5 | BIT6 | BIT7);
    UCB0CTL1 = UCSWRST;
    UCB0CTL0  |= (UCMODE_0 | UCSYNC );
    UCB0CTL0  &= ~(UCMST | UCCKPH | UCCKPL | UCMSB | UC7BIT );
    UCB0CTL1 &= ~UCSWRST;                     // Initialize USCI
    IE2 |= UCB0RXIE;                          // Enable USCI0 RX interrupt
    init_pwm();
    __bis_SR_register(GIE);       // Enter LPM4, enable interrupts
    volatile double sensorValue;
    while(1)
    {
        UV_analog_value = Read_UV(0);
        if (UV_analog_value<20)
        {
            sensorValue = 0;
        }
        else
        {
            sensorValue = 0.05*UV_analog_value-1;
        }
        __delay_cycles(100000);
    }
}

#pragma vector = USCIAB0RX_VECTOR

__interrupt void USCIAB0RX_ISR()
{
    while (!(IFG2 & UCB0RXIFG));
    if(UCB0RXBUF=='0')
    {
        stop_rotation();
    }
    if(UCB0RXBUF=='1')
    {
        position0();
    }
    if(UCB0RXBUF=='2')
    {
        position90();
    }
    if(UCB0RXBUF=='3')
    {
        position180();
    }
}

