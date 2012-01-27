// ===============
// LOGGER
// ===============


#ifndef LOGGER_H
#define LOGGER_H


// Inclusions
// ----------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>



// Constants
// ----------


// colors
#define BLACK   "\033[0;30m"
#define BLUE    "\033[0;34m"
#define CYAN    "\033[0;36m"
#define BCYAN   "\033[;36m"
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
#define LOG_OK_COLOR "\033[1;42;37m"
#define LOG_HELP_COLOR "\033[1;43;37m"
#define START_LOG_COLOR "\033[1;45;37m"
#define START_ERRLOG_COLOR "\033[1;41;37m"
#define CT_LOG_COLOR  "\033[0;47;30m"
#define END_LOG_COLOR "\033[0m"

// Structures
// ----------


// Macros
// ------

#ifdef DEBUG
  #define log(msg, output)      fprintf(output, "%s\n", msg);
  #define err_log(msg, output)  fprintf(output, "%s %s\n", msg, strerror(errno));
#else 
  #define log(msg, output)
  #define err_log(msg, output)
#endif

// Functions



#endif
