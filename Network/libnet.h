/** fichier libnet.h **/

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets.                    **/
/******************************************************************/
#ifndef LIBNET_HEADER
#define LIBNET_HEADER


/**** Constantes ****/



/**** Fonctions ****/

#ifdef DEBUG
void displaySocketAddress(  FILE* output, int socket);
void displayClientAddress( FILE* output, int socket);
void afficheAdresse(FILE *f,struct in_addr);
void afficheAdresseSocket(FILE *,struct sockaddr_in *);
void afficheHote(FILE *,struct hostent *);
#endif
int nomVersAdresse(char *,struct in_addr *);
int socketVersNom(int,char *);
int connexionServeur(char *hote,int port);
int initialisationServeur( short int *port , int connexions);
int boucleServeur(int,void (*)(int));
int creationInterfaceVirtuelle(char *);



#endif