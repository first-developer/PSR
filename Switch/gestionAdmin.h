/**********************************************************************/
/* Ce fichier contient les déclarations internes pour les fonctions   */
/* de gestion de l'administrateur                                     */
/**********************************************************************/
#ifndef HEADER_GEST_ADMIN
#define HEADER_GEST_ADMIN

// Inclusions
// ----------

// Constantes
// ----------

#define NBR_MAX_ADMIN	5

#define	ADMIN_REQUEST_TYPE		1

// Structures 
// ----------

typedef struct Admin{
	int IPCkey;  // IPC key for requesting
} Admin;

typedef struct AdminList{
	Admin list[NBR_MAX_ADMIN];
} AdminList;


// Fonctions
// ---------

void process_admin_requests(int argc,char **argv);
void validates_arguments(char* Args) ;
int nbr_args_is_valid(int argc);
int get_command_id_from_command_name( char* cmd_name);
void process_request ( char* cmd_name, char* Args, int AdminResponseQueueID);
AdminList* create_admin_list() ;
Admin* create_admin( int AdminRequestKey);
#endif