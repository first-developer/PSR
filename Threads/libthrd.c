/* PSR/Threads/libthrd.h  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <pthread.h>

#include "libthrd.h"


//  Structures
// ------------

typedef struct  {
  void (*fn)(int);
  int param;
} Thread_param;


// Variables globales
// ------------------

pthread_mutex_t mutex_client = PTHREAD_MUTEX_INITIALIZER;


// Fonctions
// ----------

// Cancel : perror suivi d'un exit 
void cancel( char* msg) {
  perror(msg);
  exit(-1);
}

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
int lanceThread(void (*fn)(int), int fn_param) {
	int statut;
	pthread_t _thread;
  
  // init thread param
  Thread_param *thd_param = (Thread_param *)malloc(sizeof(Thread_param));

// Set thread param
  thd_param->fn = fn;
  thd_param->param = fn_param;

	// on crée le thread associé au port
	statut = pthread_create(&_thread, NULL, (void *)thread_handler, (void*)thd_param);
	if (statut != 0) {
		cancel("lanceThread.pthread_create");
	}
	statut = pthread_detach(_thread);
	return statut;
}

// primitives de gestion des semaphores

void P(int S) {
  pthread_mutex_lock(& mutex_client);
}

void V(int S) {
  pthread_mutex_unlock(& mutex_client); 
}