/*  PSR/IPC/libipc.h  */

// Constantes
// ----------

#define FILE_PATH	"."
#define LIBIPC_ID	0	
// Prototypes 
// ----------

void cancel( char* msg);
void send_message( int cle, int type, char* data  );
void receive_message( int cle, int type, char* data  );

#ifdef DEBUG
#endif
