#ifndef BEAST
#define BEAST

#include "server_libs.h"


void beast_init(int i);
void* beast_move(void* index);
void make_a_move(int i);
int search_for_player(int index, int* direction);
void beast_correct(int index);
#endif