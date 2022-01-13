#include "server_functions.h"

void server_shut_down(){
    endwin();
    sem_destroy(&player1_is_in);
    sem_destroy(&player2_is_in);
    sem_destroy(&player3_is_in);
    sem_destroy(&player4_is_in);
    sem_destroy(&round_start);
    sem_destroy(&round_end);
    sem_destroy(&beast_finished);
    sem_destroy(&player1_finished);
    sem_destroy(&player2_finished);
    sem_destroy(&player3_finished);
    sem_destroy(&player4_finished);
}

void init_semaphores(){
    sem_init(&player1_is_in,0,0);
    sem_init(&player2_is_in,0,0);
    sem_init(&player3_is_in,0,0);
    sem_init(&player4_is_in,0,0);
    sem_init(&round_start,0,0);
    sem_init(&round_end,0,0);
    sem_init(&beast_finished,0,0);
    sem_init(&player1_finished,0,0);
    sem_init(&player2_finished,0,0);
    sem_init(&player3_finished,0,0);
    sem_init(&player4_finished,0,0);
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
        //sem_post(&round_start);
        sem_wait(&player1_finished);
    }
    if(server.players[1].is_in){
        //WAIT FOR PLAYER 2 TO FINISH
        //sem_post(&round_start);
        sem_wait(&player2_finished);
    }
    if(server.players[2].is_in){
        //WAIT FOR PLAYER 3 TO FINISH
        sem_wait(&player3_finished);
    }
    if(server.players[3].is_in){
        //WAIT FOR PLAYER 4 TO FINISH
        sem_wait(&player4_finished);
    }


    //DO CODE REGRDING SINGLE ROUND HERE
    server.round++;

    for(int i = 0;i<server.num_of_beasts + server.num_of_players;i++){
        sem_post(&round_end);
    }
}


//PLAYERS FUNCTIONS

void spawn_player(char character){
    int x, y;
    do{
        x = rand() % (MAP_WIDTH - 2);
        y = rand() % MAP_HEIGHT;
    }while(mvwinch(server.map,y,x) == (char)219);
    server.players[server.num_of_players].x = x;
    server.players[server.num_of_players].y = y;
    server.players[server.num_of_players].spawn_x = x;
    server.players[server.num_of_players].spawn_y = y;
    server.players[server.num_of_players].is_in = 1;
    server.players[server.num_of_players].money_brought = 0;
    server.players[server.num_of_players].money_carried = 0;
    server.players[server.num_of_players].deaths = 0;
    server.num_of_players++;
    mvwaddch(server.map,y,x,character);
}

void clear_player_stats(int index){
    struct player_t* player = &server.players[index];
    mvwaddch(server.map,player->y,player->x,' ');
    player->deaths = 0;
    player->is_in = 0;
    player->money_brought = 0;
    player->money_carried = 0;
    player->PID = 0;
    player->x = 0;
    player->y = 0;
    player->spawn_x = 0;
    player->spawn_y = 0;
    server.num_of_players--;
    strcpy(player->type,"-----");
}

