// Inclusions
// ----------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "gestionConnexions.h"
#include "commutateur.h"
#include "logger.h"
#include "libnet.h"
#include "libthrd.h"
#include "libipc.h"
#include "constants.h"
#include "commandesAdmin.h"

// Constants
// ----------


// Structures
// ----------


// Variables
// ---------


// Macros
// ------



// Fonctions 
// ----------

// lister: liste les ports d'un commutateur
void lister(int clientResponseID) {
	Port* ports = commutateur.ports;
	char* response = (char*) malloc(sizeof(char)* MAX_BUFFER_SIZE);
	char str[MAX_BUFFER_SIZE];
	Port p;
	int i;

	log("# Building response ...", stderr)
#ifdef DEBUG
	printf("# Processing response (responseID: %d) ...\n", clientResponseID);
#endif

	if ( response == NULL) {
		err_log("get_port_infos.malloc", stderr)
		exit(EXIT_FAILURE);
	}
	strcpy(response, "");
	
	for (i=0; i<NBR_MAX_PORT; i++)  {
		p = ports[i];
		sprintf(str, "PORT [%d]: ", p.num);
		response = strcat(response, str);
		if (p.state == PORT_STATE_CONNECTED ) {
			response = strcat(response, " connected\n");
		}
		else {
			response = strcat(response, " disconnected\n");
		}
	}
	send_response_to_client(clientResponseID, response);
	log("# Response sent.", stderr)
	sep()
}


// afficher: affihce les infos sur un port 
void afficher(int clientResponseID, char * params) {
	char* response = (char*) malloc(sizeof(char)* MAX_BUFFER_SIZE);
	char str[MAX_BUFFER_SIZE];
	Port p;
	int num_port;

	if (sscanf(params, "%d", &num_port) != 1) {
		log(("[Error] Parsing port number from params failed."), stderr)
		log("# Sending error to client ...", stderr)
		strcpy(response,"[Error] Parsing port number from params failed.");
		send_response_to_client(clientResponseID, response);	
		log("# Response sent.", stderr)
		sep()
	}

	if (!is_valid_port(num_port)) { // si le port n'est pas valide
#ifdef DEBUG
		fprintf(stderr,"# [Error] Port [%d] doesn't exist.\n", num_port);
#endif
		log("# Sending error to client ...", stderr)
		sprintf(response,"[Error] Port [%d] doesn't exist.\n", num_port);
		send_response_to_client(clientResponseID, response);	
		log("# Response sent.", stderr)
		sep()
	}
	else {

		log("# Building response ...", stderr)
	#ifdef DEBUG
		printf("# Processing response (responseID: %d) ...\n", clientResponseID);
	#endif

		if ( response == NULL) {
			err_log("get_port_infos.malloc", stderr)
			exit(EXIT_FAILURE);
		}
		p = get_port_by_number(num_port);

		sprintf(str, "PORT [%d]:\n", p.num);	
		response = strcat(response, str);
		build_port_state_string(p, response)
		build_port_type_string(p, response)
		sprintf(str, "+ vlan     = %d\n", p.vlan ); 
		response = strcat(response, str);
		sprintf(str, "+ rcv_size = %d\n", p.rcv_size);
		response = strcat(response, str);
		sprintf(str, "+ snd_size = %d\n", p.snd_size);
		response = strcat(response, str);
		send_response_to_client(clientResponseID, response);
		log("# Response sent.", stderr)
		sep()
	}
}


// adresses: affihce les adresses ethernet d'un port 
void adresses(int clientResponseID, char * params) {
	char* response = (char*) malloc(sizeof(char)* MAX_BUFFER_SIZE);
	char str[MAX_BUFFER_SIZE];
	Port p;
	int i,num_port;
	EthernetAddress* addresses;

	if (sscanf(params, "%d", &num_port) != 1) {
		log(("[Error] Parsing port number from params failed."), stderr)
		log("# Sending error to client ...", stderr)
		strcpy(response,"[Error] Parsing port number from params failed.");
		send_response_to_client(clientResponseID, response);	
		log("# Response sent.", stderr)
		sep()
	}

	if (!is_valid_port(num_port)) { // si le port n'est pas valide
#ifdef DEBUG
		fprintf(stderr,"# [Error] Port [%d] doesn't exist.\n", num_port);
#endif
		log("# Sending error to client ...", stderr)
		sprintf(response,"[Error] Port [%d] doesn't exist.\n", num_port);
		send_response_to_client(clientResponseID, response);	
		log("# Response sent.", stderr)
		sep()
	}
	else {

		log("# Building response ...", stderr)
	#ifdef DEBUG
		printf("# Processing response (responseID: %d) ...\n", clientResponseID);
	#endif

		if ( response == NULL) {
			err_log("get_port_infos.malloc", stderr)
			exit(EXIT_FAILURE);
		}
		p = get_port_by_number(num_port);

		// Get port addresses
		addresses = p.addresses;

		for ( i=0; i<NBR_MAX_ETHERNET_ADDR; i++) {
			sprintf(str, "addresse [%d]:\n", i+1);
			response = strcat(response, str);
			strcpy(str, ethernet_address_to_string(addresses[i]));
			response = strcat(response, str);
			response = strcat(response, "\n");
		}
		
		send_response_to_client(clientResponseID, response);
		log("# Response sent.", stderr)
		sep()
	}
}


