
// Inclusions
// ----------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#include "gestionAdmin.h"
#include "logger.h"
#include "commutateur.h"
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

void process_request ( char* request, int adminKey) {
	int cmd_id;
	char params[REQUEST_SIZE];

	// on recpere l' id de la cmd in fonction de la requete faite
	scan_command_name_and_params_from_request (&cmd_id, params, request) ;
	if  (cmd_id == NO_CMD_ID) {
		err_log("process_request.scan_command_name_and_params_from_request")
		exit(EXIT_FAILURE);
	};

	switch(cmd_id) {
		case LIST_PORT_CMD_ID:
			// le commutateur est censé être initialisé
			list_port_on_commutator_with_status (&commutateur);		
			break;
		case SHOW_PORT_INFOS_CMD_ID:
			show_port_infos(params, &commutateur);
			break;
		case LIST_PORT_ADDRESSES_CMD_ID:
			break;
		case TAP_CONNECTION_CMD_ID:
			break;
		case TCP_CONNECTION_CMD_ID:
			break;
		case SET_PORT_VLAN_CMD_ID:
			break;
		case DISCONNECT_PORT_CMD_ID:
			break;
		case SHOW_PORT_STAT_CMD_ID:
			break;
		case TRIGGER_EVENTS_CMD_ID:
			break;
		case SNIFFER_PORT_CMD_ID:
			break;
		case STOP_CMD_ID:
			break;
		default :
			err_log(" la commande que vous avez rentrée n'existe pas!")
			print_newline()
			help("Voici la liste des commandes :")
			display_command_list(stdout);
			break; 
	}
}

// scan_command_name_and_params_from_request: attribut un id de cmd et recupere les params liés
void scan_command_name_and_params_from_request( int* cmd_id, char* params, char* request) {
	char cmd[REQUEST_SIZE];

	if (sscanf(request, "%s %s", cmd, params) == 1) {
		err_log(("getCommandAndParamsForRequest.sscanf"))
		exit(EXIT_FAILURE);
	}

	if (!strcmp (cmd, LIST_PORT_CMD 		  	  )) { *cmd_id = LIST_PORT_CMD_ID; 			}
	else if (!strcmp (cmd, SHOW_PORT_INFOS_CMD	  )) { *cmd_id = SHOW_PORT_INFOS_CMD_ID;	}													
	else if (!strcmp (cmd, LIST_PORT_ADDRESSES_CMD)) { *cmd_id = LIST_PORT_ADDRESSES_CMD_ID;}												
	else if (!strcmp (cmd, TAP_CONNECTION_CMD	  )) { *cmd_id = TAP_CONNECTION_CMD_ID;		}											
	else if (!strcmp (cmd, TCP_CONNECTION_CMD	  )) { *cmd_id = TCP_CONNECTION_CMD_ID;		}													
	else if (!strcmp (cmd, SET_PORT_VLAN_CMD	  )) { *cmd_id = SET_PORT_VLAN_CMD_ID;		}												
	else if (!strcmp (cmd, DISCONNECT_PORT_CMD	  )) { *cmd_id = DISCONNECT_PORT_CMD_ID;	}										
	else if (!strcmp (cmd, SHOW_PORT_STAT_CMD	  )) { *cmd_id = SHOW_PORT_STAT_CMD_ID;		}													
	else if (!strcmp (cmd, TRIGGER_EVENTS_CMD	  )) { *cmd_id = TRIGGER_EVENTS_CMD_ID;		}										
	else if (!strcmp (cmd, SNIFFER_PORT_CMD		  )) { *cmd_id = SNIFFER_PORT_CMD_ID;		}												
	else if (!strcmp (cmd, STOP_CMD				  )) { *cmd_id = STOP_CMD_ID;				}
	else { *cmd_id = NO_CMD_ID;};										
}

// display_command_list : affiche les commandes disponibles
void display_command_list( FILE* output) {
	help_command_list(output)	
}


//list_port_on_commutator_with_status: liste les port sur un commutateur
void list_port_on_commutator_with_status( Commutator * c) {
	
}

// void show_port_infos( Commutator * );
void show_port_infos( char* params, Commutator * c) {
	short int num_port = (short int) atoi(params);
	PortList port_list = c->ports;
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
void list_port_addresses( char* params, Commutator * c) {
	
}