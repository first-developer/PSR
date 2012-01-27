/*  PSR/IPC/libipc.h  */

#ifndef LIBIPC_HEADER
#define LIBIPC_HEADER

// Structures
// ----------

#define MIN_MSG_SIZE		256
#define MSG_SIZE 			512
#define MAX_MSG_SIZE		1024
#define IPC_DEFAULT_ID		0
#define	IPC_CLIENT_TYPE		1
#define	IPC_SERVER_TYPE		1
#define NO_MQUEUE_ID		-1

#define SERVER_PUBLIC_KEY 	11  // clé publique du commutateur

#define NBR_MAX_CLIENT		5
#define NBR_MAX_COMMUTATOR 	5



// Structures
// ----------

#pragma pack(1)
typedef struct {
	long type;
	char name[MIN_MSG_SIZE];	// the name of the request
	char params[MAX_MSG_SIZE];  // params of any request
	int  clientResponseID; 		// Id of client response queue 
} Request;
#pragma pack()

typedef struct {
	long type;
	char body[MAX_MSG_SIZE];
} Response;

// Fonctions 
// ----------

int create_client_response_id();
int create_server_request_id();
int get_server_request_id();
void destroy_response_or_request_id(int id);
void send_request_to_server( int clientRespondID, char*, char*);
Request get_request_from_client (int serverRequestID);
void send_response_to_client( int clientRespondID, char* response); 
Response get_respond_from_server (int clientRespondID);

#endif



