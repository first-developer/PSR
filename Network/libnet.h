/** fichier libnet.h **/


// On previent les inclusions multiples 
#ifdef 	HEADER_FILE
#define HEADER_FILE

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets.                    **/
/******************************************************************/

/**** Constantes ****/

/**** Fonctions ****/

#ifdef DEBUG
void afficheAdresse(FILE *f,struct in_addr);
void afficheAdresseSocket(FILE *,struct sockaddr_in *);
void afficheHote(FILE *,struct hostent *);
#endif
int nomVersAdresse(char *,struct in_addr *);
int socketVersNom(int,char *);
int connexionServeur(char *,int);
int initialisationServeur(short int *);
int boucleServeur(int,void (*)(int));
int creationInterfaceVirtuelle(char *);

#endif