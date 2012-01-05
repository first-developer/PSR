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


// Structures
// ----------

typedef struct {
	long type;
	char data[IPC_MESSAGE_SIZE];
} IPC_message;


// Fonctions 
// ----------

// generate_admin_ICP_key: pour créer un cle IPC pour l'admin
int generate_ICP_key() {
	key_t adminKey;

	if ( (adminKey = ftok(IPC_FILE_PATH, IPC_DEFAULT_ID)) == -1) {
		err_log("generate_admin_ICP_key.ftok")
		return -1;
	}
	return (int) adminKey;

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

// send_message: envoyer des messages envoyés par 'type' 
void IPC_send_message( int ICP_key, int IPC_type, char* data  ) {
	IPC_message IPC_msg;
	int mqueue_id;

	// on verifie que le type est valide 
	if (IPC_type <= 0 ) {
		log("IPC_send_message : invalide type")
		exit(EXIT_FAILURE);
	}

	// creation de l' id de la file de message
	if ( (mqueue_id = create_IPC_message_queue(ICP_key)) == -1) {
		err_log("IPC_send_message.create_IPC_message_queue")
		exit(EXIT_FAILURE);
	}

	// initialisation des donnees du message 
	strcpy(IPC_msg.data, data);
	IPC_msg.type = IPC_type;

	// Envoie du message 
	if ( msgsnd(mqueue_id, (void *)&IPC_msg, IPC_MESSAGE_SIZE, 0) < 0 ) {
		err_log("send_message.msgsnd")
		exit(EXIT_FAILURE);
	}
}


// receive_message: recuperer les messages envoyés par 'type' 
void IPC_receive_message( int ICP_key, int IPC_type, char* data  ) {
	IPC_message IPC_msg;
	int mqueue_id; // identifiant de la file de message 

	// on verifie que le type est valide 
	if (IPC_type <= 0 ) {
		log("IPC_receive_message : invalide type")
		exit(EXIT_FAILURE);		
	}

	// initialisation des donnees du message 
	strcpy(IPC_msg.data, data);
	// IPC_msg.data[IPC_MESSAGE_SIZE] = '\0';
	IPC_msg.type = IPC_type;

	// creation de l' id de la file de message
	if ( (mqueue_id = create_IPC_message_queue(ICP_key)) == -1) {
		err_log("IPC_send_message.create_IPC_message_queue")
		exit(EXIT_FAILURE);
	}

	// Envoie du message 
	if ( msgrcv(mqueue_id, (void *)& IPC_msg, IPC_MESSAGE_SIZE, IPC_type, 0) < 0 ) {
		err_log("IPC_receive_message.msgsnd")
		exit(EXIT_FAILURE);
	}	
}
	