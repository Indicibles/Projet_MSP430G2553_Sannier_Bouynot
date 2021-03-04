#include <msp430.h> 
#include <intrinsics.h>

volatile unsigned char RXdata;

void main( void )
{
	WDTCTL = WDTPW | WDTHOLD;

    if(CALBC1_1MHZ==0xFF || CALDCO_1MHZ==0xFF)
    {
        __bis_SR_register(LPM4_bits);
    }
    else
    {
        DCOCTL = 0;
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = (0 | CALDCO_1MHZ);
    }

    P1SEL = 0x00;
    P1DIR = 0x00;

    // leds
    P1DIR |=  BIT0 | BIT6;
    P1OUT &= ~BIT0 | BIT6;


    USICTL0 |= USISWRST;
    USICTL1 = 0;


    USICTL0 |= (USIPE7 | USIPE6 | USIPE5 | USILSB | USIOE | USIGE );

    USICTL0 &= ~(USIMST);
    USICTL1 |= USIIE;
    USICTL1 &= ~(USICKPH | USII2C);

    USICKCTL = 0;
    USICKCTL &= ~(USICKPL | USISWCLK);	

    USICNT = 0;
    USICNT &= ~(USI16B | USIIFGCC ); 
    USISRL = 0x23;	
    USICNT = 0x08;

	while ((P1IN & BIT5)) ;

    USICTL0 &= ~USISWRST;

    __bis_SR_register(LPM4_bits | GIE); // general interrupts enable & Low Power Mode
}



#pragma vector=USI_VECTOR
__interrupt void universal_serial_interface(void)
{
	while( !(USICTL1 & USIIFG) );   // waiting char by USI counter flag
	RXdata= USISRL;

	if RXdata == '1')
	{
		P1OUT |= BIT0; //turn on LED1
	}
	if RXdata == '2') 
	{
		P1OUT |= BIT6; //turn on LED2
	}
	else if RXdata == '0')
	{
		P1OUT &= ~BIT0; //turn off LED1
		P1OUT &= ~BIT6; //turn off LED2
	}
	USISRL = RXdata;
	USICNT &= ~USI16B;
	USICNT = 0x08;
}
