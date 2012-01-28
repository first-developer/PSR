
#ifndef COMMUTATEUR_H
#define COMMUTATEUR_H 

#include "../Network/libnet.h"
#include "constants.h"
// Constants
// ----------

#define COMMUTATOR_VERROU			0

#define	NO_PORT						0
#define SERVER_LOCAL_PORT 			5000
#define SERVER_LOCAL_IP 			"127.0.0.1"



#define NBR_MAX_PORT 				5
#define NBR_MAX_CLIENT 				5
#define NBR_MAX_ETHERNET_ADDR 		5

// Type de port
#define INTF_ETHERNET_PORT_TYPE 	0
#define TCP_CONNECTION_PORT_TYPE 	1
#define NO_PORT_TYPE 				-1

// Etat de la connexion
#define PORT_STATE_CONNECTED 		1
#define PORT_STATE_DISCONNECTED 	0
#define CONNECTED_STATE 			"connecte"
#define DISCONNECTED_STATE 			"deconnecte"



// Variables
// ----------

// Macro 
// ------

#ifdef DEBUG
	#define build_port_state_string(p,buffer)		if (p.state == PORT_STATE_CONNECTED ) {\
			buffer = strcat(buffer, "+ etat     = connected\n");\
		}\
		else {\
			buffer = strcat(buffer, "+ etat     = disconnected\n");\
		}
	#define build_port_type_string(p,buffer)		if (p.type == INTF_ETHERNET_PORT_TYPE ) {\
			buffer = strcat(buffer, "+ type     = Ethernet Interface\n");\
		}\
		else if (p.type == TCP_CONNECTION_PORT_TYPE ){\
			buffer = strcat(buffer, "+ type     = TCP connection\n");\
		}\
		else {\
			buffer = strcat(buffer, "+ type     = no type\n");\
		}

#else
	build_port_state_string(p,buffer)
	build_port_type_string(p,buffer)
#endif




// Structures
// ----------

// Port
typedef struct {
short int num;						// numero de port
int state; 							// Etat du port
int type;  							// Type du port
EthernetAddress addresses[NBR_MAX_ETHERNET_ADDR];
int nb_addresses;					// indice de la dernière adresse ethernet entrée
int vlan; 							// Numéro de VLAN
int fd; 							// descripteur d' Interface Ethernet Virtuelle 
int sd; 							// descripteur de socket
int rcv_size; 						// Taille des données reçues
int snd_size; 						// Taille des données envoyées
} Port;

// CLient
typedef struct Client{
	int num;
	int responseID;
} Client; 

// Commutator
typedef struct Commutator{
	Client clients[NBR_MAX_CLIENT]; 	// liste des clients en attente d'infos
	Port ports[NBR_MAX_PORT]; 		// L'ensemble des ports du commutateur
} Commutator;


// variables publiques
// -------------------
extern Commutator commutateur;


// Prototypes de fonctions
// -----------------------

unsigned short int get_port_from_options(int argc,char **argv);
void init_commutator_and_listen_to_connections(int argc, char**argv);
Port* init_port( int num);
void init_all_ports_of_commutator ();
Port get_port_by_number(int numPort);
Client* create_client();
int is_valid_port(int);
void add_ethernet_address_to_port(int num_port, EthernetAddress* eth_addr);
int is_full_of_addresses(Port p) ;


#endif