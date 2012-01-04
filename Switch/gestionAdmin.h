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

#define NBR_MAX_CLIENT	5


// Structures 
// ----------

typedef struct {
} AdminClient;

typedef struct {
	AdminClient list[NBR_MAX_CLIENT];
} AdminClientList;


// Fonctions
#endif