/** fichier commandesAdmin.h **/

/**********************************************************************/
/* Ce fichier contient les déclarations publiques de commandesAdmin.c */
/**********************************************************************/


#ifndef CMDADMIN_HEADER
#define CMDADMIN_HEADER

// Inclusions
// ----------


#include "constants.h"
// Constants
// ----------

#define PORT_OPTION 			"port"
#define HELP_CMD 				"help"
#define LIST_PORT_CMD 			"lister"
#define SHOW_PORT_INFOS_CMD 	"afficher"
#define LIST_PORT_ADDRESSES_CMD "addresses"
#define TAP_CONNECTION_CMD 		"connecter_tap"
#define TCP_CONNECTION_CMD 		"connecter_tcp"
#define SET_PORT_VLAN_CMD 		"vlan"
#define DISCONNECT_PORT_CMD 	"deconnecter"
#define SHOW_PORT_STAT_CMD 		"stats"
#define TRIGGER_EVENTS_CMD 		"scruter" // events : connexion et deconnexion d'un port
#define SNIFFER_PORT_CMD 		"sniffer"
#define STOP_CMD 				"stoper"
#define NO_CMD 					"no_command"

#define NO_CMD_ID 					-1
#define HELP_CMD_ID 				99
#define LIST_PORT_CMD_ID 			100
#define SHOW_PORT_INFOS_CMD_ID 		101
#define LIST_PORT_ADDRESSES_CMD_ID 	102
#define TAP_CONNECTION_CMD_ID 		103
#define TCP_CONNECTION_CMD_ID 		104
#define SET_PORT_VLAN_CMD_ID 		105
#define DISCONNECT_PORT_CMD_ID 		106
#define SHOW_PORT_STAT_CMD_ID 		107
#define TRIGGER_EVENTS_CMD_ID 		108
#define SNIFFER_PORT_CMD_ID 		109
#define STOP_CMD_ID 				110


// Structures
// ----------


// Variables
// ----------
char buffer[MAX_BUFFER_SIZE];

// Macros
// ------

#define clear() system("clear");

// build the help message before send it to the client
#define build_help_string(str) sprintf(str,"Usage:\n  [-l|--lister] \t\t# liste les ports d'un commutateur\n");\
	strcat(str,"  [-a|--afficher <port>] \t# affiche les details d'un port\n"); \
	strcat(str, "  [-A|--addresses <port>] \t# liste les addr. ethernet d'un port avec l'age\n");\
	strcat(str, "  [-c|--connecter_tap <port>] \t# connecter un port a une interface virtuelle\n");\
	strcat(str, "  [-C|--connecter_tcp <lport>:[<port TCP>@]<machine>:<dport>] # connecter un port a une interface virtuelle\n"); \
	strcat(str, "  [-v|--vlan <port>:<vlan>] \t# affecter un numero de port a un port local\n");\
	strcat(str, "  [-d|--deconnecter <port>] \t# deconnecter un port \n");\
	strcat(str, "  [-s|--stats <port>] \t\t# affiche les stats d'un port ( octets recus et envoyes)\n"); \
	strcat(str, "  [--scruter] \t\t\t# scruter les evenements relatifs aux connexions et deconnexions de ports\n"); \
	strcat(str, "  [--sniffer <port>] \t\t# affiche les paquets arrivants ou partant d'un port\n");\
	strcat(str, "  [--stopper] \t\t\t# arreter le commutateur virtuel\n");\
	strcpy(buffer, str);

#ifdef DEBUG
	#define help_command_list(output) build_help_string(buffer);fprintf(output, "%s", buffer);

	#define show_cmd_details(cmd_name, optarg)	printf("Request :\n|-- command: %s\n", cmd_name);\
		printf("|-- params: %s\n", optarg);

	#define show_response_details(response) printf("Response :\n");\
		printf("|-- body:\n%s", response);
#else
	help_command_list(output)
	show_response_details(response)
	show_cmd_details(args)
#endif

// Prototypes
// -----------

void lister(int );
void afficher(int clientResponseID, char * params);
void stats(int clientResponseID, char * params);
void connect_TAP ( int port, int mqueueID);
void vlan (int port, int vlan);
#endif