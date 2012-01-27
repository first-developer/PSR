

#ifndef ADMIN_H
#define ADMIN_H





// Structures 
// ----------


// Variables
// ----------


// Macro 
// -----





// Prototypes
// ----------


// process_admin_requests:
void process_admin_requests(int argc,char **argv);
void validates_arguments(char* Args);
int nbr_args_is_valid(int argc);
void process_request ( char* cmd_name, char* Args, int AdminResponseID);
int get_command_id_from_command_name( char* cmd_name);
void process_request_and_wait_response(char* cmd_name, char* params);



#endif