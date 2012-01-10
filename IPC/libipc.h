/*  PSR/IPC/libipc.h  */

#ifndef LIBIPC_HEADER
#define LIBIPC_HEADER

// Structures
// ----------

#define IPC_MESSAGE_SIZE	1024
#define IPC_FILE_PATH		"."
#define IPC_DEFAULT_ID		0
#define NO_MQUEUE_ID		-1

#define RESPONDER_IPC_KEY 		1
#define PRIVATE_ICP_KEY			2

#define NBR_MAX_ADMIN		5
#define NBR_MAX_COMMUTATOR 	5


// Variables
// ---------

 //extern int commutatorRequestQueueID;

// Macros
// ------


// Prototypes 
// ----------

int create_responder_IPC_message_queue();
int get_responder_IPC_message_queue();
int create_requester_IPC_message_queue();
void destroy_IPC_message_queue(int mqueueID);
void IPC_send_message( int mqueueID, int IPC_type, char* data_snd  );
void IPC_receive_message( int mqueueID, int IPC_type, char* data_rcv  );
void destroy_response_queue_id(int mqueueID);
void destroy_request_queue_id(int mqueueID);


#endif



