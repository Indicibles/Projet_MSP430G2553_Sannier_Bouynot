#include <includes_msp.c>
#include <SPI.h>
#include <UART.h>
#define CMDLEN  10
unsigned char cmd[CMDLEN];      // tableau de caracteres lie a la commande user
#define RELEASE "\r\t\tSPI-rIII162018"
#define PROMPT  "\r\nmaster>"

/* ----------------------------------------------------------------------------
 * Fonction d'interpretation des commandes utilisateur
 * Entrees: -
 * Sorties:  -
 */void interpreteur( void )
 {
     if(strcmp((const char *)cmd, "h") == 0)          //----------------------------------- help
     {
         send_UART("\r\nCommandes :");
         send_UART("\r\n'ver' : version");
         send_UART("\r\n'0' : LED off");
         send_UART("\r\n'1' : LED on");
         send_UART("\r\n'h' : help\r\n");
     }
     else if (strcmp((const char *)cmd, "0") == 0)
     {
         send_UART("\r\n");
         send_UART((unsigned char *)cmd);
         send_UART("->");
         Send_SPI(0x30); // Send '0' over SPI to Slave
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "1") == 0)
     {
         send_UART("\r\n");
         send_UART((unsigned char *)cmd);
         send_UART("->");
         Send_SPI(0x31); // Send '1' over SPI to Slave
         send_UART("\r\n");
     }

     else if (strcmp((const char *)cmd, "ver") == 0)
     {
         send_UART("\r\n");
         send_UART(RELEASE);
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "2") == 0)
     {
         send_UART("\r\n");
         send_UART((unsigned char *)cmd);
         send_UART("->");
         Send_SPI(0x32); // Send '2' over SPI to Slave
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "3") == 0)
     {
         send_UART("\r\n");
         send_UART((unsigned char *)cmd);
         send_UART("->");
         Send_SPI(0x33); // Send '2' over SPI to Slave
         send_UART("\r\n");
     }
     else                          //---------------------------- default choice
     {
         send_UART("\r\n ?");
         send_UART((unsigned char *)cmd);
     }
     send_UART(PROMPT);        //---------------------------- command prompt
 }
