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
#include "libipc.h"
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


// Variables 
// -----------------
Commutator commutateur;  // the main commutator


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
void init_commutator_and_listen_to_connections(int argc, char**argv) {
	short int port;
	int contact_socket; // socket sur laquelle contacter le serveur

	// recupération du port entré par l'utilisateur
	if ( (port = get_port_from_options(argc, argv)) == DEFAULT_PORT ) {
		exit(EXIT_FAILURE);
	}

	// récuperation du descripteur de la socket serveur
	contact_socket = initialisationServeur(&port , MAX_CONNEXIONS);
	if ( contact_socket < 0 ) {
		err_log(("Main.initialisationServeur"))
		exit(EXIT_FAILURE);
	}
	
	// Affichage des données du serveur
	start_log(("Affichage des données du serveur"))
	printf("%sSERVEUR %s", BRED, BLACK);
	displaySocketAddress(stdout, contact_socket);
	end_log()

	// Creation du commutateur  
	log((" Creation du commutateur"))
	create_and_init_commutator();

	// initialisation des ports du commutateur et affcihage
	init_commutator_ports();
	start_log("initialisation des ports du commutateur")
	display_all_commutator_ports();

	// Creation de l'id  de la file de message requête du commutateur  
	int commutatorRequestQueueID = create_responder_IPC_message_queue();
	start_log(("Affichage des données IPC du commutateur"))
	printf("CommutatorRequestQueueID: %d\n", commutatorRequestQueueID);
	
	// Lancement de la boucle d'écoute
	if (boucleServeur( contact_socket, process_slight_activity_for) < 0) {
		err_log("Main.boucleServeur");
		exit(EXIT_FAILURE);
	}
}


// Create_and_init_commutator: crée et initialise le commutateur   
void create_and_init_commutator() {
	// Commutator * commutateur = (Commutator *) malloc(sizeof(Commutator));
	// if (commutateur == NULL) {
	// 	err_log(("create_and_init_commutator.malloc"))
	// 	exit(EXIT_FAILURE);
	// }

	// On crée la list des admin et des ports
	AdminList *AL = create_admin_list();
	PortList  *PL = create_port_list();

	commutateur.admins = *AL;
	commutateur.ports  = *PL;
}

// create_and_init_port: crée et initialise le port
Port* create_and_init_port( int num) {
	Port* p = (Port*)malloc(sizeof(Port));
	if (p == NULL) {
		err_log(("create_and_init_port.malloc"))
		exit(EXIT_FAILURE);
	}

	// On donne les attributs par defaut
	p->num 		= num;
	p->type 	= NO_PORT_TYPE;
	p->state 	= 0;
	p->vlan 	= 0;	
	p->IEV_fd 	= 0;	
	p->socket_fd= 0;							
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

// init_commutator_ports: initialise les ports du commutateur
void init_commutator_ports () {
	PortList * ports = &(commutateur.ports);
	int i;

	for (i=0; i<NBR_MAX_PORT;i++) {
		ports->list[i] = (Port)*(create_and_init_port(i));
	} 
}

// void display_all_commutator_ports: affiche tous les ports du comutateur et leurs détails 
void display_all_commutator_ports() {
	PortList  ports = commutateur.ports;
	int i;
	Port* port;
	for (i=0; i<NBR_MAX_PORT;i++) {
		port = &((ports.list)[i]);
		display_port_infos(port, stderr)
	} 
}


// Procedure principale
// --------------------
int main(int argc,char **argv) {
	init_commutator_and_listen_to_connections(argc, argv);
	return 0;
}
