#ifndef BEAST
#define BEAST

#include "server_libs.h"


void beast_init();
void* beast_move(void* index);
void make_a_move(int i);
int search_for_player(int index, int* direction);
#endif