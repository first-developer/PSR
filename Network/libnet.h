/** fichier libnet.h **/

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets.                    **/
/******************************************************************/

/**** Constantes ****/

// COLORS FOR DEBUG 
// ----------------
#define BLACK   "\033[0;30m"
#define BLUE    "\033[0;34m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define RED     "\033[0;31m"  
#define MAGENTA "\033[0;35m" 
#define BBLACK   "\033[1;30m"
#define BBLUE    "\033[1;34m"
#define BGREEN   "\033[1;32m"
#define BYELLOW  "\033[1;33m"
#define BRED     "\033[1;31m"  
#define BMAGENTA "\033[1;35m"  










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


