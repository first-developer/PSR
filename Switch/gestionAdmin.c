/******************************************************/
/* Ce fichier contient les fonctions permettant de    */
/* gerer l'agent d'administration.                    */
/******************************************************/

/** Fichiers d'inclusion **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <getopt.h>


#include "libnet.h"
#include "libthrd.h"
#include "gestionConnexions.h"
#include "libipc.h"
#include "gestionAdmin.h"
#include "commutateur.h"
#include "logger.h"

// Constants
// ---------

// External variables
// -------------------

// Fonctions principale de gestion de l'agent d'administration 
// ------------------------------------------------------------

// create_admin: crée et initialise un admin
Admin* create_admin( int AdminRequestKey) {
	Admin* admin = (Admin*)malloc(sizeof(Admin));
	if (admin == NULL) {
		err_log(("create_admin.malloc"))
		exit(EXIT_FAILURE);
	}
	admin->key = AdminRequestKey;
	return admin;	
}

// create_admin_list: create a list of admins
AdminList* create_admin_list() {
	AdminList* AL = (AdminList*)malloc(sizeof(AdminList));
	if (AL == NULL) {
		err_log(("create_admin_list.malloc"))
		exit(EXIT_FAILURE);
	}
	return AL;
}