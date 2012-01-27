/** fichier libnet.c **/

/************************************************************/
/** Ce fichier contient des fonctions utilisees par le     **/
/** serveur de commandes et concernant les sockets.        **/
/************************************************************/

/**** Fichiers d'inclusion ****/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <pthread.h>
#include "libnet.h"
#include "logger.h"
/**** Constantes ****/

#define TAP_PRINCIPAL	"/dev/net/tun"

/**** Variables globales *****/

/**** Fonctions de gestion des sockets ****/

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
	/* votre code ici */
	return 0;
}

/** Retourne le nom de la machine connectee sur la     **/
/** socket distante d'un descripteur                   **/

int socketVersNom(int ds,char *nom)
{
	/* votre code ici */
	return -1;
}

/** Ouvre une socket sur un hote et un port determine   **/
/** retourne le descripteur de la socket ou -1 (erreur) **/

int connexionServeur(char *hote,int port)
{
	/* votre code ici */
	return -1;
}

/** Ouvre une socket en lecture et retourne le numero    **/
/** du port utilise. La fonction retourne le descripteur **/
/** de socket ou -1 en cas d'erreur.                     **/

int initialisationServeur(short int *port , int connexions)
{
	int contact_socket;
	struct sockaddr_in server_socket_addr;
	int statut;
	socklen_t socket_size=sizeof(server_socket_addr);

	// Create contact socket from server
	contact_socket = socket(PF_INET,SOCK_STREAM,0);
	if  (contact_socket < 0) {
		err_log("initialisationServeur.socket", stderr);
		return(-1);
	}

	// Init socket params 
	server_socket_addr.sin_family=AF_INET;
	server_socket_addr.sin_addr.s_addr= INADDR_ANY;
	server_socket_addr.sin_port=htons(*port);

	// Set Socket params 
	statut=bind( contact_socket, (struct sockaddr *)&server_socket_addr, socket_size);
	if ( statut < 0 ) { 
		close(contact_socket); // fermeture de la socket
		err_log("initialisationServeur.bind", stderr);
		return -1;
	}

	// Get address and port from contact_socket'
	statut=getsockname(contact_socket,(struct sockaddr *)&server_socket_addr,&socket_size);
	if(statut<0){
		err_log("initialisationServeur.getsockname", stderr);
		exit(-1);
	}
	*port=ntohs(server_socket_addr.sin_port);

	// Put server to listen state
	statut=listen( contact_socket, connexions);
	if( statut < 0 ) {
		err_log("initialisationServeur.listen\n", stderr);
		return -1;
	}
	return contact_socket;
}

/** Implante la boucle principale d'un serveur           **/
/** Les sockets de dialogue sont passees a la fonction   **/
/** de traitement                                        **/

int boucleServeur(int contact_socket,void (*traitement)(int))
{
	struct sockaddr_in client_socket_addr;
	socklen_t socket_size = sizeof(client_socket_addr);
	int connected_socket;  // Socket de dialogue

	while(1) {
		// Wait for a connexion to the server
		connected_socket = accept(contact_socket, (struct sockaddr *)&client_socket_addr, &socket_size);
		if  ( connected_socket < 0 ) {
			err_log("boucleServeur.accept", stderr);
			return -1;
		}

		// Treat multi-connexions
		traitement(connected_socket);
	}
	return 0;
}


// display_server_address: Affiche les infos sur l'initiateur de la socket de connexion
void display_server_address(  FILE* output, int socket) {
	struct sockaddr_in address;
	socklen_t	socket_size;

	socket_size = sizeof(address);

	if (getsockname(socket, (struct sockaddr *)&address, &socket_size)) {
		err_log(("displaySocketAddress.getsocketname"), stderr)
		exit(1);
	}

	fprintf(output, "Server [ IP = %s, ", inet_ntoa(address.sin_addr));
	fprintf(output, " Port = %u ]\n", ntohs(address.sin_port));
}

// display_client_address: Affiche les infos sur l'initiateur de la socket de connexion
void display_client_address( FILE* output, int socket) {
	struct sockaddr_in address;
	socklen_t	socket_size;

	socket_size = sizeof(address);

	// Recupération des infos sur le client
	if (getpeername(socket, (struct sockaddr*) &address, &socket_size) < 0)
	{
		err_log(("displayClientAddress.getpeername"), stderr)
		exit(1);
	}

	fprintf(output, "|-- Client connected [ IP = %s, ", inet_ntoa(address.sin_addr));
	fprintf(output, " Port = %u ]\n", ntohs(address.sin_port));
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
