
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
	help_command_list(output)	
}


//list_port_on_commutator_with_status: liste les port sur un commutateur
void list_port_on_commutator_with_status() {
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

/*
void Admin_TCP_connect( char* params, Commutator * c) {
	// recuperer les parametres utile pour la requête 
	int lport, TCP_port, dport;
	char machine[REQUEST_SIZE];
	Port* Lport;
	int 	connected_socket;    	// Descripteur de la SOCKET renvoyer par le serveur 
	FILE* 	connected_socket_file;

	// 
	if (sscanf(params, "%d:[%d@]%s:%d", &lport, &TCP_port, machine, &dport) == 1) {
		err_log(("TCP_connect.sscanf"))
		exit(EXIT_FAILURE);
	}

	// recuperer le port local du commutateur
	Lport = get_port_from_commutator(lport);

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

	// si la connection s'est bien déroulée on active le port 
	// et on lui assigne le descripteur de la socket
	Lport->state = PORT_STATE_CONNECTED;
	Lport->socket_fd = connected_socket;

	// send lport et dport to the 
	fprintf(connected_socket_file, "lport\n");
	
	fprintf(connected_socket_file, "dport\n");

}
*/

//void connect_tcp()


