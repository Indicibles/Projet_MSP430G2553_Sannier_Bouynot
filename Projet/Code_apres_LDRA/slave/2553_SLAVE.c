#include <msp430.h>
#include <string.h>
#include <ADC.h>
#include <PWM.h>
#include <SPI_Slave.h>
#include <initLP.h>
#include <I2C.h>
#include <UV_Sensor.h>

typedef double DOUBLE_64;
typedef unsigned int UINT_32;
typedef int INT_32;
INT_32 main(void)
{
    volatile UINT_32 UV_value =0;
    volatile DOUBLE_64 sensorValue;
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
    while (!(IFG2 & UCA0RXIFG))
	{
	}
    if(UCB0RXBUF== (UINT_32) '0')
    {
        stop_rotation();
    }
    if(UCA0RXBUF== (UINT_32)'1')
    {
        position0();
    }
    if(UCA0RXBUF== (UINT_32)'2')
    {
        position90();
    }
    if(UCA0RXBUF== (UINT_32)'3')
    {
        position180();
    }
}

