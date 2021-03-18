#include <msp430.h>
#include <string.h>
#include <ADC.h>
#include <PWM.h>
#include <SPI_Slave.h>
#include <initLP.h>
#include <I2C.h>
#include <UV_Sensor.h>

int main(void)
{
    volatile unsigned int UV_value =0;
    volatile double sensorValue;
    initi_LP_Slave();
    init_SPI_Slave();
    init_UV();
    init_pwm();
    init_I2C();
    __bis_SR_register(GIE);       // Enter LPM4, enable interrupts
    while(1)
    {
        UV_value = Read_UV(0);

        __delay_cycles(100000);
    }
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR()
{
    while (!(IFG2 & UCA0RXIFG));
    if(UCB0RXBUF=='0')
    {
        stop_rotation();
    }
    if(UCA0RXBUF=='1')
    {
        position0();
    }
    if(UCA0RXBUF=='2')
    {
        position90();
    }
    if(UCA0RXBUF=='3')
    {
        position180();
    }
}

