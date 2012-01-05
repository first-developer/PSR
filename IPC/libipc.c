/*  PSR/IPC/libipc.c  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "libipc.h"

// Variables
// ----------

#define 	MAX_SIZE	256

// Structures
// ----------

typedef struct {
	long mtype;
	char mdata[MAX_SIZE];
} IPC_message;




// Fonctions 

// Cancel : perror following by exit 
void cancel( char* msg) {
	perror(msg);
	exit(-1);
}

// send_message: envoyer des messages envoyés par 'type' 
void send_message(int type, char* data  ) {
	key_t cle;
	IPC_message message;
	int mqueue_id; // identifiant de la file de message 

	// On laisse le système générer la clé
	if ( (cle == ftok(FILE_PATH, LIBIPC_ID)) == -1 ) {
#ifdef DEBUG
		cancel("receive_message.ftok");
#endif
	}

	// on verifie que le type est valide 
	if (type <= 0 ) {
#ifdef DEBUG
		cancel("send_message : invalide type");
#endif
	}

	// initialisation des donnees du message 
	strcpy(message.mdata, data);
	message.mdata[MAX_SIZE] = '\0';
	message.mtype = type;

	// On recupere l'id de la file s'il existe ou on en cré une.
	if ( (mqueue_id = msgget(cle, IPC_CREAT | 0600)) == -1 ){
#ifdef DEBUG
		cancel("send_message.msgget");
#endif
	}

	// Envoie du message 
	if ( msgsnd(mqueue_id, (void *)&message, MAX_SIZE, 0) < 0 ) {
#ifdef DEBUG
		cancel("send_message.msgsnd");	
#endif
	}
}


// receive_message: recuperer les messages envoyés par 'type' 
void receive_message( int type, char* data  ) {
	key_t cle;
	IPC_message message;
	int mqueue_id; // identifiant de la file de message 

	// On laisse le système générer la clé
	if ( (cle == ftok(FILE_PATH, LIBIPC_ID)) == -1 ) {
#ifdef DEBUG
		cancel("receive_message.ftok");
#endif
	}

	// on verifie que le type est valide 
	if (type <= 0 ) {
#ifdef DEBUG
		cancel("send_message : invalide type");
#endif
	}

	// On recupere l'id de la file s'il existe ou on en cré une.
	if ( (mqueue_id = msgget(cle, IPC_CREAT | 0600)) == -1 ){
#ifdef DEBUG
		cancel("send_message.msgget");
#endif
	}

	// reception du message 
	if ( msgrcv(mqueue_id, (void *)&message, MAX_SIZE, type, 0) < 0 ) {
#ifdef DEBUG
		cancel("send_message.msgrcv");	
#endif
	}
	
	// get data from the IPC message
	data = message.mdata;	
}