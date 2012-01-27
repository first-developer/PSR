/** fichier commutateur.c **/

/******************************************************************/
/** Commutateur virtuel pour etablir des VPN **/
/******************************************************************/

// Inclusions
// -----------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <getopt.h>

#include "libnet.h"
#include "libthrd.h"
#include "libipc.h"
#include "gestionConnexions.h"
#include "commandesAdmin.h"
#include "gestionAdmin.h"
#include "constants.h"
#include "commutateur.h"
#include "logger.h"



// Constants
// ----------

// Variables
// -----------------
Commutator commutateur;
int clientsCount = 0;
int serverResquestID;

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

	port = NO_PORT;

	// Handle options passed to ME!
	if ( argc > 1 ) {
		if ((options = getopt_long(argc, argv, liste_opts, long_options, &option_index)) != -1 ) {
			if (options == 'p' && argc > 2) {
				port = atoi(argv[2]);
			}
			else { // is 'h' options
			log("Usage :\n  [-p | --port] <port> ", stderr)
			exit(EXIT_FAILURE);
			}
		}
	}
	return port;
}

// init_commutator_and_listen_to_connections: intialise la socket de connexion du serveur
// et effectue boucle de connexion pour les clients
void init_commutator_and_listen_to_connections(int argc, char**argv) {
	short int port;
	int contact_socket; // socket sur laquelle contacter le serveur

	// recupération du port entré par l'utilisateur
	if ( (port = get_port_from_options(argc, argv)) == NO_PORT ) {
		log(("Default port has being set"), stderr)
		port = SERVER_LOCAL_PORT;
	}

	// récuperation du descripteur de la socket serveur
	contact_socket = initialisationServeur(&port , MAX_CONNEXIONS);
	if ( contact_socket < 0 ) {
		err_log(("Main.initialisationServeur"), stderr)
		exit(EXIT_FAILURE);
	}

	// Affichage des données du serveur
	display_server_address(stdout, contact_socket);
	
	init_all_ports_of_commutator();

	// Creation de l'id de la file de requete
	serverResquestID = create_server_request_id();	
	

	// ------------------------------------------------------------------
	// TODO: run two thread for responding to clients and purge addresses
	// ------------------------------------------------------------------
	lanceThread(request_admin_handler, serverResquestID);

	// Lancement de la boucle d'écoute
	if (boucleServeur( contact_socket, process_slight_activity_for) < 0) {
		err_log("Main.boucleServeur", stderr)
		exit(EXIT_FAILURE);
	}
}



// create_and_init_port: crée et initialise le port
Port* init_port( int num) {
	Port* p = (Port*) malloc(sizeof(Port));
	// verification 
	if (p == (Port*) NULL) {
		log("init_port.get_port_by_number", stderr)
		exit(EXIT_FAILURE);
	}

	// On donne les attributs par defaut
	p->num 			= num;
	p->type 		= NO_PORT_TYPE;
	p->state 		= PORT_STATE_DISCONNECTED;
	p->vlan 		= 0;
	p->fd 			= 0;
	p->sd			= 0;
	p->rcv_size 	= 0;
	p->snd_size 	= 0;
	return p;
}


// init_all_ports_of_commutator: initialise les ports du commutateur
void init_all_ports_of_commutator () {
	Port* ports;
	ports = commutateur.ports;
	int i;
	// TODO: poser le verrou sur le commutateur 
	// P(COMMUTATOR_VERROU);
	for (i=0; i<NBR_MAX_PORT;i++) {
		ports[i] = *(init_port( i+1));
	}
	// TODO: liberer le verrou sur le commutateur 
	// V(COMMUTATOR_VERROU);
}


// get_port_by_number: recuperation du port à l'état connecté
Port get_port_by_number(int numPort) {
	Port* ports = commutateur.ports;
	int i;
	Port port;
	if (is_valid_port(numPort)) {
		for (i=0; i<NBR_MAX_PORT;i++) {
			if (ports[i].num == numPort) {
				port = ports[i];
				break;
			}
		}
	}
	else {
		log("get_port_by_number: mauvais numéro de port", stderr)
		exit(EXIT_FAILURE);
	}
	return port;
}


// add_client
void add_client (Client c) {
	Client* admins = commutateur.clients; // get all admins
	if (clientsCount < NBR_MAX_CLIENT) {
		admins[clientsCount] = c;
		clientsCount++;
	} 
	else {
		log("add_client: full clients", stderr)
		exit(EXIT_FAILURE);
	}
}

// Create_client: pour client un client admin	
Client* create_client() {
	Client* c = (Client *)malloc(sizeof(Client));
	if ( c == (Client *)NULL ) {
		err_log("create_client.malloc", stderr)
		exit(EXIT_FAILURE);
	}
	return c;
}

// is_valid_port: pour voir si un numero de port est bien entre 0 et NBR_MAX_PORT
int is_valid_port(int num) {
	return (num >0 && num < NBR_MAX_PORT) ? TRUE : FALSE;
}


// Procedure principale
// --------------------
int main(int argc,char **argv) {
	init_commutator_and_listen_to_connections(argc, argv);
	return 0;
}