/** fichier commutateur.c **/

/******************************************************************/
/** Commutateur virtuel pour etablir des VPN                     **/
/******************************************************************/

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
#include "commutateur.h" 

/** Constantes **/

#define		DEFAULT_PORT	4000
#define  	MAX_CONNEXIONS	10
#define 	MAX_SIZE		1000



/** Variables publiques **/

/** Variables statiques **/

/** Fonctions utiles  **/
unsigned short int get_port_from_options(int argc,char **argv) {
	int option_index = 0;
	unsigned short int port;
	char options; // Main option
	
	static struct option long_options[] = {
	   {"port", 0, 0, 'p'}
	};

	port = DEFAULT_PORT;

	// Handle options passed to ME!
	if ( argc > 2 ) {
		if ((options = getopt_long(argc, argv, "p", long_options, &option_index)) != -1 )  {
			switch(options) {
				case 'p':
					port = atoi(argv[2]);
					break;	
				default: break;
			}
			
		}

	}
	return port;
}


void *mortdefils (int sig) {};


// traitement 
void *traitement (int fd) {
	char buffer[MAX_SIZE];
	
	FILE* fserver;
	fserver = fdopen(fd, "a+");

	while (fgets(buffer, MAX_SIZE, fserver) != NULL) {
		printf(" > %s", buffer);
	}
	fclose(fserver);
};


/** Procedure principale **/

int main(int argc,char **argv) {
	unsigned short int port;
	int fd_socket_server;


	// get the port from user input 
	port = get_port_from_options(argc, argv);
	#ifdef DEBUG
		printf(" Port attribué : %d\n", port);
	#endif

	// récuperation du descripteur de la socket 
	fd_socket_server = initialisationServeur(&port , MAX_CONNEXIONS);

	// Lancement de la boucle d'écoute
	if (boucleServeur(fd_socket_server, traitement) == -1) {
		perror("Main.boucleServeur \n");
		exit(1);
	}

}