void* wait_for_players(void* arg){
    //FIFO NAMES
    char player1_moves_fifo_name[] = "../server/player1_moves_fifo";
    char info_for_player1_fifo_name[] = "../server/info_for_pla1_fifo";
    char player2_moves_fifo_name[] = "../server/player2_moves_fifo";
    char info_for_player2_fifo_name[] = "../server/info_for_pla2_fifo";
    char player3_moves_fifo_name[] = "../server/player3_moves_fifo";
    char info_for_player3_fifo_name[] = "../server/info_for_pla3_fifo";
    char player4_moves_fifo_name[] = "../server/player4_moves_fifo";
    char info_for_player4_fifo_name[] = "../server/info_for_pla4_fifo";
    int index = 1;

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

    //WAIT FOR PLAYER 1
    write(fd,&server.PID,sizeof(int));
    write(fd,&index,sizeof(int));
    write(fd,player1_moves_fifo_name,strlen(player1_moves_fifo_name));
    write(fd,info_for_player1_fifo_name,strlen(info_for_player1_fifo_name));
    strcpy(server.players[0].fifo_to_read,"player1_moves_fifo");
    strcpy(server.players[0].fifo_to_write,"info_for_pla1_fifo");
    strcpy(server.players[0].type,"HUMAN");
    spawn_player('1');
    close(fd);
    //PLAYER1 IS IN
    sem_post(&player1_is_in);
    index++;

    //<-----------------------------------END OF PLAYER 1 ------------------------------------------------>
    
    //PLAYER'S 2 ACTIONS SENT TO SERVER
    if(mkfifo("player2_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 2 
    if(mkfifo("info_for_pla2_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    sleep(1);
    int fd2 = open("connection_fifo",O_WRONLY);
    //WAIT FOR PLAYER 2
    write(fd2,&server.PID,sizeof(int));
    write(fd2,&index,sizeof(int));
    write(fd2,player2_moves_fifo_name,strlen(player2_moves_fifo_name));
    write(fd2,info_for_player2_fifo_name,strlen(info_for_player2_fifo_name));
    strcpy(server.players[1].fifo_to_read,"player2_moves_fifo");
    strcpy(server.players[1].fifo_to_write,"info_for_pla2_fifo");
    strcpy(server.players[1].type,"HUMAN");
    spawn_player('2');
    close(fd2);
    //PLAYER2 IS IN
    sem_post(&player2_is_in);
    index++;

    //<-----------------------------------END OF PLAYER 2------------------------------------------------>

    //PLAYER'S 3 ACTIONS SENT TO SERVER
    if(mkfifo("player3_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 3 
    if(mkfifo("info_for_pla3_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    sleep(1);
    int fd3 = open("connection_fifo",O_WRONLY);
    //WAIT FOR PLAYER 3
    write(fd3,&server.PID,sizeof(int));
    write(fd3,&index,sizeof(int));
    write(fd3,player3_moves_fifo_name,strlen(player3_moves_fifo_name));
    write(fd3,info_for_player3_fifo_name,strlen(info_for_player3_fifo_name));
    strcpy(server.players[2].fifo_to_read,"player3_moves_fifo");
    strcpy(server.players[2].fifo_to_write,"info_for_pla3_fifo");
    strcpy(server.players[2].type,"HUMAN");
    spawn_player('3');
    close(fd3);
    //PLAYER3 IS IN
    sem_post(&player3_is_in);
    index++;

    //<-----------------------------------END OF PLAYER 3 ------------------------------------------------>

    //PLAYER'S 4 ACTIONS SENT TO SERVER
    if(mkfifo("player4_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 4 
    if(mkfifo("info_for_pla4_fifo",0777) == -1){
        if(errno != EEXIST){
            exit(1);
        }
    }
    sleep(1);
    int fd4 = open("connection_fifo",O_WRONLY);
    //WAIT FOR PLAYER 4
    write(fd4,&server.PID,sizeof(int));
    write(fd4,&index,sizeof(int));
    write(fd4,player4_moves_fifo_name,strlen(player4_moves_fifo_name));
    write(fd4,info_for_player4_fifo_name,strlen(info_for_player4_fifo_name));
    strcpy(server.players[3].fifo_to_read,"player4_moves_fifo");
    strcpy(server.players[3].fifo_to_write,"info_for_pla4_fifo");
    strcpy(server.players[3].type,"HUMAN");
    spawn_player('4');
    close(fd4);
    //PLAYER4 IS IN
    sem_post(&player4_is_in);

    //<-----------------------------------END OF PLAYER 4 ------------------------------------------------>
    return NULL;
}


void* manage_player1(void* arg){
    sem_wait(&player1_is_in);
    int index = *(int*)arg;
    int fd = open(server.players[index].fifo_to_read,O_RDONLY);

    //GET PLAYER'S PID FIRST
    read(fd,&server.players[index].PID,sizeof(int));
    close(fd);

    int move;
    char x = (index+1) + '0';
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
        //MOVE PLAYER
        read_player_move(0,&move);
        if(move == 'q' || move == 'Q'){
            clear_player_stats(0);
            sem_post(&player1_finished);
            break;
        }
        move_player(&move,index,x);
        sem_post(&player1_finished);
        sem_wait(&round_end);

        //SEND MAP
        send_map_data_to_player(index);
    }
    return NULL;
}

void* manage_player2(void* arg){
    sem_wait(&player2_is_in);
    int index = *(int*)arg;
    int fd = open(server.players[index].fifo_to_read,O_RDONLY);

    //GET PLAYER'S PID FIRST
    read(fd,&server.players[index].PID,sizeof(int));
    close(fd);

    int move;
    char x = (index+1) + '0';
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
       
        //MOVE PLAYER
        read_player_move(index,&move);
        if(move == 'q' || move == 'Q'){
            clear_player_stats(1);
            sem_post(&player2_finished);
            break;
        }
        move_player(&move,index,x);
        sem_post(&player2_finished);
        sem_wait(&round_end);
        //SEND MAP
        send_map_data_to_player(index);
    }
    return NULL;

}

void* manage_player3(void* arg){
    sem_wait(&player3_is_in);
    int index = *(int*)arg;
    int fd = open(server.players[index].fifo_to_read,O_RDONLY);

    //GET PLAYER'S PID FIRST
    read(fd,&server.players[index].PID,sizeof(int));
    close(fd);

    int move;
    char x = (index+1) + '0';
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
       
        //MOVE PLAYER
        read_player_move(index,&move);
        if(move == 'q' || move == 'Q'){
            clear_player_stats(2);
            sem_post(&player3_finished);
            break;
        }
        move_player(&move,index,x);

        sem_post(&player3_finished);
        sem_wait(&round_end);

        //SEND MAP
        send_map_data_to_player(index);
    }
    return NULL;

}

void* manage_player4(void* arg){
    sem_wait(&player4_is_in);
    int index = *(int*)arg;
    int fd = open(server.players[index].fifo_to_read,O_RDONLY);

    //GET PLAYER'S PID FIRST
    read(fd,&server.players[index].PID,sizeof(int));
    close(fd);

    int move;
    char x = (index+1) + '0';
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
       
        //MOVE PLAYER
        read_player_move(index,&move);
        if(move == 'q' || move == 'Q'){
            clear_player_stats(3);
            sem_post(&player4_finished);
            break;
        }
        move_player(&move,index,x);

        sem_post(&player4_finished);
        sem_wait(&round_end);

        //SEND MAP
        send_map_data_to_player(index);
    }
    return NULL;

}

void read_player_move(int index, int* move){
    int fd = open(server.players[index].fifo_to_read,O_RDONLY);
    read(fd,move,sizeof(int));
    close(fd);
}

void send_map_data_to_player(int index){
    
    int x = server.players[index].x;
    int y = server.players[index].y;
    char map[5][5];
    int start_x, start_y, end_x, end_y;
    start_x = x - 2;
    start_y = y - 2;
    end_x = x + 2;
    end_y = y + 2;
    if(start_x < 0){
        start_x = 0;
    }
    if(start_y < 1){
        start_y = 0;
    }
    if(end_x > MAP_WIDTH - 2){
        end_x = MAP_WIDTH - 2;
    }
    if(end_y > MAP_HEIGHT - 1){
        end_y = MAP_HEIGHT- 1;
    }
    
    int m = start_x;

    for(int k = 0;k<5;k++){
        for(int l = 0;l<5;l++){
            if(start_x <= end_x && start_y <= end_y){
                map[k][l] = mvwinch(server.map,start_y,start_x);
            }
            else{
                map[k][l] = ' ';
            }
            start_x++;
        }
        start_y++;
        start_x = m;
    }
    int fd = open(server.players[index].fifo_to_write,O_WRONLY);
    for(int i = 0;i<5;i++){
        for(int j = 0;j<5;j++){
            write(fd,&map[i][j],sizeof(char));
        }
    }
    //SEND DATA
    write(fd,&server.players[index].x,sizeof(int));
    write(fd,&server.players[index].y,sizeof(int));
    write(fd,&server.players[index].deaths,sizeof(int));
    write(fd,&server.players[index].money_carried,sizeof(int));
    write(fd,&server.players[index].money_brought,sizeof(int));
    close(fd);
}



void move_player(int* move, int index, char character){
    switch(*move){
        case KEY_UP:
            if(mvwinch(server.map,server.players[index].y-1,server.players[index].x) == (char)219){
                break;
            }
            mvwaddch(server.map,server.players[index].y,server.players[index].x,' ');
            mvwaddch(server.map,server.players[index].y-1,server.players[index].x,character);
            server.players[index].y--;
            break;
        case KEY_DOWN:
            if(mvwinch(server.map,server.players[index].y+1,server.players[index].x) == (char)219){
                break;
            }
            mvwaddch(server.map,server.players[index].y,server.players[index].x,' ');
            mvwaddch(server.map,server.players[index].y+1,server.players[index].x,character);
            server.players[index].y++;
            break;
        case KEY_LEFT:
            if(mvwinch(server.map,server.players[index].y,server.players[index].x-1) == (char)219){
                break;
            }
            mvwaddch(server.map,server.players[index].y,server.players[index].x,' ');
            mvwaddch(server.map,server.players[index].y,server.players[index].x-1,character);
            server.players[index].x--;
            break;
        case KEY_RIGHT:
            if(mvwinch(server.map,server.players[index].y,server.players[index].x+1) == (char)219){
                break;
            }
            mvwaddch(server.map,server.players[index].y,server.players[index].x,' ');
            mvwaddch(server.map,server.players[index].y,server.players[index].x+1,character);
            server.players[index].x++;
            break;
        default:
            break;
    }
}