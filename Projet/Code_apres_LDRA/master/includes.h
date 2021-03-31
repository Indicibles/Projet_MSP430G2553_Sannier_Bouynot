#include <includes_msp.c>
#include <UART.h>
#include <SPI.h>
#include <initLP.h>
#include <Interpreteur.h>

#define PROMPT  "\r\nmaster>"
#define CMDLEN  10

typedef unsigned char UCHAR_8;

UCHAR_8 cmd[CMDLEN];      // tableau de caracteres lie a la commande user
#define RELEASE "\r\t\tSPI-rIII162018"

#define SCK         BIT5            // Serial Clock
#define DATA_OUT    BIT6            // DATA out
#define DATA_IN     BIT7            // DATA in

