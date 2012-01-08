/*  PSR/IPC/libipc.h  */

#ifndef LIBIPC_HEADER
#define LIBIPC_HEADER

// Structures
// ----------

#define IPC_MESSAGE_SIZE	1024
#define IPC_FILE_PATH		"."
#define IPC_DEFAULT_ID		0
#define NO_MQUEUE_ID		-1

#define RESPONDER_IPC_KEY 		10

#define NBR_MAX_ADMIN		5
#define NBR_MAX_COMMUTATOR 	5


// Variables
// ---------

 extern int commutatorRequestQueueID;

// Macros
// ------


// Prototypes 
// ----------

int generate_ICP_key();
void IPC_send_message( int ICP_key, int IPC_type, char* IPC_data  );
void IPC_receive_message( int ICP_key, int IPC_type, char* IPC_data  );
int generate_requester_ICP_key();
int generate_responder_ICP_key();
int create_IPC_message_queue(int ICP_key);
int generate_reponse_queue_id();
int generate_request_queue_id();
void destroy_IPC_message_queue(int mqueueID);
void destroy_response_queue_id(int mqueueID);
void destroy_request_queue_id(int mqueueID);


#endif



