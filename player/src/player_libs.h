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
#include <semaphore.h>
#define CAMP_X 24
#define CAMP_Y 12


#define BEAST_PAIR 1
#define PLAYER_PAIR 2
#define TREASURE_PAIR 3
#define CAMP_PAIR 4

struct player_t{
    int x;
    int y;
    int index;
    int server_PID;
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

int connect_to_server();
void get_map_data();
void send_move(int* move);
void send_PID();
void set_current_map_data();
void init_colors();

extern struct player_t player;
extern sem_t* player_sem;
#endif