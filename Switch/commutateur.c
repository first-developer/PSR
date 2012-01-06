/** fichier commutateur.c **/

/******************************************************************/
/** Commutateur virtuel pour etablir des VPN                     **/
/******************************************************************/

// Inclusions
// -----------

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
#include "libthrd.h"
#include "gestionConnexions.h"
#include "gestionAdmin.h"
#include "commutateur.h"


// Constants
// ----------

#define		DEFAULT_PORT		4000
#define  	MAX_CONNEXIONS		10
#define 	MAX_SIZE			1000
#define 	BUFFER_SIZE			256
#define 	SERVER_LOCAL_PORT 	5000
#define 	SERVER_LOCAL_IP   	"127.0.0.1"


// Public variables 
// -----------------


// Static variables 
// -----------------


// Helper functions 
// ----------------

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
				start_help((" Erreur de syntax"))
				fprintf(stderr, "    %s> Syntaxe : %s%s%s [-p | --port port]%s\n", BBLACK, BLACK, argv[0], BBLUE, BLACK);
				end_log()
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
		exit(EXIT_FAILURE);
	}

	// récuperation du descripteur de la socket serveur
	contact_socket = initialisationServeur(&port , MAX_CONNEXIONS);
	if ( contact_socket < 0 ) {
		err_log(("Main.initialisationServeur: la recupération du descripteur de la socket\
	             liée au serveur a échoué sur "))
		exit(EXIT_FAILURE);
	}
	
	// Affichage des données du serveur
	start_log(("Affichage des données du serveur"))
	printf("%sSERVEUR %s", BRED, BLACK);
	displaySocketAddress(stdout, contact_socket);
	end_log()

	// Lancement de la boucle d'écoute
	if (boucleServeur( contact_socket, handle_thread_by_port) < 0) {
		err_log("Main.boucleServeur: erreur lors du lancement de l'écoute");
		exit(EXIT_FAILURE);
	}
}


// Create_and_init_commutator: crée et initialise le commutateur   
Commutator* create_commutator() {
	Commutator * commutateur = (Commutator *) malloc(sizeof(Commutator));
	if (commutateur == NULL) {
		err_log(("create_and_init_commutator.malloc"))
		exit(EXIT_FAILURE);
	}
	AdminList *AL = create_admin_list();
	PortList  *PL = create_port_list();

	commutateur->admins = *AL;
	commutateur->ports  = *PL;

	return commutateur;
}

// create_and_init_port: crée et initialise le port
Port* create_port( int num, int type, int nVlan) {
	Port* p = (Port*)malloc(sizeof(Port));
	if (p == NULL) {
		err_log(("create_and_init_port.malloc"))
		exit(EXIT_FAILURE);
	}
	p->num = num;
	p->type = type;
	p->vlan = nVlan;								
	p->rcv_size = 0; 							 
	p->snd_size = 0;
	
	return p;	
}

// create_port_list: create a list
PortList* create_port_list() {
	PortList* pl = (PortList*)malloc(sizeof(PortList));
	if (pl == NULL) {
		err_log(("create_and_init_port.malloc"))
		exit(EXIT_FAILURE);
	}
	return pl;
}




// Procedure principale
// --------------------
int main(int argc,char **argv) {
	init_and_loop_server(argc, argv);
	return 0;
}
