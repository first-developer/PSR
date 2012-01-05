
// ===============================================
// 		LOGGER: Tools to display errors or debugs
// ===============================================

// Hacks
// -----


// Inclusions
// ----------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Constants
// ----------


// Structures
// ----------



// Functions
// ---------

// Cancel : 
void cancel( char* msg) {
	perror(msg);
	exit(-1);
}


// ===============================================
