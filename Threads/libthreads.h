/* PSR/Threads/libthreads.h  */


// On previent les inclusions multiples 
#ifdef 	HEADER_FILE
#define HEADER_FILE

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

int lanceThread(void (*fn) (int), int);


#ifdef DEBUG
#endif

#endif
