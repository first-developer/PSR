
#ifndef GEST_CONNEXION
#define GEST_CONNEXION



// Constantes
// ----------

#define MAX_CONNEXIONS 		10
#define SYNTAX_MSG 			"SYNTAX\n"
#define OK_MSG 				"OK\n"
#define BOUNDS_MSG 			"BOUNDS\n"
// Prototypes
// ----------

void handle_connection (int);
void process_slight_activity_for( int);
int we_are_in_bounds_condition(int port, FILE* connected_socket_file);

#endif