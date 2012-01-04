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
#include "commutateur.h"
#include "gestionAdmin.h"

/** Constantes **/

/** Variables externes **/

/** Fonction principale de gestion de l'agent d'administration **/

