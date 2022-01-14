#ifndef SERVER_LIBS
#define SERVER_LIBS

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

#define MAX_BEAST_NUM 5
#define MAX_PLAYER_NUM 4
#define MAP_HEIGHT 25
#define MAP_WIDTH 53
#define STATUS_BAR_HEIGHT 15
#define STATUS_BAR_WIDTH 48

#define COINS_AMOUNT 5
#define TREASURES_AMOUNT 5
#define LARGE_TREASURES_AMOUNT 5
#define BUSHES_AMOUNT 10

#define CAMP_X 24
#define CAMP_Y 12

struct player_t{
    int x;
    int y;
    int spawn_x;
    int spawn_y;
    int deaths;
    int money_carried;
    int money_brought;
    int PID;
    char type[10];
    char fifo_to_read[30];
    char fifo_to_write[30];
    char is_in;
    char char_to_display;
    char on_bush;
};

struct beast_t{
    int x;
    int y;
};

struct server_t{
    int num_of_beasts;
    int num_of_players;
    int PID;
    int round;
    struct player_t players[MAX_PLAYER_NUM];
    struct beast_t beasts[MAX_BEAST_NUM];
    WINDOW* map;
    WINDOW* status;
};

extern struct server_t server;
extern sem_t round_start;
extern sem_t round_end;
extern sem_t player1_is_in;
extern sem_t player2_is_in;
extern sem_t player3_is_in;
extern sem_t player4_is_in;
extern sem_t player1_finished;
extern sem_t player2_finished;
extern sem_t player3_finished;
extern sem_t player4_finished;
extern sem_t beast_finished;


#endif