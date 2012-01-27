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

#include "gestionConnexions.h"
#include "commutateur.h"
#include "logger.h"
#include "libnet.h"
#include "libthrd.h"
#include "libipc.h"
#include "constants.h"
#include "commandesAdmin.h"

// Constants
// ----------


// Structures
// ----------


// Variables
// ---------


// Macros
// ------


// Helper Functions
// -----------------

// get_command_id_from_command_name: retourne l'id correspondant à un nom de commande
int get_command_id_from_command_name( char* cmd_name) {
	if 		(!strcmp (cmd_name, LIST_PORT_CMD 		  )) { return LIST_PORT_CMD_ID; 	 	 }
	else if (!strcmp (cmd_name, SHOW_PORT_INFOS_CMD	  )) { return SHOW_PORT_INFOS_CMD_ID;	 }				
	else if (!strcmp (cmd_name, LIST_PORT_ADDRESSES_CMD)){ return LIST_PORT_ADDRESSES_CMD_ID;}				
	else if (!strcmp (cmd_name, TAP_CONNECTION_CMD	  )) { return TAP_CONNECTION_CMD_ID;	 }					
	else if (!strcmp (cmd_name, TCP_CONNECTION_CMD	  )) { return TCP_CONNECTION_CMD_ID;	 }	
	else if (!strcmp (cmd_name, SET_PORT_VLAN_CMD	  )) { return SET_PORT_VLAN_CMD_ID;		 }	
	else if (!strcmp (cmd_name, DISCONNECT_PORT_CMD	  )) { return DISCONNECT_PORT_CMD_ID;	 }	
	else if (!strcmp (cmd_name, SHOW_PORT_STAT_CMD	  )) { return SHOW_PORT_STAT_CMD_ID;	 }	
	else if (!strcmp (cmd_name, TRIGGER_EVENTS_CMD	  )) { return TRIGGER_EVENTS_CMD_ID;	 }	
	else if (!strcmp (cmd_name, SNIFFER_PORT_CMD	  )) { return SNIFFER_PORT_CMD_ID;		 }	
	else if (!strcmp (cmd_name, STOP_CMD			  )) { return STOP_CMD_ID;				 }
	else { return NO_CMD_ID;};										
}


// lister: liste les ports d'un commutateur
void lister(char* response) {
	Port* ports = commutateur.ports;
	char str[MAX_BUFFER_SIZE];
	int i;
	for (i=0; i<NBR_MAX_PORT; i++)  {
		strcpy(str, get_port_infos(ports[i]));
		strcat(response, str);
	}
}



/*
// TCP_connect: connecter un lport à un port de comutateur distant
void Admin_TCP_connect( char* params, int AdminResponseQueueID) {
	// recuperer les parametres utile pour la requête
	char help_message[MAX_BUFFER_SIZE];
	int lport, TCP_port, dport;
	char machine[REQUEST_SIZE];
	char message[REQUEST_SIZE]; // port envoyer lport et dport

	Port* Lport, *Dport;
	int connected_socket; // Descripteur de la SOCKET renvoyer par le serveur
	FILE* connected_socket_file;

	//
	if (sscanf(params, "%d:[%d@]%s:%d", &lport, &TCP_port, machine, &dport) == 1) {
		err_log(("TCP_connect.sscanf"))
		build_help_string(help_message)
		// TODO: finir de repondre au client du message SYNTAX
		IPC_send_message( AdminResponseQueueID, 1, SYNTAX_MSG ) ;
		// envoie d'un message contenat l'aide
		IPC_send_message( AdminResponseQueueID, 1, help_message ) ;
		exit(EXIT_FAILURE);
	}

	// Verification ds paramètres et envoie de message si erreur de type
	// ou non validité
	if (lport < 0 || dport < 0 || lport > NBR_MAX_PORT || dport > NBR_MAX_PORT ) {
		log("Envoie du message BOUNDS à l'admin client")
		IPC_send_message( AdminResponseQueueID, 1, BOUNDS_MSG) ;
	}
	else {
		// recuperer le port local du commutateur
		Lport = get_port_from_commutator(lport);
		Dport = get_port_from_commutator(dport);

		// ----------- start connection -------------
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

		sprintf(message, "lport %d\n", lport); // on prepare le lport
		// send lport et dport to the
		fprintf(connected_socket_file, message);
		fflush(connected_socket_file); // clean buffer

		if (fgets(message, REQUEST_SIZE , connected_socket_file )) {
			// TODO: filtrer pour recupere le port
			if (!strcmp(message, OK_MSG)) {
				sprintf(message, "dport %d\n", dport); // on prepare le dport
				// send lport et dport to the
				fprintf(connected_socket_file, message);

				// reception du OK du dport
				if (fgets(message, REQUEST_SIZE, connected_socket_file )) {
					if (!strcmp(message, OK_MSG)) {

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
			log("pas de reception du OK (lport")
		}
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
		IPC_send_message( mqueueID, 1, "connect_TAP: port deja connecte" );
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

*/