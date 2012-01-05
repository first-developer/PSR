/*  PSR/IPC/libipc.h  */

// Structures
// ----------

#define 	IPC_MESSAGE_SIZE			1024
#define 	IPC_FILE_PATH				"."
#define 	IPC_DEFAULT_ID				0
#define 	IPC_KEY_GENERATION_TYPE		1

// Macros
// ------


// Prototypes 
// ----------

int generate_admin_ICP_key();
void IPC_send_message( int ICP_key, int IPC_type, char* IPC_data  );
void IPC_receive_message( int ICP_key, int IPC_type, char* IPC_data  );

#ifdef DEBUG
#endif
