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


#include "libnet.h"
#include "libthrd.h"
#include "gestionConnexions.h"
#include "gestionAdmin.h"
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


// init_and_loop_server: 	intialise la socket de connexion du serveur
//							et effectue boucle de connexion	pour les clients
void init_and_loop_server(int argc, char**argv) {
	short int port;
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


// Create_and_init_commutator: crée et initialise le commutateur   
Commutator* create_and_init_commutator(AdminClientList ac, PortsList p) {
	Commutator * commutateur = (Commutator *) malloc(sizeof(Commutator));
	if (commutateur == NULL) {
#ifdef DEBUG
		cancel("create_and_init_commutator.malloc");
#endif
	}
	commutateur->admin_clients = ac;
	commutateur->ports = p;
	return commutateur;
}

// create_and_init_port: crée et initialise le port
Port* create_and_init_port( int num, int type, int nVlan) {
	Port* p = (Port*)malloc(sizeof(Port));
	if (p == NULL) {
#ifdef DEBUG
		cancel("create_and_init_port.malloc");
#endif
	}
	p->num = num;
	p->type = type;
	p->vlan = nVlan;								
	p->rcv_size = 0; 							 
	p->snd_size = 0;
	
	return p;	
}



// Procedure principale
// --------------------
int main(int argc,char **argv) {

	//init_and_loop_server(argc, argv);
	return 0;
}
