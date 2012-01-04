/* PSR/Threads/libthreads.h  */


// On previent les inclusions multiples 
#ifdef 	HEADER_FILE
#define HEADER_FILE
#endif

/**** Constantes ****/

// ====================
//	STRUCTURES
// ====================

// struct use in 'initiate_thread'
/*typedef struct  {
	void (*fn)(int);
  int param;
}Thread_param;
 */

/**** Fonctions ****/
void * thread_handler(void *arg);
int lanceThread(void (*fn) (int), int);


#ifdef DEBUG
#endif

