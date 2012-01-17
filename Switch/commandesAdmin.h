/** fichier commandesAdmin.h **/

/**********************************************************************/
/* Ce fichier contient les déclarations publiques de commandesAdmin.c */
/**********************************************************************/


#ifndef CMDADMIN_HEADER
#define CMDADMIN_HEADER

// Inclusions
// ----------

// Constants
// ----------

#define MAX_BUFFER_SIZE				1025

#define PORT_OPTION 				"port"
#define HELP_CMD 					"help"
#define LIST_PORT_CMD				"lister"
#define SHOW_PORT_INFOS_CMD			"afficher"
#define LIST_PORT_ADDRESSES_CMD		"addresses"
#define TAP_CONNECTION_CMD			"connecter_tap"
#define TCP_CONNECTION_CMD			"connecter_tcp"
#define SET_PORT_VLAN_CMD			"vlan"
#define DISCONNECT_PORT_CMD			"deconnecter"
#define SHOW_PORT_STAT_CMD			"stats"
#define TRIGGER_EVENTS_CMD			"scruter"  // events : connexion et deconnexion d'un port
#define SNIFFER_PORT_CMD			"sniffer"
#define STOP_CMD					"stoper"
#define NO_CMD 						"no_command"

#define NO_CMD_ID 						-1
#define HELP_CMD_ID						99
#define LIST_PORT_CMD_ID				100
#define SHOW_PORT_INFOS_CMD_ID			101 
#define LIST_PORT_ADDRESSES_CMD_ID		102 
#define TAP_CONNECTION_CMD_ID			103 
#define TCP_CONNECTION_CMD_ID			104
#define SET_PORT_VLAN_CMD_ID			105	
#define DISCONNECT_PORT_CMD_ID			106
#define SHOW_PORT_STAT_CMD_ID			107	
#define TRIGGER_EVENTS_CMD_ID			108
#define SNIFFER_PORT_CMD_ID				109
#define STOP_CMD_ID						110

#define REQUEST_SIZE					256

#define	SYNTAX_MSG				"syntax\n"
#define	OK_MSG					"ok\n"
#define	BOUNDS_MSG				"bounds\n"


// Structures
// ----------


// Macros
// ------

// build the help message before send it to the client 
#define build_help_string(buffer)			sprintf(buffer, \
			"%s[-l|--lister] \t\t\t%s# liste les ports d'un commutateur\n",\
				BCYAN, END_LOG_COLOR);\
			sprintf(buffer, \
				"%s[-a|--afficher <port>] \t\t%s# affiche les details d'un port\n",	BCYAN, END_LOG_COLOR); \
			sprintf(buffer, \
				"%s[-A|--addresses <port>] \t%s# liste les addr. ethernet d'un port avec l'age\n",\
				BCYAN, END_LOG_COLOR);\
			sprintf(buffer, \
				"%s[-c|--connecter_tap <port>] %s\t# connecter un port a une interface virtuelle\n", \
				BCYAN, END_LOG_COLOR);\
			sprintf(buffer, \
				"%s[-C|--connecter_tcp <lport>:[<port TCP>@]<machine>:<dport>] "\
				"%s# connecter un port a une interface virtuelle\n", \
				BCYAN, END_LOG_COLOR); \
			sprintf(buffer, \
				"%s[-v|--vlan <port>:<vlan>] %s\t# affecter un numero de port a un port local\n", \
				BCYAN, END_LOG_COLOR);\
			sprintf(buffer, \
				"%s[-d|--deconnecter <port>] %s\t# deconnecter un port \n", BCYAN, END_LOG_COLOR);\
			sprintf(buffer, \
				"%s[-s|--stats <port>] %s\t\t# affiche les stats d'un port ( octets recus et envoyes)\n",\
				BCYAN, END_LOG_COLOR); \
			sprintf(buffer, \
				"%s[--scruter] %s\t\t\t# scruter les evenements relatifs aux connexions et deconnexions "\
				"de ports\n", BCYAN, END_LOG_COLOR); \
			sprintf(buffer, \
				"%s[--sniffer <port>] %s\t\t# affiche les paquets arrivants ou partant d'un port\n", \
				BCYAN, END_LOG_COLOR);\
			sprintf(buffer, \
				"%s[--stopper] %s\t\t\t# arreter le commutateur virtuel\n", BCYAN, END_LOG_COLOR);


