#include "server_functions.h"

void server_shut_down(){
    endwin();
    sem_destroy(&player1_is_in);
    
}

void init_mutexes(){
    sem_init(&player1_is_in,0,0);
}

void spawn_player(char character){
    int x, y;
    do{
        x = rand() % (MAP_WIDTH - 1);
        y = rand() % MAP_HEIGHT;
    }while(mvwinch(server.map,y,x) == (char)219);
    server.players[server.num_of_players].x = x;
    server.players[server.num_of_players].y = y;
    server.num_of_players++;
    mvwaddch(server.map,y,x,character);
}

void* wait_for_players(void* arg){
    //CONNECCTION FIFO FOR ALL PLAYERS
    if(mkfifo("connection_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    //PLAYERS'S ACTIONS SENT TO SERVER
    if(mkfifo("player1_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    int fd = open("connection_fifo", O_WRONLY);
    write(fd,&server.PID,sizeof(int));
    close(fd);
    spawn_player('1');
    sem_post(&player1_is_in);
    return NULL;
}


void* manage_player1(void* arg){
    sem_wait(&player1_is_in);
    int fd = open("player1_moves_fifo",O_RDONLY);
    int move;
    while(1){
        read(fd,&move,sizeof(int));
        move_player(&move,0,'1');
        sleep(1);
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