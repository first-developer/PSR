/** fichier commutateur.h **/

/***************************************************/
/* Structures necessaires a la gestion du          */
/* commutateur                                     */
/***************************************************/

#ifndef COMMUTATEUR_HEADER
#define COMMUTATEUR_HEADER

// Inclusions
// -----------
//	static int CommutatorRequestQueueID;	// id file de la requête 


// Constantes 
// ----------

#define ETHERNET_ADDRESS_SIZE		6
#define NBR_MAX_PORT				5
#define NBR_MAX_ETHERNET_ADDR		5
// Type de port
#define INTF_ETHERNET_PORT_TYPE		0
#define TCP_CONNECTION_PORT_TYPE	1
#define NO_PORT_TYPE 				-1

// Etat de la connexion
#define PORT_STATE_CONNECTED		1
#define PORT_STATE_DISCONNECTED		0
#define CONNECTED_STATE				"connecte"
#define DISCONNECTED_STATE			"deconnecte"


// Macro 
// ------

#ifdef DEBUG
	#define display_port_infos_state(p, output)	if (p->state == PORT_STATE_CONNECTED ) {\
													fprintf(output, "%s - etat     = %s %s", MAGENTA, BBLACK,CONNECTED_STATE);\
												}\
												else {\
													fprintf(output, "%s - etat     = %s %s", MAGENTA, BBLACK, DISCONNECTED_STATE);\
												}
	#define display_port_infos_type(p,output)	if (p->type == INTF_ETHERNET_PORT_TYPE ) {\
													fprintf(output, "%s - type     = %s %s", MAGENTA, BBLACK, "Pnterface Ethernet");\
												}\
												else if (p->type == TCP_CONNECTION_PORT_TYPE ){\
													fprintf(output, "%s - type     = %s %s", MAGENTA, BBLACK, "Connexion TCP");\
												}\
												else {\
													fprintf(output, "%s - type     = %s %s", MAGENTA, BBLACK, "Pas de type");\
												}
	#define display_port_infos(p,output)	fprintf(output, "\n%sPORT [%d]: %s", BBLACK, p->num, BBLACK);print_newline()\
											fprintf(output, "%s - num      = %s%d", MAGENTA, BBLACK, p->num);print_newline()\
											display_port_infos_state(p, output) print_newline()\
											display_port_infos_type(p, output) print_newline() \
											fprintf(output, "%s - VLAN     = %s %d", MAGENTA, BBLACK, p->vlan   ); print_newline()\
											fprintf(output, "%s - rcv_size = %s %d", MAGENTA, BBLACK, p->rcv_size); print_newline()\
											fprintf(output, "%s - snd_size = %s %d", MAGENTA, BBLACK, p->snd_size); print_newline()\
											end_log()
	
#else
	#define display_port_infos_state(p, output)
	#define display_port_infos_type(p,output)
	#define display_port_infos(p,output)
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


typedef struct {
	short int num;
	int state;								// Etat du port
	int type;								// Type du port
	EthernetAddresses eth_addresses;
	int vlan;								// Numéro de VLAN
	int IEV_fd;								// descripteur  d' Interface Ethernet Virtuelle (IEV)
	int socket_fd;							// descripteur de socket
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
	extern Commutator commutateur;  // the main commutator

// Prototypes de fonctions 
// -----------------------

// Ports
Port* 	create_and_init_port( int num);
PortList* create_and_init_port_list();
void init_commutator_ports ();
PortList* create_port_list();
void display_all_commutator_ports();
Port* get_port_from_commutator(int numPort);

// Commutateur
void create_and_init_commutator();
void add_port_to_commutator(Port* p, Commutator *c);
void add_admins_to_commutator(Admin* a, Commutator *c);
void *mortdefils (int sig);
void cancel(char*);
void init_commutator_and_listen_to_connections(int , char**);
unsigned short int get_port_from_options(int ,char **);
void set_port_vlan(Port*p , int nVLAN);


#endif
