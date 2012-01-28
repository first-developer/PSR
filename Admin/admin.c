
// ==========================
//	CLIENT AMDIN
// ==========================



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
#include "admin.h"
#include "commandesAdmin.h"


// Structures 
// ----------


// Variables
// ----------

Response serverResponse;
char requestParams[2][BUFFER_SIZE];
int AdminResponseID;
int RequestID;

// External variables
// -------------------
extern char *optarg;
extern int 	optind,	opterr,	optopt;


// Fonctions annnexes
// -------------------


// process_admin_requests:
void process_admin_requests(int argc,char **argv) {
	int option_index = 0;
	char cmd_name[BUFFER_SIZE];  // pour stocker le nom dela requete
	int CMD_ID;

	// initialisation du nom de la commande par NO_CMD (commande par default) 
	strcpy(cmd_name, NO_CMD);

	char optionArg; // Main option

	static struct option long_options[] = {
			{HELP_CMD,					NOT_REQUIRED,	NULL,	'h'},
			{LIST_PORT_CMD,				NOT_REQUIRED,	NULL,	'l'},
			{SHOW_PORT_INFOS_CMD,		REQUIRED,		NULL,	'a'},
			{LIST_PORT_ADDRESSES_CMD,	REQUIRED,		NULL,	'A'},
			{TAP_CONNECTION_CMD,		REQUIRED,		NULL,	'c'},
			{TCP_CONNECTION_CMD,		REQUIRED,		NULL,	'C'},
			{SET_PORT_VLAN_CMD,			REQUIRED,		NULL,	'v'},
			{DISCONNECT_PORT_CMD,		REQUIRED,		NULL,	'd'},
			{SHOW_PORT_STAT_CMD,		REQUIRED,		NULL,	's'},
			{TRIGGER_EVENTS_CMD,		NOT_REQUIRED,	NULL,	0},
			{SNIFFER_PORT_CMD,			NOT_REQUIRED,	NULL,	0},
			{STOP_CMD,					NOT_REQUIRED,	NULL,	0},
			{0, 0, 0, 0}
	};

	char* shortOptsList = "hla:A:c:C:v:s:	";
	char helpMsg[MAX_BUFFER_SIZE];
	// Creation de l'id de la file de reponse du client 
	AdminResponseID = create_client_response_id();

	// Verifiaction du nombre d'arguments pour la preparation des requetes
	if (nbr_args_is_valid(argc)) {
		do {
			optionArg = getopt_long(argc, argv, shortOptsList, long_options, &option_index) ;
				
			
			switch(optionArg) {
				case    0:
					// recuperation de  l'index de la commande correspondant 
					CMD_ID = get_command_id_from_command_name((char *)long_options[option_index].name);
					switch(CMD_ID) {
						case TRIGGER_EVENTS_CMD_ID:
							strcpy(cmd_name, TRIGGER_EVENTS_CMD); 
							break;
						case SNIFFER_PORT_CMD_ID:
							strcpy(cmd_name, SNIFFER_PORT_CMD); 
							break;
						case STOP_CMD_ID:
							strcpy(cmd_name, STOP_CMD); 
							break;
						default:
						break;
					}
					break;
				case 'h':
					build_help_string(helpMsg) 
					log(helpMsg, stderr)
					break;
				case 'l':
					process_request_and_wait_response(LIST_PORT_CMD, EMPTY_PARAM);
					break;
				case 'a':
					process_request_and_wait_response(SHOW_PORT_INFOS_CMD, optarg);
					break;
				case 'A':
					process_request_and_wait_response(LIST_PORT_ADDRESSES_CMD, optarg);
					break;
				case 'c':
					strcpy(cmd_name, TAP_CONNECTION_CMD); 
					break;
				case 'C':
					strcpy(cmd_name, TCP_CONNECTION_CMD); 
					break;
				case 'v':
					strcpy(cmd_name, SET_PORT_VLAN_CMD); 
					break;
				case 'd':
					strcpy(cmd_name, DISCONNECT_PORT_CMD); 
					break;
				case 's':
					process_request_and_wait_response(SHOW_PORT_STAT_CMD, optarg);
					break;					
				case  '?':
					log("commande(s) inconnue(s). ", stderr)
					build_help_string(helpMsg) 
					log(helpMsg, stderr)
					break;
				default:
					break;
			}
				
			
		} while( optionArg != -1);

		// process admin request
		// process_request(cmd_name, optarg, AdminResponseID);
	}
}


// validates_arguments: verifie la validité des arguments passés par l'admin
void validates_arguments(char* Args) {
	
}

// process_request_and_wait_response()
void process_request_and_wait_response(char* cmd_name, char* params) {
	// Display log of request sending
	log("# Sending request ...", stderr	)	
	show_cmd_details(cmd_name, params)
	// send request 
	send_request_to_server( AdminResponseID, cmd_name, params);
	log("# Request sent.", stderr)
	
	log("# Waiting response from server ...", stderr)
	// get response 
	serverResponse = get_respond_from_server(AdminResponseID);
	
	// display response getting from the server
	show_response_details(serverResponse.body)
	sep()
}


// nbr_args_is_valid: verification du nombre d'arguments passé
int nbr_args_is_valid(int argc) {
	if (argc == 1 ) {
		log("Vous devez passer des arguments en paramètres", stderr)
		return FALSE;
	}
	return TRUE;
}

// void build_request_params
void build_request_params( char * cmd_name, char * args) {
	strcpy(requestParams[0], cmd_name);
	strcpy(requestParams[1], args);
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


// Main
// -----


int main(int argc,char **argv) {
	process_admin_requests(argc, argv);
	return EXIT_SUCCESS;
}
