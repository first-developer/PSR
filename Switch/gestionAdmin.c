/******************************************************/
/* Ce fichier contient les fonctions permettant de    */
/* gerer l'agent d'administration.                    */
/******************************************************/

/** Fichiers d'inclusion **/
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

#include "libipc.h"

#include "libnet.h"
#include "libthrd.h"
#include "gestionAdmin.h"
#include "gestionConnexions.h"
#include "commandesAdmin.h"
#include "commutateur.h"
#include "logger.h"

// Constants
// ---------

// External variables
// -------------------
extern char *optarg;
extern int 	optind,
       		opterr,
       		optopt;

// Fonctions principale de gestion de l'agent d'administration 
// ------------------------------------------------------------


// process_admin_requests:
void process_admin_requests(int argc,char **argv) {
	int option_index = 0;
	char cmd_name[REQUEST_SIZE];  // pour stocker le nom dela requete
	int CMD_ID;

	// initialisation du nom de la commande par NO_CMD (commande par default) 
	strcpy(cmd_name, NO_CMD);

	char optionArg; // Main option

	static struct option long_options[] = {
			{HELP_CMD,					NOT_REQUIRED,	NULL,	'h'},
			{PORT_OPTION,				NOT_REQUIRED,	NULL,	'p'},
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

	char* shortOptsList = "hplaAcCvs";

	// Creation de l'id de la file de reponse du client 
	int AdminResponseQueueID = create_responder_IPC_message_queue();

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
					strcpy(cmd_name, HELP_CMD); 
					break;
				case 'p':
					strcpy(cmd_name, PORT_OPTION); 
					break;
				case 'l':
					strcpy(cmd_name, LIST_PORT_CMD); 
					break;
				case 'a':
					strcpy(cmd_name, SHOW_PORT_INFOS_CMD); 
					break;
				case 'A':
					strcpy(cmd_name, LIST_PORT_ADDRESSES_CMD); 
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
					strcpy(cmd_name, SHOW_PORT_STAT_CMD); 
					break;					
				case  '?':
					log("commande(s) inconnue(s). ")
					print_newline()
					help("Consultez la liste des commandes ci-dessous:")
					display_command_list(stderr);
					break;
				default:
					err_log("process_admin_requests.getopt_long")
					display_command_list(stderr);
					break;
			}
				
			// ------------------------------------------------------
			// process_request
			// ------------------------------------------------------
			process_request(cmd_name, optarg, AdminResponseQueueID);
		} while( optionArg != -1);
	}
}


// validates_arguments: verifie la validité des arguments passés par l'admin
void validates_arguments(char* Args) {
	
}


// nbr_args_is_valid: verification du nombre d'arguments passé
int nbr_args_is_valid(int argc) {
	if (argc == 1 ) {
		err_log("Vous devez passer des arguments en paramètres")
		display_command_list(stderr);
		return TRUE;
	}
	return FALSE;
}


void process_request ( char* cmd_name, char* Args, int AdminResponseQueueID) {
	// on recupere l'id de la commande 
	int cmd_id = get_command_id_from_command_name(cmd_name);

	// recuperer l'identifiant de la file de requête (commutateur)
	// int commutatorRequestQueueID = get_requester_IPC_message_queue();

	if  (cmd_id == NO_CMD_ID) {
		log("process_request.get_command_id_from_command_name: Mauvaise commande")
		exit(EXIT_FAILURE);
	};

	// un commutateur est censé être lancé
	switch(cmd_id) {
		case LIST_PORT_CMD_ID:				
			// TODO: envoyer la requete(message) 'lister' au commutateur		
		case SHOW_PORT_INFOS_CMD_ID:		
			// TODO: envoyer la requete(message) 'afficher' au commutateur
		case LIST_PORT_ADDRESSES_CMD_ID:	
			// TODO: envoyer la requete(message) 'adresses' au commutateur
		case TAP_CONNECTION_CMD_ID:			
			// TODO: envoyer la requete(message) 'connecter_tap' au commutateur
		case TCP_CONNECTION_CMD_ID:			
			// TODO: envoyer la requete(message) 'connecter_tcp' au commutateur
		case SET_PORT_VLAN_CMD_ID:			
			// TODO: envoyer la requete(message) 'vlan' au commutateur
		case DISCONNECT_PORT_CMD_ID:		
			// TODO: envoyer la requete(message) 'deconnecter' au commutateur	
		case SHOW_PORT_STAT_CMD_ID:			
			// TODO: envoyer la requete(message) 'stats' au commutateur
		case TRIGGER_EVENTS_CMD_ID:			
			// TODO: envoyer la requete(message) 'scruter' au commutateur
			break;
		case SNIFFER_PORT_CMD_ID:
			// TODO: envoyer la requete(message) 'sniffer' au commutateur
			break;
		case STOP_CMD_ID:
			// TODO: envoyer la requete(message) 'stopper' au commutateur
			break;
		default :
			break; 
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




// create_admin: crée et initialise un admin
Admin* create_admin( int AdminRequestKey) {
	Admin* admin = (Admin*)malloc(sizeof(Admin));
	if (admin == NULL) {
		err_log(("create_admin.malloc"))
		exit(EXIT_FAILURE);
	}
	admin->IPCkey = AdminRequestKey;
	return admin;	
}

// create_admin_list: create a list of admins
AdminList* create_admin_list() {
	AdminList* AL = (AdminList*)malloc(sizeof(AdminList));
	if (AL == NULL) {
		err_log(("create_admin_list.malloc"))
		exit(EXIT_FAILURE);
	}
	return AL;
}