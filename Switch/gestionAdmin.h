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


// Structures 
// ----------

typedef struct Admin{
	int key;  // IPC key for requesting
} Admin;

typedef struct AdminList{
	Admin list[NBR_MAX_ADMIN];
} AdminList;


// Fonctions
// ---------


Admin* create_admin( int AdminRequestKey);
AdminList* create_admin_list();


#endif