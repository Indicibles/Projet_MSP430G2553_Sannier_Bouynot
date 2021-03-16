#include <includes_msp.c>
//#include <BT.c>
#include <BT.h>
//#include <UART.c>
#include <UART.h>
//#include <SPI.c>
#include <SPI.h>
#include <initLP.h>
//#include <initLP.c>

#define PROMPT  "\r\nmaster>"
#define CMDLEN  10
unsigned char cmd[CMDLEN];      // tableau de caracteres lie a la commande user
#define RELEASE "\r\t\tSPI-rIII162018"

#define SCK         BIT5            // Serial Clock
#define DATA_OUT    BIT6            // DATA out
#define DATA_IN     BIT7            // DATA in
