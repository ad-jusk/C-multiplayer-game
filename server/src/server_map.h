#ifndef SERVER_MAP
#define SERVER_MAP

#include "server_libs.h"

//SERVER
int load_map();
int start_server();
void init_colors();
void set_camp();
void set_bushes();
void set_collectibles(int num_of_coins, int num_of_treasures, int num_of_large_treasures);
void set_current_server_status_and_map();
void fill_command_window();
void set_legend();

//ROUND
void display_players();
void display_beasts();
void collect_money();
void deposit_money();
void check_if_bush(int index);
void check_if_collission();
void set_death_point(int index);
void set_death_point_collission(int index1, int index2);
void pick_up_loot(int index);

#endif