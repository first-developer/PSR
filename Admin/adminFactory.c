// Inclusions
// ----------

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


#include "logger.h"
#include "libnet.h"
#include "libipc.h"
#include "libthrd.h"
#include "gestionConnexions.h"
#include "gestionAdmin.h"
#include "commutateur.h"
#include "commandesAdmin.h"
#include "adminFactory.h"



// Constants
// ----------

#define TAP_PRINCIPAL	"/dev/net/tun"
#define BUFFER_SIZE		256

// Server constant variables 
// ----------------------------------
#define SERVER_IP_ADDR  "127.0.0.1"
#define SERVER_PORT     5000


// variables
// ---------


// Structures
// ----------


// Macros
// ------



// Functions
// ---------




// Principale
// ----------

int main(int argc,char **argv) {

	int 	connected_socket;    	/* Descripteur de la SOCKET renvoyer par le serveur */
	FILE* 	connected_socket_file;
	char 	request[REQUEST_SIZE];

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

	// Création de la file de message reponse côté client
	int reponseQueueID = generate_reponse_queue_id();
	
	while (1) {
		scanf("%s", request);

		if (!strcmp(request, "stop")) { // si on veut arrete
			break;
		}
		IPC_send_message( reponseQueueID, ADMIN_REQUEST_TYPE, request );
	}
	
	fprintf(connected_socket_file, "lport\n");
	fprintf(connected_socket_file, "dport\n");

	return EXIT_SUCCESS;
}
