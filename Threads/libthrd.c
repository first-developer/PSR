/* PSR/Threads/libthrd.h  */

#ifndef LIBTHRD_H
#define LIBTHRD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <pthread.h>

#include "commutateur.h"
#include "libthrd.h"
#include "logger.h"

//  Structures
// ------------

typedef struct  {
  void (*fn)(int);
  int param;
} Thread_param;


// Variables globales
// ------------------

pthread_mutex_t verrous[NBR_MAX_PORT +1]; // verrous[0] = mutex lié au commutateur 
                                          // verrous[i] ceux liés aux port

// Fonctions
// ----------

// Thread_handler: 
void * thread_handler(void *arg) {
  Thread_param *thd_param = (Thread_param *)arg;
  void  (*fn)(int) = thd_param->fn;
  int   param      = thd_param->param; 
  int p_status; 
  // exec fn with its given param
  fn(param);
  free(thd_param);
  pthread_exit(&p_status);
}


// lanceThread: return 0 if successully done and a number != 0 if not
void lanceThread(void (*fn)(int), int fn_param) {
	int statut;
	pthread_t _thread;
  
  // init thread param
  Thread_param *thd_param = (Thread_param *)malloc(sizeof(Thread_param));

// Set thread param
  thd_param->fn = fn;
  thd_param->param = fn_param;

	// on crée le thread associé au port
	statut = pthread_create(&_thread, NULL, (void *)thread_handler, (void*)thd_param);
	if (statut == -1) {
		err_log(("lanceThread.pthread_create"), stderr)
    exit(EXIT_FAILURE);
	}

  // on détache le thread
	statut = pthread_detach(_thread);
  if (statut == -1) {
    err_log(("lanceThread.pthread_detach"), stderr)
    exit(EXIT_FAILURE);
  }

}

// primitives de gestion des semaphores

void P(int S) {
  // si S=0 c'est que c'est le commutateur sinon on a à faire aux ports
  pthread_mutex_lock(&(verrous[S])); 
}

void V(int S) {
  pthread_mutex_unlock(&(verrous[S])); 
}


#endif