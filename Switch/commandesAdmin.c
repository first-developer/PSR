
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
#include <string.h>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "gestionAdmin.h"
#include "commutateur.h"
#include "logger.h"
#include "libnet.h"
#include "libthrd.h"
#include "libipc.h"
#include "commandesAdmin.h"

// Constants
// ----------


// Structures
// ----------


// Variables
// ---------


// Macros
// ------


// Functions
// ---------


// display_command_list : affiche les commandes disponibles
void display_command_list( FILE* output) {
	// TODO: Envoyer plutot la chaine de caractere au client
	help_command_list(output)	
}


//list_port_on_commutator_with_status: liste les port sur un commutateur
void list_port_on_commutator_with_status() {
	// TODO: Envoyer plutot la chaine de caractere au client
	display_all_commutator_ports();
}

// void show_port_infos( Commutator * );
void show_port_infos( char* Args) {
	short int num_port = (short int) atoi(Args);
	PortList port_list = commutateur.ports;
	Port *port;
	int i;
	for (i=0; i<NBR_MAX_PORT; i++) /*&& (&port_list.list[i] != NULL*/ {
		port = &(port_list.list[i]);
		if ( port->num == num_port) {
			display_port_infos(port, stdout)
			break;
		}
	}
}

// list_port_addresses: liste les adresses d'un port
void list_port_addresses( char* Args) {
	
}

// TCP_connect: connecter un lport à un port de comutateur distant


void Admin_TCP_connect( char* params, Commutator * c) {
	// recuperer les parametres utile pour la requête 
	int lport, TCP_port, dport;
	char machine[REQUEST_SIZE];
	char message[REQUEST_SIZE]; // port envoyer lport et dport

	Port* Lport, *Dport;
	int 	connected_socket;    	// Descripteur de la SOCKET renvoyer par le serveur 
	FILE* 	connected_socket_file;

	// 
	if (sscanf(params, "%d:[%d@]%s:%d", &lport, &TCP_port, machine, &dport) == 1) {
		err_log(("TCP_connect.sscanf"))
		exit(EXIT_FAILURE);
	}

	// recuperer le port local du commutateur
	Lport = get_port_from_commutator(lport);
	Dport = get_port_from_commutator(dport);

	//  -----------  start connection -------------
	log("initialisation de la connexion TCP")

	// Connection au serveur
	connected_socket=connexionServeur(machine, TCP_port);
	if(connected_socket<0){ 
		err_log("Main.connexionServeur")
		exit(EXIT_FAILURE);
	}

	connected_socket_file = fdopen(connected_socket, "w+");
	if (connected_socket_file == NULL) {
		err_log(("Main.fdopen"))
		exit(EXIT_FAILURE);
	}

	sprintf(message, "lport %d\n", lport);  // on prepare le lport
	// send lport et dport to the 
	fprintf(connected_socket_file, message);
	fflush(connected_socket_file);  // clean buffer

	if (fgets(message, REQUEST_SIZE	, connected_socket_file )) {
		// TODO: filtrer pour recupere le port 
		if (!strcmp(message, "OK")) {
			sprintf(message, "dport %d\n", dport);  // on prepare le dport
			// send lport et dport to the 
			fprintf(connected_socket_file, message);

			// reception du OK du dport
			if (fgets(message, REQUEST_SIZE, connected_socket_file )) {
				if (!strcmp(message, "OK")) {

					// TODO: poser un verrou avant la modification des ports
					P(lport);
					P(dport);
					// mise a jour des deux ports
					Lport->state = PORT_STATE_CONNECTED;
					Lport->state = PORT_STATE_CONNECTED;

					// Assignation du descripteur de la socket
					Lport->socket_fd = connected_socket;
					Dport->socket_fd = connected_socket;
					V(lport);
					V(dport);
					// TODO: liberer le verrou apres la modification des ports
				}
			}
			else {
				log("pas de reception du OK (dport")
			}
		}
	}
	else {
		log("pas de reception du OK (dport")
	}

}

void connect_TAP ( int port, int mqueueID) {
	char nom_intf[REQUEST_SIZE];
	int intf_fd;
	// recuperation du port 
	Port* p = get_port_from_commutator(port);

	// Verifier l'état du port 
	if ( p->state == PORT_STATE_CONNECTED) {
		err_log("connect_TAP: port deja connecte")
		IPC_send_message( mqueueID, 1, "connect_TAP: port deja connecte"  );
	}
	else { // port libre
		// TODO: poser le verrou sur le port
		P(port);
		p->state = PORT_STATE_CONNECTED;
		intf_fd = creationInterfaceVirtuelle(nom_intf);
		V(port);
		// TODO: mettre le nom de l'intf dans la structure (à voir)
		// TODO: Envoie le nom au client par msg
	}

} 


void vlan (int port, int vlan) {
	// recuperation du port 
	// verrou
	P(port);
	Port* p = get_port_from_commutator(port);
	p->vlan = vlan;
	V(port);
	// leve le verrou
}

