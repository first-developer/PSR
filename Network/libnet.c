/** fichier libnet.c **/

/************************************************************/
/** Ce fichier contient des fonctions utilisees par le     **/
/** serveur de commandes et concernant les sockets.        **/
/************************************************************/


// Fichiers d'inclusion
// --------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include "gestionConnexions.h"
#include "libnet.h"
#include "logger.h"


// Constants
// ---------

#define 	TAP_PRINCIPAL	"/dev/net/tun"
#define 	BUFFER_SIZE		256
#define 	h_addr 			h_addr_list[0]

// parametres serveur
#define SERVER_LOCAL_PORT 5000
#define SERVER_LOCAL_IP   "127.0.0.1"


// Global variables 
// -----------------


// Fonctions de gestion des sockets 
// -------------------------------------

#ifdef DEBUG
/** Impression d'une adresse generale **/

void afficheAdresse(FILE *flux,struct in_addr a)
{
	fprintf(flux,inet_ntoa(a));
} 

/** Impression d'une adresse de socket **/

void afficheAdresseSocket(FILE *flux,struct sockaddr_in *sa)
{
	fprintf(flux,"Adresse IP : ");
	afficheAdresse(flux,sa->sin_addr);
	fprintf(flux,"\nPort de la socket : %d.\n",ntohs(sa->sin_port));
}

/** Impression des informations d'un hote **/

void afficheHote(FILE *flux,struct hostent *hote)
{
	char **params;

	fprintf(flux,"Nom officiel : '%s'.\n",hote->h_name);
	fprintf(flux,"Surnoms: ");
	for(params=hote->h_aliases;*params!=NULL;params++){
		fprintf(flux,"%s",*params);
		if(*(params+1)==NULL) fprintf(flux,",");
	}
	fprintf(flux,"\n");
	fprintf(flux,"Type des adresses   : %d.\n",hote->h_addrtype);
	fprintf(flux,"Taille des adresses : %d.\n",hote->h_length);
	fprintf(flux,"Adresses: ");
	for(params=hote->h_addr_list;params[0]!=NULL;params++){
		afficheAdresse(flux,*(struct in_addr *)params);
		if((*params+1)!=NULL) fprintf(flux,",");
	}
	fprintf(flux,"\n");
}
#endif

/** Cherche l'adresse IP d'un hote (0 sur succes et -1 sinon) **/

int nomVersAdresse(char *hote,struct in_addr *adresse)
{
	if (inet_aton((const char *)hote, adresse) == 0) {
#ifdef DEBUG
		perror("nomVersAdresse:inet_aton echec de l'initialisation de l'adresse de l'hote.\n");
#endif
		return -1;
	};
	return 0;
}

// displaySocketAddress: Affiche les infos sur l'initiateur de la socket de connexion
void displaySocketAddress(  FILE* output, int socket) {
	struct sockaddr_in address;
	socklen_t	socket_size;

	socket_size = sizeof(address);

	if (getsockname(socket, (struct sockaddr *)&address, &socket_size)) {
		err_log(("displaySocketAddress.getsocketname"))
		exit(1);
	}

	//fprintf(output, "  - IP   = %s%s%s\n",BBLUE, inet_ntoa(address.sin_addr), BLACK);
	//fprintf(output, "  - Port = %s%u%s\n",BBLUE, ntohs(address.sin_port), BLACK);
	fprintf(output, " [ IP   = %s%s%s, ",BBLUE, inet_ntoa(address.sin_addr), BLACK);
	fprintf(output, " Port = %s%u%s ]\n",BBLUE, ntohs(address.sin_port), BLACK);
}

