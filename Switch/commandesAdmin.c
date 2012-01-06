
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

	scan_command_name_and_params_from_request (&cmd_id, params, request);

	switch(cmd_id) {
		case LIST_PORT_CMD_ID:
			// mainCommutator expected to be declare before this call
			list_port_on_commutator_with_status (&mainCommutator);		
			break;
		case SHOW_PORT_INFOS_CMD_ID:
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

// scan_command_name_and_params_from_request: just to get cmd name and params from the request
void scan_command_name_and_params_from_request( int* cmd_id, char* params, char* request) {
	char cmd[REQUEST_SIZE];

	if (sscanf(request, "%s %s", cmd, params) == 1) {
		err_log(("getCommandAndParamsForRequest.sscanf"))
		exit(EXIT_FAILURE);
	}

	if (!strcmp (cmd, LIST_PORT_CMD)) {}
}

// display_command_list : affiche les commandes disponibles
void display_command_list( FILE* output) {
	help_command_list(output)	
}


//list_port_on_commutator_with_status: liste les port sur un commutateur
void list_port_on_commutator_with_status( Commutator * c) {
	
}