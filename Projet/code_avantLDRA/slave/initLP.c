#include <msp430.h>
#include <string.h>

void init_LP_Slave()
{
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
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    WDTCTL  = WDTPW + WDTHOLD;          // Stop WDT
}
