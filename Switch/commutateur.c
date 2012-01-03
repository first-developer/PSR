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
#include "commutateur.h"

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



/** Variables publiques **/

/** Variables statiques **/

/** Fonctions utiles  **/
unsigned short int get_port_from_options(int argc,char **argv) {
	int option_index = 0;
	unsigned short int port;
	char options; // Main option

	static struct option long_options[] = {
			{"port", 0, 0, 'p'},
			{"help", 0, 0, 'h'}
	};

	char* liste_opts = "p:h";

	port = SERVER_LOCAL_PORT;


	// Handle options passed to ME!
	if ( argc > 1 ) {
		if ((options = getopt_long(argc, argv, liste_opts, long_options, &option_index)) != -1 )  {
			if (options == 'p' && argc > 2) {
				port = atoi(argv[2]);
			}
			else { // is 'h' options
				fprintf(stderr, "    %sHELP > %sSyntaxe : %s%s%s [-p | --port port]%s\n", BRED, BBLACK, BLACK, argv[0], BBLUE, BLACK);
				port = DEFAULT_PORT;
			}
		}
	}
	return port;
}


//void *mortdefils (int sig) {};

// traitement 
void* handle_connection (int connected_socket) {  // côté client
	char buffer[MAX_SIZE];
	char nom[BUFFER_SIZE];
	struct sockaddr_in address;
	FILE* connected_socket_file;
	int lportIsSet = 0;  // Flag to know if lport cmt is entered

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

//	if ( strcmp(buffer, "lport") == 0 ) {
//		lportIsSet = 1;
//		//printf("%slport%s\n",BRED, BLACK);
//		sprintf(buffer, "%slport%s\n",BRED, BLACK);
//		fprintf(file_dialogue, buffer);
//	}
//	else  if ( lportIsSet && (strcmp(buffer, "dport") == 0) ) {
//		sprintf(buffer, "%sdport\n\n%sClient déconnecté%s\n",BRED, BGREEN, BLACK);
//		fprintf(file_dialogue, buffer);
//		//printf("%sdport%s\n",BRED, BLACK);
//		shutdown(socket_client,2);
//		if ( fclose(file_dialogue) < 0 ) {
//#ifdef DEBUG
//				perror("traitement.deconnexionServeur: erreur lors de la deconnexion (file_dialogue)\n");
//#endif
//			}
//		}
//		else {
//			printf(" > %s", buffer);
//		}
//	}
}

void handle_thread_by_port( int contact_socket) {
  lanceThread(handle_connection, contact_socket);
}


// init_and_loop_server: 	intialise la socket de connexion du serveur
//							et effectue boucle de connexion	pour les clients
void init_and_loop_server(int argc, char**argv) {
	unsigned short int port;
	int contact_socket; // socket sur laquelle contacter le serveur

	// recupération du port entré par l'utilisateur
	if ( (port = get_port_from_options(argc, argv)) == DEFAULT_PORT ) {
		exit(1);
	}

	// récuperation du descripteur de la socket serveur
	contact_socket = initialisationServeur(&port , MAX_CONNEXIONS);
	if ( contact_socket < 0 ) {
#ifdef DEBUG
		printf("Main.initialisationServeur: la recupération du descripteur de la socket\
	             liée au serveur a échoué sur le port [%d].\n", port);
#endif
		exit(1);
	}

	// Affichage des données du serveur
	printf("%sSERVEUR %s", BRED, BLACK);
	displaySocketAddress(stdout, contact_socket);

	// Lancement de la boucle d'écoute
	if (boucleServeur( contact_socket, handle_thread_by_port) < 0) {
#ifdef DEBUG
		perror("Main.boucleServeur: erreur lors du lancement de l'écoute.\n");
#endif
		exit(1);
	}
}
/** Procedure principale **/

int main(int argc,char **argv) {

	init_and_loop_server(argc, argv);
	return 0;
}
