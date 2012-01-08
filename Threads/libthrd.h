/* PSR/Threads/libthrd.h  */

#ifndef HEADER_LIBTHRD
#define HEADER_LIBTHRD

// Constantes 
// ----------




// Fonctions 
// ----------

void cancel( char* msg);
void* thread_handler(void *arg);
void lanceThread(void (*fn) (int), int);
void P(int);
void V(int);


#endif