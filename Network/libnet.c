/** fichier libnet.c **/

/************************************************************/
/** Ce fichier contient des fonctions utilisees par le     **/
/** serveur de commandes et concernant les sockets.        **/
/************************************************************/

/**** Fichiers d'inclusion ****/

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

#include "libnet.h"

/**** Constantes ****/

#define 	TAP_PRINCIPAL	"/dev/net/tun"
#define 	BUFFER_SIZE		256
#define 	h_addr 			h_addr_list[0]


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
	if (inet_aton((const char *)hote, adresse) < 0) {
		printf("nomVersAdresse:inet_aton failed\n");
		exit(-1);
	} ;

	return 0;
}

/** Retourne le nom de la machine connectee sur la     **/
/** socket distante d'un descripteur                   **/

int socketVersNom(int ds,char *nom)
{
	struct sockaddr_in adresse;
	socklen_t socket_size;
	struct hostent* Host;
	socket_size = sizeof(struct sockaddr_in);

	// Set address params with the socket descriptor
	if (getpeername(ds, (struct sockaddr*) &adresse, &socket_size) < 0)
	{
		fprintf(stderr, "socketVersNom:getpeername error\n");
		return -1;
	}
	
	
	// get addr size
	int addr_size = sizeof(adresse.sin_addr);
	// get name of connected machine by host name 
	Host = gethostbyaddr( (const char*)&(adresse.sin_addr), addr_size, AF_INET );

	if ( Host == NULL ) {
		// get host name 
		strcpy(nom, inet_ntoa(adresse.sin_addr));
		return -1;
	}
	else {	
		// Set machine name
		strcpy(nom, Host->h_name);
		return 0;
	}
}

/** Ouvre une socket sur un hote et un port determine   **/
/** retourne le descripteur de la socket ou -1 (erreur) **/

int connexionServeur(char *hote,int port)
{
	int df;
	struct sockaddr_in adresse;
	struct in_addr adresseIP;
	int statut;

	/* Creation d'une socket */
	df=socket(PF_INET,SOCK_STREAM,0);
	if(df<0){
	  perror("connexionServeur.socket");
	  exit(-1);
	}
	adresse.sin_family = AF_INET;
	
	// Set the port of the socket 
	adresse.sin_port = htons(port);
	
	// Set IP address of the socket with host name given
	statut=nomVersAdresse(hote,&adresseIP);
	if(statut<0) return -1;
		
	if(connect(df,(struct sockaddr *)&adresse,sizeof(adresse))<0) return(-1);
	else return df;

}

/** Ouvre une socket en lecture et retourne le numero    **/
/** du port utilise. La fonction retourne le descripteur **/
/** de socket ou -1 en cas d'erreur.                     **/

int initialisationServeur(short int *port , int connexions)
{
	int df;
	struct sockaddr_in adresse;
	int statut;
	socklen_t taille=sizeof adresse;

	/* Creation d'une socket */
	df=socket(PF_INET,SOCK_STREAM,0);
	if(df<0) {
  		perror("initialisationServeur.socket");
  		exit(-1);
  	}

	/* On fixe l'adresse de la socket */
	adresse.sin_family=AF_INET;
	adresse.sin_addr.s_addr=INADDR_ANY;
	adresse.sin_port=htons(*port);
	statut=bind(df,(struct sockaddr *)&adresse,sizeof(adresse));
	if(statut<0) return -1;

	/* On recupere le numero du port utilise */
	statut=getsockname(df,(struct sockaddr *)&adresse,&taille);
	if(statut<0){
  	perror("initialisationServeur.getsockname");
  	exit(-1);
  }
	*port=ntohs(adresse.sin_port);

	/* Initialisation de l'ecoute */
	statut=listen(df,connexions);
	if(statut<0) return -1;

	return df;
}

/** Implante la boucle principale d'un serveur           **/
/** Les sockets de dialogue sont passees a la fonction   **/
/** de traitement                                        **/

int boucleServeur(int fd_socket_server, void (*traitement)(int))
{
	struct sockaddr_in adresse;
	socklen_t taille=sizeof adresse;
	int dialogue;

	while(1) {
	  /* Attente d'une connexion */
	  dialogue = accept(fd_socket_server,(struct sockaddr *)&adresse,&taille);
	  if(dialogue<0) return -1;

	  /* Passage de la socket de dialogue a la fonction de traitement */
	  traitement(dialogue);
	}
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
