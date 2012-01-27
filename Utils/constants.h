
// ==========
// Constants
// ==========



// Avoid multiple inclusions
#ifndef CONSTANTS_H
#define CONSTANTS_H


// Inclusions
// ----------


// Macro 
// -----

#ifdef DEBUG
	#define sep() fprintf(stderr, "--------------------------------------------------------\n");
#else
	#define sep()
#endif


// Constants
// ---------

#define   MIN_BUFFER_SIZE   256
#define   BUFFER_SIZE   	512 
#define   MAX_BUFFER_SIZE   1024 
#define   TRUE              1
#define   FALSE             0
#define   REQUIRED          1
#define   NOT_REQUIRED      0
#define   EMPTY_PARAM		""
#define   EMPTY_STRING		""
#endif