// displaySocketAddress: Affiche les infos sur l'initiateur de la socket de connexion
void displayClientAddress( FILE* output, int socket) {
	struct sockaddr_in address;
	socklen_t	socket_size;

	socket_size = sizeof(address);

	// Recupération des infos sur le client
	if (getpeername(socket, (struct sockaddr*) &address, &socket_size) < 0)
	{
#ifdef DEBUG
		err_log(("displayClientAddress.getpeername"))
#endif
		exit(1);
	}

	//fprintf(output, "  - IP   = %s%s%s\n",BBLUE, inet_ntoa(address.sin_addr), BLACK);
	//fprintf(output, "  - Port = %s%u%s\n",BBLUE, ntohs(address.sin_port), BLACK);
	fprintf(output, " [ IP   = %s%s%s, ",BBLUE, inet_ntoa(address.sin_addr), BLACK);
	fprintf(output, " Port = %s%u%s ]\n",BBLUE, ntohs(address.sin_port), BLACK);
}



/** Retourne le nom de la machine connectee sur la     **/
/** socket distante d'un descripteur                   **/

int socketVersNom(int ds,char *nom)
{
	struct sockaddr_in socket_address;
	socklen_t socket_size;
	struct hostent* Host;
	socket_size = sizeof(socket_address);

	// Recupération des infos sur le client
	if (getpeername(ds, (struct sockaddr*) &socket_address, &socket_size) < 0)
	{
#ifdef DEBUG
		err_log(("socketVersNom.getpeername"))
#endif
		return -1;
	}

	// recuperation de la taille de l'adresse
	int addr_size = sizeof(socket_address.sin_addr);

	// Recuppération du nom de la machine connectée 
	Host = gethostbyaddr( (const char*)&(socket_address.sin_addr), addr_size, AF_INET );
	if ( Host == NULL ) {
		// recuperation du nom de l'hôte par default 
		strcpy(nom, inet_ntoa(socket_address.sin_addr));
		return -1;
	}

	// Recupération du nom de la machine connectée
	strcpy(nom, Host->h_name);
	return 0;
}

/** Ouvre une socket sur un hote et un port determine   **/
/** retourne le descripteur de la socket ou -1 (erreur) **/

int connexionServeur(char *hote,int port)
{
	int client_socket;
	struct sockaddr_in server_socket_name;
	struct in_addr adresseIP;
	int statut;

	// Creation de la socket du client 
	// -------------------------------
	client_socket=socket(PF_INET,SOCK_STREAM,0);
	if(client_socket < 0){
#ifdef DEBUG
		perror("connexionServeur.socket: la création de la socket client a échoué!\n");
#endif
		return -1;
	}

	// intialisation des paramètres de l'adresse
	// -----------------------------------------
	// Initialisation de la mémoire de la structure
	memset ((char*) &server_socket_name, 0, sizeof(struct sockaddr_in));

	// initialisation de l'adresse du server grâce à l'hote
	statut=nomVersAdresse(hote,&adresseIP);
	if( statut < 0 ) {
#ifdef DEBUG
		perror("connexionServeur.nomVersAdresse: Initialisation \
                       de l'adresse du serveur a échoué\n");
#endif
		return -1;
	}

	// Définir les données de 'server_socket_name'
	server_socket_name.sin_family = AF_INET;
	server_socket_name.sin_port = htons(port);
	server_socket_name.sin_addr=adresseIP;

	// Connexion au serveur 
	// --------------------
	if  (connect( client_socket, 
			(const struct sockaddr *)&server_socket_name,
			sizeof(server_socket_name)
	) < 0
	) {
#ifdef DEBUG
perror("connexionServeur.connect: la connexion au serveur a échoué! \n");
#endif
return(-1);
	}

	return client_socket;
}

/** DeconnexionServeur: Clôt la connexion avec le serveur
 * @params: int client_socket
 * @return: 0:success, -1:error
 */
/*
int deconnexionServeur ( int client_socket ) {
  int statut;
  FILE* f;
  f = fdopen(client_socket, "r");
  if ( (statut=shutdown(client_socket, SHUT_RDWR)) < 0) {
#ifdef DEBUG
    perror("deconnexionServeur.shutdown: petit souci avec la deconnexion.\n");
#endif
    return -1;
  };
  if ( fclose(f)<0 ) {
#ifdef DEBUG
    perror("deconnexionServeur.close: petit souci avec le fichier lié socket client.\n");
#endif
    return(-1);
  };
  return statut;
}

 */


