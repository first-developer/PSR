/** fichier commutateur.h **/

/***************************************************/
/* Structures necessaires a la gestion du          */
/* commutateur                                     */
/***************************************************/

#ifndef COMMUTATEUR_HEADER
#define COMMUTATEUR_HEADER

// Inclusions
// -----------


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
#define CONNECTED_STATE				"connecte"
#define DISCONNECTED_STATE			"deconnecte"


// Macro 
// ------

#ifdef DEBUG
	#define display_port_infos_state(p, output)	if (p->etat == PORT_STATE_CONNECTED ) {\
													fprintf(output, "%s - etat     = %s %s", MAGENTA, CONNECTED_STATE, BBLACK);\
												}\
												else {\
													fprintf(output, "%s - etat     = %s %s", MAGENTA, DISCONNECTED_STATE, BBLACK);\
												}
	#define display_port_infos_type(p,output)	if (p->type == INTF_ETHERNET_PORT_TYPE ) {\
													fprintf(output, "%s - type     = %s %s", MAGENTA,"interface Ethernet", BBLACK);\
												}\
												else {\
													fprintf(output, "%s - type     = %s %s", MAGENTA, "Connexion TCP", BBLACK);\
												}
	#define display_port_infos(p,output)	fprintf(output, "%s - num      = %d %s", MAGENTA, p->num, BBLACK);\
											display_port_infos_state(p, output)\
											display_port_infos_type(p, output)\
											fprintf(output, "%s - VLAN     = %d %s", MAGENTA, p->vlan,     BBLACK);\
											fprintf(output, "%s - rcv_size = %d %s", MAGENTA, p->rcv_size, BBLACK);\
											fprintf(output, "%s - snd_size = %d %s", MAGENTA, p->snd_size, BBLACK);\
											end_log()
	
#else
	display_port_infos_state(p, output)
	display_port_infos_type(p,output)
	display_port_infos(p,output)
#endif





// Structures 
// ----------

// Adresse ethernet
typedef struct {
  unsigned char bytes[ETHERNET_ADDRESS_SIZE];
} EthernetAddress;

typedef struct {
  EthernetAddress list[NBR_MAX_ETHERNET_ADDR];
} EthernetAddresses;

// descripteurs de connexion
typedef struct {
	int fd;
	int sock;
} Connection_descriptors;

typedef struct {
	short int num;
	int etat;								// Etat du port
	int type;								// Type du port
	EthernetAddresses eth_addresses;
	int vlan;								// Numéro de VLAN
	Connection_descriptors dc; 				// descripteurs de connexion
	int rcv_size; 							// Taille des données reçues 
	int snd_size;							// Taille des données envoyées 
} Port;

typedef struct {
	Port list[NBR_MAX_PORT];
} PortList;


typedef struct Commutator{
	AdminList admins;  	// liste des clients en attente d'infos
	PortList ports; 	// L'ensemble des ports du commutateur	
} Commutator;


// variables publiques  
// -------------------

static Commutator mainCommutator;

// Prototypes de fonctions 
// -----------------------

// Ports
Port* 	create_port( int num, int type, int nVlan);
PortList* create_port_list();

// Commutateur
Commutator* create_commutator();
void add_port_to_commutator(Port* p, Commutator *c);
void add_admins_to_commutator(Admin* a, Commutator *c);
void *mortdefils (int sig);
void cancel(char*);
void init_and_loop_server(int , char**);
unsigned short int get_port_from_options(int ,char **);
void set_port_vlan(Port*p , int nVLAN);


#endif