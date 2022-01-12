#include "server_libs.h"
#include "beast.h"
#include "server_map.h"
#include "server_functions.h"

struct server_t server;
sem_t player1_is_in;
sem_t player2_is_in;
sem_t player3_is_in;
sem_t player4_is_in;
sem_t round_start;
sem_t round_end;
sem_t beast_finished;
sem_t player1_finished;
sem_t player2_finished;
sem_t player3_finished;
sem_t player4_finished;

int main(){
    
    srand(time(NULL));
    initscr();
    curs_set(0);
    halfdelay(10);
    noecho();

    //SERVER MAP
    server.map = newwin(MAP_HEIGHT,MAP_WIDTH,0,0);
    keypad(server.map,1);

    //SERVER STATUS
    server.status = newwin(STATUS_BAR_HEIGHT,STATUS_BAR_WIDTH,0,MAP_WIDTH+2);
    box(server.status,0,0);

    //START_SERVER
    if(start_server()){
        endwin();
        return 1;
    }

    //INIT_MUTEXES
    init_mutexes();

    //THREADS
    pthread_t beasts[MAX_BEAST_NUM];
    pthread_t players_wait;
    pthread_t players[MAX_PLAYER_NUM];

    int input;
    int player_indexes[4] = {0,1,2,3};
    int beast_indexes[5] = {0,1,2,3,4};

    beast_init();
    pthread_create(&beasts[server.num_of_beasts-1],NULL,beast_move,&beast_indexes[server.num_of_beasts-1]);
    pthread_create(&players_wait,NULL,wait_for_players,NULL);
    pthread_create(&players[0],NULL,manage_player1,&player_indexes[0]);
    pthread_create(&players[1],NULL,manage_player2,&player_indexes[1]);
    pthread_create(&players[2],NULL,manage_player3,&player_indexes[2]);
    pthread_create(&players[3],NULL,manage_player4,&player_indexes[3]);

    do{
        input = wgetch(server.map);
        if(input == 'b' || input == 'B'){
            if(server.num_of_beasts == MAX_BEAST_NUM){
                continue;
            }
            beast_init();
            pthread_create(&beasts[server.num_of_beasts-1],NULL,beast_move,&beast_indexes[server.num_of_beasts-1]);
        }
        run_round();
        set_current_server_status_and_map();

    }while(input != 'q');

    server_shut_down();

    return 0;
}