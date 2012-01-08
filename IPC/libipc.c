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

//int requesterKeys[NBR_MAX_ADMIN];			// tableau stockant les clé admins generées
int requesterQueueIds[NBR_MAX_ADMIN];		// tableau stockant les id de fle de message 
											//   	correspond aux clé admins générées
int responderQueueIds[NBR_MAX_COMMUTATOR];	// tableau stockant les id de fle de message 
											//   	correspond aux clé de commutateurs
int current_requester = 0;					// la clé du client admin courant
int current_responder = 0;					// la clé du commutateur courant
int reponseQueueID;
int requestQueueID;			
int requesterCount = 0;						// compteur global de client admins (requesters)



// Structures
// ----------

typedef struct {
	long type;
	char data[IPC_MESSAGE_SIZE];
} IPC_message;


// Fonctions 
// ----------

// generate_admin_ICP_key: pour créer un cle IPC pour l'admin
int generate_requester_ICP_key() {
	if (current_requester == NBR_MAX_ADMIN)  {// atteint le max d'admin
		err_log("generate_requester_ICP_key: Le max d'admin client est atteint")
		exit(EXIT_FAILURE);
	}
	return current_requester++;
}


// generate_commmutateur_ICP_key: pour créer un cle IPC pour le commutateur
int generate_responder_ICP_key() {
	if (current_responder == NBR_MAX_COMMUTATOR)  {// atteint le max d'admin
		err_log("generate_responder_ICP_key: Le max de commutateur est atteint")
		exit(EXIT_FAILURE);
	}
	return current_responder++;
}


// create_IPC_message_queue: pour créer une file de message
int create_IPC_message_queue(int ICP_key) {
	int mqueue_id; // identifiant de la file de message 

	// On recupere l'id de la file s'il existe ou on en crée une.
	if ( (mqueue_id = msgget(ICP_key, IPC_CREAT | 0600)) == -1 ){
		err_log("send_message.msgget")
		return -1;
	}
	return mqueue_id;
}


// generate_reponse_queue_id: crée l'id 
//		de la file de message reponse et renvoie cet id (mqueue_id).
int generate_reponse_queue_id() {
	key_t requesterKey;
	int mqueueID;
	// on recupere la clé du client courant créé par 'generate_requester_ICP_key'
	if (current_requester == 0) {// Aucun client admin 
		err_log("generate_reponse_queue_id: Aucun client admin ")
		exit(EXIT_FAILURE);
	}

	requesterKey = current_requester;

	mqueueID = create_IPC_message_queue(requesterKey);
	// Ajoute l'id de la file de reponse du client crée
	requesterQueueIds[current_requester] = mqueueID;

	return mqueueID;
}


// generate_request_queue_id: crée la clé et l'id 
//		de la file de message requête et renvoie cet id (mqueue_id).
int generate_request_queue_id() {
	key_t responderKey;
	// on recupere la clé du commutateur courant créé par 'generate_responder_ICP_key'
	if (current_responder == 0) {// Aucun commutateur 
		err_log("generate_request_queue_id: Aucun commutateur ")
		exit(EXIT_FAILURE);
	}

	responderKey = current_responder;

	int mqueueID = create_IPC_message_queue(responderKey);
	// Ajoute l'id de la file de reponse du client crée
	responderQueueIds[current_responder] = mqueueID;
	return mqueueID;
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
	// TODO: Uitliser une boucle pour etre sûr de supprimer le bon mqueueID
	// On vide le contenu du client admin courant dans le tableau 'requesterQueueIds'  
	requesterQueueIds[current_requester] = NO_MQUEUE_ID;
	current_requester--;
	// TODO : verifier que 'current_requester' est bien nul à la fin.
}


// destroy_request_queue_id: pour créer la file de message reponse
void destroy_request_queue_id(int mqueueID) {
	// On tente de détruire la file de message reponse
	destroy_IPC_message_queue(mqueueID);
	// TODO: Uitliser une boucle pour etre sûr de supprimer le bon mqueueID
	// On vide le contenu du commutateur courant dans le tableau 'responderQueueIds'  
	responderQueueIds[current_responder] = NO_MQUEUE_ID;
	current_responder--;
	// TODO : verifier que 'current_responder' est bien nul à la fin.
}