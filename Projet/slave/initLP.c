#include <msp430.h>
#include <string.h>

void initi_LP_Slave()
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    WDTCTL  = WDTPW + WDTHOLD;          // Stop WDT
    BCSCTL1 = CALBC1_1MHZ;              // Set DCO
    DCOCTL  = CALDCO_1MHZ;
}
