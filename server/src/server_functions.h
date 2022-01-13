#ifndef SERVER_FUNCTIONS
#define SERVER_FUNCTIONS

#include "server_libs.h"

void server_shut_down();
void init_semaphores();
void run_round();

//PLAYERS
void* wait_for_players(void* arg);
void spawn_player(char character);
void clear_player_stats(int index);
void* manage_player1(void* arg);
void* manage_player2(void* arg);
void* manage_player3(void* arg);
void* manage_player4(void* arg);
void move_player(int* move, int index, char character);
void read_player_move(int index, int* move);
void send_map_data_to_player(int index);

#endif