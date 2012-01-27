/* PSR/Threads/libthrd.h  */

#ifndef LIBTHRD_H
#define LIBTHRD_H

// Constantes 
// ----------




// Fonctions 
// ----------

void* thread_handler(void *arg);
void lanceThread(void (*fn) (int), int);
void P(int);
void V(int);

#endif
