#ifndef SERVER_FUNCTIONS
#define SERVER_FUNCTIONS

#include "server_libs.h"

void server_shut_down();
void init_mutexes();

//PLAYERS
void* wait_for_players(void* arg);
void spawn_player(char character);
void* manage_player1(void* arg);
void move_player(int* move, int index, char character);

#endif