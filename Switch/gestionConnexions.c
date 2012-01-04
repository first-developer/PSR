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
#include "libthreads.h"
#include "gestionConnexions.h"


/** Constantes **/

#define		DEFAULT_PORT		4000
#define  	MAX_CONNEXIONS		10
#define 	MAX_SIZE			1000
#define 	BUFFER_SIZE			256
#define 	SERVER_LOCAL_PORT 	5000
#define 	SERVER_LOCAL_IP   	"127.0.0.1"

// COLORS FOR DEBUG
// ----------------
#define BLACK   "\033[0;30m"
#define BLUE    "\033[0;34m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define MAGENTA "\033[0;35m"
#define BBLACK   "\033[1;30m"
#define BBLUE    "\033[1;34m"
#define BGREEN   "\033[1;32m"
#define BRED     "\033[1;31m"
#define BMAGENTA "\033[1;35m"
/** Variables externes **/




void handle_connection (int connected_socket) {  // côté client
	char buffer[MAX_SIZE];
	FILE* connected_socket_file;

	// Affichage des infos de l'initiateur de la socket de connection
#ifdef DEBUG
	fprintf(stdout, "%s<CONNEXION> :  %s\n", BGREEN,BLACK);
	fprintf(stdout,	"  %slocale   %s", BMAGENTA, BLACK);
	displaySocketAddress(stdout, connected_socket);
#endif


	// traitement de la socket de dialogue sous forme de fichier
	connected_socket_file = fdopen(connected_socket, "w+");
	if ( connected_socket_file < 0 ) {
#ifdef DEBUG
		perror("traitement.fdopen");
#endif
		exit(1);
	}

#ifdef DEBUG
	// Affichage des infos du client sur la machine local
	fprintf(stdout, "  %sdistante%s ", BMAGENTA, BLACK);
	displayClientAddress(stdout, connected_socket);

	// Affichage des infos du client sur la machine distante
	fprintf(connected_socket_file, "%sCLIENT %s ", BRED, BLACK);
	displayClientAddress(connected_socket_file, connected_socket);  // Affichage sur la machine distante
#endif

	fflush(connected_socket_file);  // clean buffer
	while (fgets(buffer, BUFFER_SIZE, connected_socket_file )>0) {
		fprintf(stdout, "%s    > %s%s\n ", BBLACK, buffer, BLACK);
		fprintf(connected_socket_file, "    %s>> %s%s\n", BBLACK, buffer, BLACK);
		break;
	}
	fprintf(stdout, "%s</DECONNEXION> %s\n", BGREEN,BLACK);
	fclose(connected_socket_file);
}

void handle_thread_by_port( int contact_socket) {
  lanceThread(handle_connection, contact_socket);
}


/** Fonction principale de gestion d'une connexion **/