#ifdef DEBUG
	#define help_lister_cmd(output)			fprintf(output, \
												"%s[-l|--lister] \t\t\t%s# liste les ports d'un commutateur\n",\
												BCYAN, END_LOG_COLOR);
	#define help_afficher_cmd(output)		fprintf(output, \
												"%s[-a|--afficher <port>] \t\t%s# affiche les details d'un port\n", \
												BCYAN, END_LOG_COLOR);
	#define help_addresses_cmd(output)		fprintf(output, \
												"%s[-A|--addresses <port>] \t%s# liste les addr. ethernet d'un port avec l'age\n",\
												BCYAN, END_LOG_COLOR);
	#define help_connecter_tap_cmd(output)	fprintf(output, \
												"%s[-c|--connecter_tap <port>] %s\t# connecter un port a une interface virtuelle\n", \
												BCYAN, END_LOG_COLOR);
	#define help_connecter_tcp_cmd(output)	fprintf(output, \
												"%s[-C|--connecter_tcp <lport>:[<port TCP>@]<machine>:<dport>] "\
												"%s# connecter un port a une interface virtuelle\n", \
												BCYAN, END_LOG_COLOR);
	#define help_vlan_cmd(output)			fprintf(output, \
												"%s[-v|--vlan <port>:<vlan>] %s\t# affecter un numero de port a un port local\n", \
												BCYAN, END_LOG_COLOR);
	#define help_deconnecter_cmd(output)	fprintf(output, \
												"%s[-d|--deconnecter <port>] %s\t# deconnecter un port \n", BCYAN, END_LOG_COLOR);
	#define help_stats_cmd(output)			fprintf(output, \
												"%s[-s|--stats <port>] %s\t\t# affiche les stats d'un port ( octets recus et envoyes)\n",\
													BCYAN, END_LOG_COLOR);
	#define help_scruter_cmd(output)		fprintf(output, \
												"%s[--scruter] %s\t\t\t# scruter les evenements relatifs aux connexions et deconnexions "\
												"de ports\n", BCYAN, END_LOG_COLOR);
	#define help_sniffer_cmd(output)		fprintf(output, \
												"%s[--sniffer <port>] %s\t\t# affiche les paquets arrivants ou partant d'un port\n", \
												BCYAN, END_LOG_COLOR);
	#define help_stopper_cmd(output)		fprintf(output, \
												"%s[--stopper] %s\t\t\t# arreter le commutateur virtuel\n", BCYAN, END_LOG_COLOR);
	#define help_command_list(output) \
	    help("Liste des commandes")\
		help_lister_cmd(output) \
		help_afficher_cmd(output) \
		help_addresses_cmd(output) \
		help_connecter_tap_cmd(output) \
		help_connecter_tcp_cmd(output) \
		help_vlan_cmd(output) \
		help_deconnecter_cmd(output) \
		help_stats_cmd(output) \
		help_scruter_cmd(output) \
		help_sniffer_cmd(output) \
		help_stopper_cmd(output)
#else
	#define help_lister_cmd(output)										
	#define help_afficher_cmd(output)	
	#define help_addresses_cmd(output)	
	#define help_connecter_tap_cmd(output)	
	#define help_vlan_cmd(output)	
	#define help_deconnecter_cmd(output)	
	#define help_stats_cmd(output)	
	#define help_scruter_cmd(output)	
	#define help_sniffer_cmd(output)	
	#define help_stopper_cmd(output)	
#endif

// Prototypes
// -----------

void display_command_list( FILE* output);
void list_port_addresses( char* Args);
void show_port_infos( char* Args);
void list_port_on_commutator_with_status();
void connect_TAP ( int port, int mqueueID);
void vlan (int port, int vlan);
#endif
