/** fichier commutateur.h **/

/***************************************************/
/* Structures necessaires a la gestion du          */
/* commutateur                                     */
/***************************************************/

// Inclusions
// -----------

#include "gestionAdmin.h"

// Constantes 
// ----------

#define ETHERNET_ADDRESS_SIZE		6
#define NBR_MAX_PORT				5
#define NBR_MAX_ETHERNET_ADDR		5
// Type de port
#define INTF_ETHERNET_PORT_TYPE		0
#define TCP_CONNECTION_PORT_TYPE	1

// Etat de la connexion
#define PORT_STATE_CONNECTED		1
#define PORT_STATE_DISCONNECTED		0


// Structures 
// ----------

// Adresse ethernet
typedef struct {
  unsigned char bytes[ETHERNET_ADDRESS_SIZE];
} EthernetAddress;

// descripteurs de connexion
typedef struct {
	int fd;
	int sock;
} Connection_descriptors;

typedef struct {
	short int num;
	int etat;								// Etat du port
	int type;								// Type du port
	EthernetAddress ethernet_addr_list[NBR_MAX_ETHERNET_ADDR];
	int nVlan;								// Numéro de VLAN
	Connection_descriptors dc; 				// descripteurs de connexion
	int rcv_size; 							// Taille des données reçues 
	int snd_size;							// Taille des données envoyées 
} Port;

typedef struct {
	Port list[NBR_MAX_PORT];
} PortsList;


typedef struct {
	AdminClientList admin_clients;  	// liste des clients en attente d'infos
	PortsList ports; 						// L'ensemble des ports du commutateur	
} Commutator;


// variables publiques  
// -------------------

// Prototypes de fonctions 
// -----------------------
Commutator* create_and_init_commutator(AdminClientList , PortsList );
Port* 	create_and_init_port( int num, int type, int nVlan);
void *mortdefils (int sig);
void cancel(char*);
void init_and_loop_server(int , char**);
unsigned short int get_port_from_options(int ,char **);



