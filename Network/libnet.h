/** fichier libnet.h **/
#ifndef LIBNET_H
#define LIBNET_H
/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets.                    **/
/******************************************************************/

// Inclusions
// ----------
#include <netdb.h>
#include <string.h>

/**** Constantes ****/

#define TAP_PRINCIPAL	"/dev/net/tun"

#define ETHERNET_STRING_MAX		18
#define ETHERNET_PACKET_MAX		1518
#define ETHERNET_ADDRESS_SIZE 	6

// Adresse ethernet
typedef struct {
  unsigned char bytes[ETHERNET_ADDRESS_SIZE];
} EthernetAddress;





/**** Fonctions ****/

#ifdef DEBUG
void afficheAdresse(FILE *f,struct in_addr);
void afficheAdresseSocket(FILE *,struct sockaddr_in *);
void afficheHote(FILE *,struct hostent *);
#endif
int nomVersAdresse(char *,struct in_addr *);
int socketVersNom(int,char *);
int connexionServeur(char *,int);
int initialisationServeur(short int *, int);
int boucleServeur(int,void (*)(int));
int creationInterfaceVirtuelle(char *);
void display_client_address( FILE* output, int socket);
void display_server_address( FILE* output, int socket);
char *ethernet_address_to_string(EthernetAddress ethernet);
#endif