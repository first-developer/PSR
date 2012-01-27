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



// Varaibles 
// ---------

Request adminRequest;
char cmd_name[MIN_BUFFER_SIZE];
char requestParams[BUFFER_SIZE];
char* responseBody;
int serverResquestID;

// Functions 
// ----------


void request_admin_handler(int arg) {

	int cmd_id;
	// allocate memory for reponse body
	responseBody = (char *) malloc(sizeof(char)*MAX_BUFFER_SIZE);
	
	// Creation de l'id de la file de requete
	serverResquestID = create_server_request_id();	
	
	// handle client requests 
	for (;;) {
		// switch through arg 
		log("# Waiting for clients' requests ...", stderr)
		adminRequest = get_request_from_client(serverResquestID);
		
		// get ID of the command sent by the client 
		cmd_id = get_command_id_from_command_name(adminRequest.name);

		switch(cmd_id) {
			case LIST_PORT_CMD_ID:
				log("# Building response ...", stderr)
		#ifdef DEBUG
				printf("# Processing response (responseID: %d) ...\n", adminRequest.clientResponseID);
		#endif
				lister(responseBody);
				send_response_to_client(adminRequest.clientResponseID, responseBody);
				log("# Response sent.", stderr)
				free(responseBody);
				break;
			default:
				break;
		}
	}
}


