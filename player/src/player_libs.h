#ifndef PLAYER_LIBS
#define PLAYER_LIBS

#include <ncurses.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


struct player_t{
    int x;
    int y;
    int index;
    int server_PID;
    int camp_pos_x;
    int camp_pos_y;
    int deaths;
    int round;
    int money_carried;
    int money_brought;
    char is_camp_discovered;
    WINDOW* map;
    WINDOW* stats;
    char fifo_to_write[30];
    char fifo_to_read[30];
};

void connect_to_server();
void get_map_data();
void send_move(int* move);
void send_PID();
void set_current_map_data();

extern struct player_t player;

#endif