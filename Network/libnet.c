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
#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include "libnet.h"

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
	// 1.Initialisation du serveur avec le 'port'
	// > verification s'il existe un port libre = 'port'
	// Renvoie le df retourné par "initialisationServeur"
	return -1;
}

/** Ouvre une socket en lecture et retourne le numero    **/
/** du port utilise. La fonction retourne le descripteur **/
/** de socket ou -1 en cas d'erreur.                     **/

int initialisationServeur(short int *port)
{
	int df;
	struct sockaddr_in adresse;
	int statut;
	socklen_t taille=sizeof adresse;

	/* Creation d'une socket */
	df=socket(PF_INET,SOCK_STREAM,0);
	if(df<0){
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

int boucleServeur(int ecoute,void (*traitement)(int))
{
	struct sockaddr_in adresse;
	socklen_t taille=sizeof adresse;
	int dialogue;

	while(1){
	  /* Attente d'une connexion */
	  dialogue = accept(ecoute,(struct sockaddr *)&adresse,&taille);
	  if(dialogue<0) return -1;

	  /* Passage de la socket de dialogue a la fonction de traitement */
	  if(traitement(dialogue)<0){ close(ecoute); return 0;}
	}
}

int gestionClient(int sd){
	int pid;
	int statut;

	pid=fork();
	if(pid<0){perror("fork"); exit(-1);}
	if(pid!=0)
		  close(sd);
	else{
  	dup2(sd,0); dup2(sd,1); dup2(sd,2);
  	statut=execl(CHEMIN_PROGRAMME,CHEMIN_PROGRAMME,NULL);
  }  
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
