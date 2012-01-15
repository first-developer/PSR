/** fichier gestionConnexions.c **/

/******************************************************/
/* Ce fichier contient les procedures permettant de   */
/* gerer les connexions des autres commutateurs       */
/******************************************************/

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

#include "libnet.h"
#include "libthrd.h"
#include "gestionConnexions.h"
#include "logger.h"


/** Constantes **/

#define		DEFAULT_PORT		4000
#define  	MAX_CONNEXIONS		10
#define 	MAX_SIZE			1000
#define 	BUFFER_SIZE			256
#define 	SERVER_LOCAL_PORT 	5000
#define 	SERVER_LOCAL_IP   	"127.0.0.1"


// Functions 
// ----------

void handle_connection (int connected_socket) {  // côté client
	char buffer[MAX_SIZE];
	FILE* connected_socket_file;

	// Affichage des infos de l'initiateur de la socket de connection
	log_ok(("CONNEXION"))
	fprintf(stdout,	"  %slocale   %s", BMAGENTA, BLACK);
	displaySocketAddress(stdout, connected_socket);

	// traitement de la socket de dialogue sous forme de fichier
	connected_socket_file = fdopen(connected_socket, "w+");
	if ( connected_socket_file < 0 ) {
		err_log(("traitement.fdopen"))
		exit(1);
	}

	// Affichage des infos du client sur la machine local
	fprintf(stdout, "  %sdistante%s ", BMAGENTA, BLACK);
	displayClientAddress(stdout, connected_socket);

	// Affichage des infos du client sur la machine distante
	fprintf(connected_socket_file, "%sCLIENT %s ", BRED, BLACK);
	displayClientAddress(connected_socket_file, connected_socket);  // Affichage sur la machine distante
	

	fflush(connected_socket_file);  // clean buffer
	
	if (fgets(buffer, BUFFER_SIZE, connected_socket_file )) {
		// TODO: filtrer pour recupere le port 
		if (!strcmp(buffer, "lport")) {
			fprintf(connected_socket_file, "    %s>> %s%s\n", BBLACK, "OK", BLACK);	
		}
		fprintf(stdout, "%s    > %s%s\n ", BBLACK, buffer, BLACK);
	}
	
	log_ok(("DECONNEXION"))
	end_log()
	fclose(connected_socket_file);
}


// process_activity_for: lance un processus leger pur effectuer un traitement
void process_slight_activity_for( int contact_socket) {
  lanceThread(handle_connection, contact_socket);
}


/** Fonction principale de gestion d'une connexion **/
