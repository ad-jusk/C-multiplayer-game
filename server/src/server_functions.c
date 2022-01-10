#include "server_functions.h"

void server_shut_down(){
    endwin();
    sem_destroy(&player1_is_in);
    sem_destroy(&round_start);
    sem_destroy(&round_end);
    sem_destroy(&beast_finished);
}

void init_mutexes(){
    sem_init(&player1_is_in,0,0);
    sem_init(&round_start,0,0);
    sem_init(&round_end,0,0);
    sem_init(&beast_finished,0,0);
    sem_init(&player1_finished,0,0);
}

void run_round(){
    //START_ROUND
    for(int i = 0;i<server.num_of_beasts + server.num_of_players;i++){
        sem_post(&round_start);
    }
    //WAIT FOR BEASTS TO FINISH
    for(int i = 0;i<server.num_of_beasts;i++){
        sem_wait(&beast_finished);
    }
    if(server.players[0].is_in){
        //WAIT FOR PLAYER 1 TO FINISH
        sem_wait(&player1_finished);
    }

    //DO CODE HERE
    server.round++;


    for(int i = 0;i<server.num_of_beasts + server.num_of_players;i++){
        sem_post(&round_end);
    }
}


//PLAYERS FUNCTIONS

void spawn_player(char character){
    int x, y;
    do{
        x = rand() % (MAP_WIDTH - 1);
        y = rand() % MAP_HEIGHT;
    }while(mvwinch(server.map,y,x) == (char)219);
    server.players[server.num_of_players].x = x;
    server.players[server.num_of_players].y = y;
    server.players[server.num_of_players].is_in = 1;
    server.num_of_players++;
    mvwaddch(server.map,y,x,character);
}

void* wait_for_players(void* arg){

    //FIFO NAMES
    char player1_moves_fifo_name[] = "../server/player1_moves_fifo";
    char info_for_player1_fifo_name[] = "../server/info_for_pla1_fifo";

    //CONNECCTION FIFO FOR ALL PLAYERS
    if(mkfifo("connection_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    //PLAYER'S 1 ACTIONS SENT TO SERVER
    if(mkfifo("player1_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 1 
    if(mkfifo("info_for_pla1_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    int fd = open("connection_fifo", O_WRONLY);
    if(fd == -1){
        exit(2);
    }
    //UPON CONNECTION PLAYER HAS SERVER'S PID AND NAMES OF 2 FIFOS

    write(fd,&server.PID,sizeof(int));
    write(fd,player1_moves_fifo_name,strlen(player1_moves_fifo_name));
    write(fd,info_for_player1_fifo_name,strlen(info_for_player1_fifo_name));
    spawn_player('1');

    sem_post(&player1_is_in);
    close(fd);
    return NULL;
}


void* manage_player1(void* arg){
    sem_wait(&player1_is_in);
    int fd = open("player1_moves_fifo",O_RDONLY);
    //GET PLAYER'S PID FIRST
    read(fd,&server.players[0].PID,sizeof(int));
    int move;
    while(1){
        sem_wait(&round_start);

        read(fd,&move,sizeof(int));
        move_player(&move,0,'1');

        sem_post(&player1_finished);
        sem_wait(&round_end);
        //SEND MAP HERE
    }
    close(fd);

}

void move_player(int* move, int index, char character){
    switch(*move){
        case 'w':
            if(mvwinch(server.map,server.players[index].y-1,server.players[index].x) == (char)219){
                break;
            }
            mvwaddch(server.map,server.players[index].y,server.players[index].x,' ');
            mvwaddch(server.map,server.players[index].y-1,server.players[index].x,character);
            server.players[index].y--;
            break;
        default:
            break;
    }
}