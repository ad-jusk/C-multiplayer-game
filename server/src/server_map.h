#ifndef SERVER_MAP
#define SERVER_MAP

#include "server_libs.h"

//SERVER
int load_map();
int start_server();
void set_camp();
void set_bushes();
void set_collectibles(int num_of_coins, int num_of_treasures, int num_of_large_treasures);
void set_current_server_status_and_map();

//ROUND
void display_players();
void display_beasts();
void collect_money();
void deposit_money();
void check_if_bush(int index);

#endif