/** Ouvre une socket en lecture et retourne le numero    **/
/** du port utilise. La fonction retourne le descripteur **/
/** de socket ou -1 en cas d'erreur.                     **/

int initialisationServeur(short int *port , int connexions)
{
	int contact_socket;
	struct sockaddr_in server_socket_addr;
	int statut;
	socklen_t socket_size=sizeof(server_socket_addr);

	// Creation de la socket côté serveur
	contact_socket = socket(PF_INET,SOCK_STREAM,0);
	if  (contact_socket < 0) {
#ifdef DEBUG
		perror("initialisationServeur.socket: Erreur de création de la socket côté serveur.\n");
#endif
		return(-1);
	}

	// Definition des paramètres de la socket serveur
	server_socket_addr.sin_family=AF_INET;
	server_socket_addr.sin_addr.s_addr= INADDR_ANY;
	server_socket_addr.sin_port=htons(*port);

	// Affectation d'une identité à notre socket
	statut=bind( contact_socket, (struct sockaddr *)&server_socket_addr, socket_size);
	if ( statut < 0 ) { 
		close(contact_socket); // fermeture de la socket
#ifdef DEBUG
		perror("initialisationServeur.bind: Erreur dans l'attribution du nom de la socket serveur.\n");
#endif
		return -1;
	}
	// Recuperation de l'adresse et du port auquel est liée la socket 'contact_socket'
	statut=getsockname(contact_socket,(struct sockaddr *)&server_socket_addr,&socket_size);
	if(statut<0){
		perror("initialisationServeur.getsockname: erreur de récupération du port utilisé");
		exit(-1);
	}
	*port=ntohs(server_socket_addr.sin_port);


	// Mise en écoute sur la socket du serveur
	statut=listen( contact_socket, connexions);
	if( statut < 0 ) {
#ifdef DEBUG
		perror("initialisationServeur.listen: Erreur d'écoute du serveur.\n");
#endif
		return -1;
	}
	return contact_socket;
}

/** Implante la boucle principale d'un serveur           **/
/** Les sockets de dialogue sont passees a la fonction   **/
/** de traitement                                        **/

int boucleServeur(int contact_socket, void (*traitement)(int))
{
	struct sockaddr_in client_socket_addr;
	socklen_t socket_size = sizeof(client_socket_addr);
	int connected_socket;  // Socket de dialogue

	while(1) {
		// Attente d'une connexion client
		connected_socket = accept(contact_socket, (struct sockaddr *)&client_socket_addr, &socket_size);
		if  ( connected_socket < 0 ) {
#ifdef DEBUG
			perror("boucleServeur.accept: Impossible de créer une socket de dialogue.\n");
#endif
			return -1;
		}

		// Gestion de multi-connection
    	traitement (connected_socket);
	}
	return 0;
}

int gestionClient(int sd){

	return 0;
}

/** Ouverture d'une interface Ethernet virtuelle **/

int creationInterfaceVirtuelle(char *nom){
	struct ifreq interface;
	int fd,erreur;

	/* Ouverture du peripherique principal */
	if((fd=open(TAP_PRINCIPAL,O_RDWR))<0) return fd;

	/* Preparation de la structure */
	memset(&interface,0,sizeof(interface));
	interface.ifr_flags=IFF_TAP|IFF_NO_PI;
	if(nom!=NULL) strncpy(interface.ifr_name,nom,IFNAMSIZ);

	/* Creation de l'interface */
	if((erreur=ioctl(fd,TUNSETIFF,(void *)&interface))<0){ close(fd); return erreur; }

	/* Recuperation du nom de l'interface */
	if(nom!=NULL) strcpy(nom,interface.ifr_name);

	return fd;
}