// stats : affcihe les stats sur un port 
void stats(int clientResponseID, char * params) {
	char* response = (char*) malloc(sizeof(char)* MAX_BUFFER_SIZE);
	char str[MAX_BUFFER_SIZE];
	Port p;
	int i,num_port;

	if (sscanf(params, "%d", &num_port) != 1) {
		log(("[Error] Parsing port number from params failed."), stderr)
		log("# Sending error to client ...", stderr)
		strcpy(response,"[Error] Parsing port number from params failed.");
		send_response_to_client(clientResponseID, response);	
		log("# Response sent.", stderr)
		sep()
	}

	if (!is_valid_port(num_port)) { // si le port n'est pas valide
#ifdef DEBUG
		fprintf(stderr,"# [Error] Port [%d] doesn't exist.\n", num_port);
#endif
		log("# Sending error to client ...", stderr)
		sprintf(response,"[Error] Port [%d] doesn't exist.\n", num_port);
		send_response_to_client(clientResponseID, response);	
		log("# Response sent.", stderr)
		sep()
	}
	else {

		log("# Building response ...", stderr)
	#ifdef DEBUG
		printf("# Processing response (responseID: %d) ...\n", clientResponseID);
	#endif

		if ( response == NULL) {
			err_log("get_port_infos.malloc", stderr)
			exit(EXIT_FAILURE);
		}
		p = get_port_by_number(num_port);

		for ( i=0; i<NBR_MAX_ETHERNET_ADDR; i++) {
			sprintf(str, "Port [%d]:\t", p.num);
			response = strcat(response, str);
			sprintf(str, "[ rcv = %d, snd = %d ]\n", p.rcv_size, p.snd_size);
			response = strcat(response, str);
		}
		
		send_response_to_client(clientResponseID, response);
		log("# Response sent.", stderr)
		sep()
	}
}
/*
// TCP_connect: connecter un lport à un port de comutateur distant
void Admin_TCP_connect( char* params, int AdminResponseQueueID) {
	// recuperer les parametres utile pour la requête
	char help_message[MAX_BUFFER_SIZE];
	int lport, TCP_port, dport;
	char machine[REQUEST_SIZE];
	char message[REQUEST_SIZE]; // port envoyer lport et dport

	Port* Lport, *Dport;
	int connected_socket; // Descripteur de la SOCKET renvoyer par le serveur
	FILE* connected_socket_file;

	//
	if (sscanf(params, "%d:[%d@]%s:%d", &lport, &TCP_port, machine, &dport) == 1) {
		err_log(("TCP_connect.sscanf"))
		build_help_string(help_message)
		// TODO: finir de repondre au client du message SYNTAX
		IPC_send_message( AdminResponseQueueID, 1, SYNTAX_MSG ) ;
		// envoie d'un message contenat l'aide
		IPC_send_message( AdminResponseQueueID, 1, help_message ) ;
		exit(EXIT_FAILURE);
	}

	// Verification ds paramètres et envoie de message si erreur de type
	// ou non validité
	if (lport < 0 || dport < 0 || lport > NBR_MAX_PORT || dport > NBR_MAX_PORT ) {
		log("Envoie du message BOUNDS à l'admin client")
		IPC_send_message( AdminResponseQueueID, 1, BOUNDS_MSG) ;
	}
	else {
		// recuperer le port local du commutateur
		Lport = get_port_from_commutator(lport);
		Dport = get_port_from_commutator(dport);

		// ----------- start connection -------------
		log("initialisation de la connexion TCP")

		// Connection au serveur
		connected_socket=connexionServeur(machine, TCP_port);
		if(connected_socket<0){
			err_log("Main.connexionServeur")
			exit(EXIT_FAILURE);
		}

		connected_socket_file = fdopen(connected_socket, "w+");
		if (connected_socket_file == NULL) {
			err_log(("Main.fdopen"))
			exit(EXIT_FAILURE);
		}

		sprintf(message, "lport %d\n", lport); // on prepare le lport
		// send lport et dport to the
		fprintf(connected_socket_file, message);
		fflush(connected_socket_file); // clean buffer

		if (fgets(message, REQUEST_SIZE , connected_socket_file )) {
			// TODO: filtrer pour recupere le port
			if (!strcmp(message, OK_MSG)) {
				sprintf(message, "dport %d\n", dport); // on prepare le dport
				// send lport et dport to the
				fprintf(connected_socket_file, message);

				// reception du OK du dport
				if (fgets(message, REQUEST_SIZE, connected_socket_file )) {
					if (!strcmp(message, OK_MSG)) {

						// TODO: poser un verrou avant la modification des ports
						P(lport);
						P(dport);
						// mise a jour des deux ports
						Lport->state = PORT_STATE_CONNECTED;
						Lport->state = PORT_STATE_CONNECTED;

						// Assignation du descripteur de la socket
						Lport->socket_fd = connected_socket;
						Dport->socket_fd = connected_socket;
						V(lport);
						V(dport);
						// TODO: liberer le verrou apres la modification des ports
					}
				}
				else {
					log("pas de reception du OK (dport")
				}
			}
		}
		else {
			log("pas de reception du OK (lport")
		}
	}

}

void connect_TAP ( int port, int mqueueID) {
	char nom_intf[REQUEST_SIZE];
	int intf_fd;
	// recuperation du port
	Port* p = get_port_from_commutator(port);

	// Verifier l'état du port
	if ( p->state == PORT_STATE_CONNECTED) {
		err_log("connect_TAP: port deja connecte")
		IPC_send_message( mqueueID, 1, "connect_TAP: port deja connecte" );
	}
	else { // port libre
		// TODO: poser le verrou sur le port
		P(port);
		p->state = PORT_STATE_CONNECTED;
		intf_fd = creationInterfaceVirtuelle(nom_intf);
		V(port);
		// TODO: mettre le nom de l'intf dans la structure (à voir)
		// TODO: Envoie le nom au client par msg
	}

}


void vlan (int port, int vlan) {
	// recuperation du port
	// verrou
	P(port);
	Port* p = get_port_from_commutator(port);
	p->vlan = vlan;
	V(port);
	// leve le verrou
}

*/