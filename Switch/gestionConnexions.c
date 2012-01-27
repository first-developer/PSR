/** fichier gestionConnexions.c **/

/******************************************************/
/* Ce fichier contient les procedures permettant de */
/* gerer les connexions des autres commutateurs */
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
#include "logger.h"
#include "constants.h"
#include "commutateur.h"
#include "gestionConnexions.h"


/** Constantes **/


// Functions
// ----------

void handle_connection (int connected_socket) { // côté client
	char buffer[BUFFER_SIZE];
	FILE* connected_socket_file;
	int status;
	int lport, dport;
	int lport_is_set=0; // pour verifier qu'on a bien lport et dport de suite

	// traitement de la socket de dialogue sous forme de fichier
	connected_socket_file = fdopen(connected_socket, "w+");
	if ( connected_socket_file < 0 ) {
		err_log(("traitement.fdopen"), stderr)
		exit(1);
	}

	// Affichage des infos du client
	display_client_address( stderr, connected_socket) ;

	fflush(connected_socket_file); // clean buffer

	if (fgets(buffer, BUFFER_SIZE, connected_socket_file )) {
		if (sscanf(buffer, "lport %d", &lport) != 1) {
			log(("# Bad local port received."), stderr)
			// envoyer un SYNTAX au commutateur pour invalider la requete
			fprintf(connected_socket_file, SYNTAX_MSG);
			log("# 'SYNTAX' sent.", stderr)
			fclose(connected_socket_file);
		}
		else {
			lport_is_set = 1; // on fait signe d'avoir reçu le lport
			log("# 'lport' received.", stderr)

			// Check the BOUNDS condition
			if (we_are_in_bounds_condition(lport, connected_socket_file)) {
				fclose(connected_socket_file);
			}
			else {
				if (fgets(buffer, BUFFER_SIZE, connected_socket_file )) {
					if (sscanf(buffer, "dport %d", &dport) != 1) {
						log(("Bad remote port received."), stderr)
						// envoyer un SYNTAX au commutateur pour invalider la requete
						fprintf(connected_socket_file, SYNTAX_MSG);
						fclose(connected_socket_file);
					}
					else {
						log("# 'dport'received.", stderr)
						status =  we_are_in_bounds_condition(dport, connected_socket_file);
						fclose(connected_socket_file);	
					}
				}
			}
		}
	}
	log("|-- Client disconnected", stderr)
	
}


int we_are_in_bounds_condition(int port, FILE* connected_socket_file) {
	// Check the BOUNDS condition
	if (port < 0 || port > NBR_MAX_PORT) {
		log(("# The port doesn't exist."), stderr)
		// envoyer un BOUNDS au commutateur pour invalider la requete
		fprintf(connected_socket_file, BOUNDS_MSG);
		log("# 'BOUNDS' sent.", stderr)		
		return TRUE;
	}
	else {
		// if everything is fine send 'ok'
		fflush(connected_socket_file); // clean buffer
		fprintf(connected_socket_file, OK_MSG);
		log("# 'OK' sent.", stderr)
		return FALSE;
	}
}

// process_activity_for: lance un processus leger pur effectuer un traitement
void process_slight_activity_for( int contact_socket) {
	lanceThread(handle_connection, contact_socket);
}
