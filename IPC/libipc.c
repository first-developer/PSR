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
	long type;
	char data[MAX_SIZE];
} IPC_message;




// Fonctions 

// Cancel : perror following by exit 
void cancel( char* msg) {
	perror(msg);
	exit(-1);
}

// send_message: envoyer des messages envoyés par 'type' 
void send_message( int cle, int type, char* data  ) {
	IPC_message message;
	int ipc_file_id; // identifiant de la file de message 

	// on verifie que le type est valide 
	if (type <= 0 ) {
#ifdef DEBUG
		cancel("send_message : invalide type");
#endif
	}

	// initialisation du message 
	strcpy(message.data, data);
	message.data[MAX_SIZE] = '\0';

	// On recupere l'id de la file s'il existe ou on en cré une.
	if ( (ipc_file_id = msgget(cle, IPC_CREAT | 0600)) == -1 ){
#ifdef DEBUG
		cancel("send_message.msgget");
#endif
	}

	// Envoie du message 
	if ( msgsnd(ipc_file_id, (void *)&message, MAX_SIZE, 0) < 0 ) {
#ifdef DEBUG
		cancel("send_message.msgsnd");	
#endif
	}
}


// receive_message: recuperer les messages envoyés par 'type' 
void receive_message( int cle, int type, char* data  ) {
	IPC_message message;
	int ipc_file_id; // identifiant de la file de message 

	// on verifie que le type est valide 
	if (type <= 0 ) {
#ifdef DEBUG
		cancel("send_message : invalide type");
#endif
	}

	// initialisation du message 
	strcpy(message.data, data);
	message.data[MAX_SIZE] = '\0';

	// On recupere l'id de la file s'il existe ou on en cré une.
	if ( (ipc_file_id = msgget(cle, IPC_CREAT | 0600)) == -1 ){
#ifdef DEBUG
		cancel("send_message.msgget");
#endif
	}

	// reception du message 
	if ( msgrcv(ipc_file_id, (void *)&message, MAX_SIZE, type, 0) < 0 ) {
#ifdef DEBUG
		cancel("send_message.msgrcv");	
#endif
	}	
}