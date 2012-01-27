/******************************************************/
/* Ce fichier contient les fonctions permettant de    */
/* gerer l'agent d'administration.                    */
/******************************************************/


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
#include <stdbool.h>
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


#include "libipc.h"
#include "logger.h"
#include "constants.h"
#include "commandesAdmin.h"
#include "gestionAdmin.h"



// Varaibles 
// ---------


char cmd_name[MIN_BUFFER_SIZE];
char requestParams[BUFFER_SIZE];

// Functions 
// ----------


void request_admin_handler(int serverResquestID) {

	int cmd_id;
	Request adminRequest;
	// handle client requests 
	for (;;) {
		// switch through arg 
		log("# Waiting for clients' requests ...", stderr)
		
		// get request from admin 
		adminRequest = get_request_from_client(serverResquestID);

		// get ID of the command sent by the client 
		cmd_id = get_command_id_from_command_name(adminRequest.name);

		switch(cmd_id) {
			case LIST_PORT_CMD_ID:
				lister(adminRequest.clientResponseID);
				break;
			case SHOW_PORT_INFOS_CMD_ID:
				afficher(adminRequest.clientResponseID, adminRequest.params);
				break;
			default:
				break;
		}
	}
}



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
