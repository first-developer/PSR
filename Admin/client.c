/** fichier commutateur.c **/

/******************************************************************/
/** Commutateur virtuel pour etablir des VPN                     **/
/******************************************************************/

/** Fichiers d'inclusion **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <getopt.h>

#include "../Network/libnet.c"
#include "commutateur.h"
#include "../Logger/logger.h"
#include "commandesAdmin.h"

#define TAP_PRINCIPAL	"/dev/net/tun"
#define BUFFER_SIZE		256

// Server constant variables 
// ----------------------------------
#define SERVER_IP_ADDR  "127.0.0.1"
#define SERVER_PORT     5000

/** Procedure principale **/

int main(int argc,char **argv) {

	struct sockaddr_in adresse; 	/* Adresse du serveur distant */
	char 	statut;   				/* Stocke le statut des primitives */
	int 	connected_socket;    	/* Descripteur de la SOCKET renvoyer par le serveur */
	char 	buffer[BUFFER_SIZE];
	FILE* 	connected_socket_file;
	char* 	request[REQUEST_SIZE];

	// Connection au serveur
	connected_socket=connexionServeur(SERVER_IP_ADDR, SERVER_PORT);
#ifdef DEBUG
	if(connected_socket<0){ perror("Main.connexionServeur"); exit(-1); }
#endif

	connected_socket_file = fdopen(connected_socket, "w+");
	if (connected_socket_file == NULL) {
		perror("Main.fdopen");
		exit(-1);
	}

	// Start requesting the server side
	while (1) {
		scanf("%s", request);

		if (!strcmp(request, "stop")) { // si on veut arrete
			break;
		}
	}
	fprintf(connected_socket_file, "lport\n");
	fprintf(connected_socket_file, "dport\n");

	return EXIT_SUCCESS;
}
