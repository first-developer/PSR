/*  PSR/IPC/libipc.c  */

// Inclusions
// -----------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#include "libipc.h"
#include "constants.h"
#include "logger.h"

// Constants
// ----------



// Fonctions 
// ----------

// create_client_response_id: pour créer une file de message privée (client)
int create_client_response_id() {
	int respondID; // identifiant de la file de message 

	// On crée l'id de la file de message si elle n'existe pas; Dans le cas 
	// contraire on genere une erreur grace au flag 'IPC_EXCL'
	if ( (respondID = msgget(IPC_PRIVATE, IPC_EXCL | IPC_CREAT | 0666)) == -1 ){
		err_log("create_responder_IPC_message_queue.msgget", stderr)
		return -1;
	}
	return respondID;
}


// create_server_request_id: pour créer une file de message public(commutateur)
//										accessible par la clé public 'RESPONDER_IPC_KEY'
int create_server_request_id() {
	int requestID; // identifiant de la file de message 
	// On crée l'id de la file de reponse par une clé public
	if ( (requestID = msgget(SERVER_PUBLIC_KEY, IPC_EXCL | IPC_CREAT | 0666)) == -1 ){
		err_log("create_requester_IPC_message_queue.msgget", stderr)
		return -1;
	}
	return requestID;
}


// get_server_request_id: recupere l'id de la file de message
int get_server_request_id() {
	int requestID; // identifiant de la file de message 
	// recuperaton de la l'id de la file de requête
	if ( (requestID = msgget(SERVER_PUBLIC_KEY, 0666)) == -1 ){
		err_log("get_responder_IPC_message_queue.msgget", stderr)
		return -1;
	}
	return requestID;
}
	

// destroy_IPC_message_queue: pour spprime une file de message
void destroy_response_or_request_id(int id) {
	int status;  

	// On tente de détruire la file de message 
	status = msgctl(id, IPC_RMID, NULL);

	if ( status == -1 ){
		err_log("destroy_IPC_message_queue.msgctl", stderr)
		exit(EXIT_FAILURE);
	}
}


// send_msg_from_client: envoyer des messages envoyés par 'type' 
void send_request_to_server( int clientRespondID, char* cmd_name, char* Args  ) {
	Request clientRequest;

	// get request queue Id from commutateur
	int serverRequestID = get_server_request_id();

	// Init message data
	strcpy(clientRequest.name, cmd_name);   // put the name of the request
	strcpy(clientRequest.params, Args);   // put the params of the request
	clientRequest.type = IPC_CLIENT_TYPE;  // TODO: see if it's really needed
	clientRequest.clientResponseID = clientRespondID;

	int size = sizeof(Request) - sizeof(long);
	// send the message
	if ( msgsnd(serverRequestID, (void *)& clientRequest, size, 0) < 0 ) {
		err_log("send_message.msgsnd", stderr)
		exit(EXIT_FAILURE);
	}
}


// get_request_from_client: recuperer les messages envoyés par 'type' 
Request get_request_from_client (int serverRequestID) {
	Request clientRequest;
	int size = sizeof(Request) - sizeof(long);
	// reception du message du message 
	if ( msgrcv(serverRequestID, (void *)& clientRequest, size, IPC_CLIENT_TYPE, 0) < 0 ) {
		err_log("IPC_receive_message.msgsnd", stderr)
		exit(EXIT_FAILURE);
	}	
	return clientRequest;
}
	

// send_response_to_client: envoyer des messages envoyés par 'type' 
void send_response_to_client( int clientRespondID, char* response) {
	Response clientResponse;

	// Init message data
	strcpy(clientResponse.body, response);
	clientResponse.type = IPC_CLIENT_TYPE;  // TODO: see if it's really needed

	int size = sizeof(Response) - sizeof(long);
	// send the message
	if ( msgsnd(clientRespondID, (void *)& clientResponse, size, 0) < 0 ) {
		err_log("send_message.msgsnd", stderr)
		exit(EXIT_FAILURE);
	}
}


// get_request_from_client: recuperer les messages envoyés par 'type' 
Response get_respond_from_server (int clientRespondID) {
	Response clientResponse;
	int size = sizeof(Response) - sizeof(long);
	// reception du message du message 
	if ( msgrcv(clientRespondID, (void *)& clientResponse, size, IPC_CLIENT_TYPE, 0) < 0 ) {
		err_log("IPC_receive_message.msgsnd", stderr)
		exit(EXIT_FAILURE);
	}	
	return clientResponse;
}


