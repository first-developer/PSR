/*  PSR/IPC/libipc.c  */

// Inclusions
// -----------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "logger.h"
#include "libipc.h"


// Constants
// ----------

int requesterQueueIds[NBR_MAX_ADMIN];		// tableau stockant les id de fle de message 
											//   	correspond aux clé admins générées
int current_requester = 0;					// la clé du client admin courant
	


// Structures
// ----------

typedef struct {
	long type;
	char data[IPC_MESSAGE_SIZE];
	int  queueID;
} IPC_message;


// Fonctions 
// ----------

// create_responder_IPC_message_queue: pour créer une file de message public(commutateur)
//										accessible par la clé public 'RESPONDER_IPC_KEY'
int create_responder_IPC_message_queue() {
	int mqueue_id; // identifiant de la file de message 

	// On crée l'id de la file de message si elle n'existe pas; Dans le cas 
	// contraire on genere une erreur grace au flag 'IPC_EXCL'
	if ( (mqueue_id = msgget(IPC_PRIVATE, IPC_EXCL | IPC_CREAT | 0600)) == -1 ){
		err_log("create_responder_IPC_message_queue.msgget")
		return -1;
	}
	return mqueue_id;
}


// create_requester_IPC_message_queue: pour créer une file de message privée (client)
int create_requester_IPC_message_queue() {
	int mqueue_id; // identifiant de la file de message 
	// On crée l'id de la file de reponse par une clé privée
	if ( (mqueue_id = msgget(RESPONDER_IPC_KEY, IPC_CREAT | 0600)) == -1 ){
		err_log("create_requester_IPC_message_queue.msgget")
		return -1;
	}
	return mqueue_id;
}


// get_requester_IPC_message_queue: recupere l'id de la file de message
int get_requester_IPC_message_queue() {
	int mqueue_id; // identifiant de la file de message 
	// recuperaton de la l'id de la file de requête
	if ( (mqueue_id = msgget(RESPONDER_IPC_KEY, 0600)) == -1 ){
		err_log("get_responder_IPC_message_queue.msgget")
		return -1;
	}
	return mqueue_id;
}
	

// destroy_IPC_message_queue: pour spprime une file de message
void destroy_IPC_message_queue(int mqueueID) {
	int status;  

	// On tente de détruire la file de message 
	status = msgctl(mqueueID, IPC_RMID, NULL);

	if ( status == -1 ){
		err_log("destroy_IPC_message_queue.msgctl")
		exit(EXIT_FAILURE);
	}
}


// IPC_send_message: envoyer des messages envoyés par 'type' 
void IPC_send_message( int mqueueID, int IPC_type, char* data_snd  ) {
	IPC_message IPC_msg;

	// Initialisation des donnees du message 
	strcpy(IPC_msg.data, data_snd);
	IPC_msg.type = IPC_type;
	IPC_msg.queueID = mqueueID;
	// Envoie du message 
	if ( msgsnd(mqueueID, (void *)&IPC_msg, IPC_MESSAGE_SIZE, 0) < 0 ) {
		err_log("send_message.msgsnd")
		exit(EXIT_FAILURE);
	}
}


// IPC_receive_message: recuperer les messages envoyés par 'type' 
void IPC_receive_message( int mqueueID, int IPC_type, char* data_rcv  ) {
	IPC_message IPC_msg;
	
	// reception du message du message 
	if ( msgrcv(mqueueID, (void *)& IPC_msg, IPC_MESSAGE_SIZE, IPC_type, 0) < 0 ) {
		err_log("IPC_receive_message.msgsnd")
		exit(EXIT_FAILURE);
	}	

	// On met la reponse dans la variable 'data_rcv'
	data_rcv = IPC_msg.data;
}
	

// destroy_response_queue_id: pour créer la file de message reponse
void destroy_response_queue_id(int mqueueID) {
	// On tente de détruire la file de message reponse
	destroy_IPC_message_queue(mqueueID);
}


// destroy_request_queue_id: pour créer la file de message reponse
void destroy_request_queue_id(int mqueueID) {
	// On tente de détruire la file de message reponse
	destroy_IPC_message_queue(mqueueID);
}