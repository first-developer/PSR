/*  PSR/IPC/libipc.h  */

// Constantes
// ----------

// Prototypes 
// ----------

void send_message( int cle, int type, char* data  );
void receive_message( int cle, int type, char* data  );

#ifdef DEBUG
#endif
