#include <includes_msp.c>
#include <SPI.h>
#include <UART.h>
#define CMDLEN  10
unsigned char cmd[CMDLEN];      /* tableau de caracteres lie a la commande user*/
#define RELEASE "\rSerre connectee 2000 v1.0"
#define PROMPT  "\r\nAttente...\n"

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
         send_UART("\r\n'0' : Arret rotation servo-moteur");
         send_UART("\r\n'1' : Servo position 0");
         send_UART("\r\n'2' : Servo position 90");
         send_UART("\r\n'3' : Servo position 180");
         send_UART("\r\n'4' : afficher UV");
         send_UART("\r\n'5' : afficher humiditee");
         send_UART("\r\n'6' : afficher temperature");

     }
     else if (strcmp((const char *)cmd, "0") == 0)
     {
         send_UART("\r\n");
         send_UART("0");
         send_UART("->");
         Send_SPI(0x30); /* Send '0' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
         send_UART(" : commande envoyee\n");
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "1") == 0)
     {
         send_UART("\r\n");
         send_UART("1");
         send_UART("->");
         Send_SPI(0x31); /* Send '1' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
                 send_UART("  : commande envoyee\n");
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
         send_UART("2");
         send_UART("->");
         Send_SPI(0x32); /* Send '2' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
         send_UART("  : commande envoyee\n");
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "3") == 0)
     {
         send_UART("\r\n");
         send_UART("3");
         send_UART("->");
         Send_SPI(0x33); /* Send '3' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
         send_UART("  : commande envoyee\n");
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "4") == 0)
     {
         send_UART("\r\n");
         send_UART("4");
         send_UART("->");
         Send_SPI(0x34); /* Send '4' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
         send_UART("  : commande envoyee\n");
         send_UART("\r\n");
         send_UART("reponse : ");
         send_UART((unsigned char *) UCB0RXBUF);
         send_UART("\r\n");
     }
     else if (strcmp((const char *)cmd, "5") == 0)
     {
         send_UART("\r\n");
         send_UART("5");
         send_UART("->");
         Send_SPI(0x35); /* Send '5' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
         send_UART("  : commande envoyee\n");
         send_UART("\r\n");
         send_UART("reponse : ERR");
         send_UART("\r\n");

     }     else if (strcmp((const char *)cmd, "6") == 0)
     {
         send_UART("\r\n");
         send_UART("6");
         send_UART("->");
         Send_SPI(0x36); /* Send '6' over SPI to Slave*/
         send_UART((unsigned char *)cmd);
         send_UART("  : commande envoyee\n");
         send_UART("\r\n");
         send_UART("reponse : ERR");
         send_UART("\r\n");

     }
     else                          /*Si commande non reconnu*/
     {
         send_UART("\r\n commande non interpretee");
         send_UART((unsigned char *)cmd);
     }
     send_UART(PROMPT);        /*affiche un message notifiant que le système est en attente de réception*/
 }
