
// ===============================================
// 		LOGGER: Tools to display errors or debugs
// ===============================================

// Hacks
// -----

// Avoid multiple inclusions
#ifndef LOGGER_HEADER
#define LOGGER_HEADER


// Inclusions
// ----------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Constants
// ----------



// colors
#define BLACK   "\033[0;30m"
#define BLUE    "\033[0;34m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define RED     "\033[0;31m"  
#define MAGENTA "\033[0;35m" 
#define BBLACK   "\033[1;30m"
#define BBLUE    "\033[1;34m"
#define BGREEN   "\033[1;32m"
#define BYELLOW  "\033[1;33m"
#define BRED     "\033[1;31m"  
#define BMAGENTA "\033[1;35m"  
#define START_LOG_COLOR "\033[1;45;37m"
#define START_ERRLOG_COLOR "\033[1;41;37m"
#define CT_LOG_COLOR 	"\033[0;47;30m"
#define END_LOG_COLOR	"\033[0m"

// Structures
// ----------



// Macros
// ------


// Macros
// ------

#ifdef DEBUG
	#define log_title(msg,color) 	printf("%s %s %s ", color, msg, CT_LOG_COLOR);
	#define log_ct(msg) 			printf("%s %s\n", msg, END_LOG_COLOR);
	#define err_log_ct(msg) 		printf("%s %s %s\n ", msg, strerror(errno), END_LOG_COLOR);
	#define log(msg) 				log_title(("LOGGER"), (START_LOG_COLOR)) log_ct(msg) end_log()
	#define err_log(msg) 			log_title(("ERROR"), (START_ERRLOG_COLOR)) err_log_ct(msg) end_log()
	#define start_log(msg) 			log_title(("LOGGER"), (START_LOG_COLOR)) log_ct(msg)
	#define end_log() 				printf("\n%s", END_LOG_COLOR);
#else 
	#define log(msg)
#endif

// Functions