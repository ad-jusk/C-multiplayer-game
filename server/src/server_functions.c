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
    remove("connection_fifo");
    remove("info_for_pla1_fifo");
    remove("player1_moves_fifo");
    remove("info_for_pla2_fifo");
    remove("player2_moves_fifo");
    remove("info_for_pla3_fifo");
    remove("player3_moves_fifo");
    remove("info_for_pla4_fifo");
    remove("player4_moves_fifo");
}

int init_semaphores(){
    if(sem_init(&player1_is_in,0,0) == -1){
        return 1;
    }
    if(sem_init(&player2_is_in,0,0) == -1){
        sem_destroy(&player1_is_in);
        return 1;
    }
    if(sem_init(&player3_is_in,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        return 1;
    }
    if(sem_init(&player4_is_in,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        return 1;
    }
    if(sem_init(&round_start,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        sem_destroy(&player4_is_in);
        return 1;
    }
    if(sem_init(&round_end,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        sem_destroy(&player4_is_in);
        sem_destroy(&round_start);
        return 1;
    }
    if(sem_init(&beast_finished,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        sem_destroy(&player4_is_in);
        sem_destroy(&round_start);
        sem_destroy(&round_end);
        return 1;
    }
    if(sem_init(&player1_finished,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        sem_destroy(&player4_is_in);
        sem_destroy(&round_start);
        sem_destroy(&round_end);
        sem_destroy(&beast_finished);
        return 1;
    }
    if(sem_init(&player2_finished,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        sem_destroy(&player4_is_in);
        sem_destroy(&round_start);
        sem_destroy(&round_end);
        sem_destroy(&beast_finished);
        sem_destroy(&player1_finished);
        return 1;
    }
    if(sem_init(&player3_finished,0,0) == -1){
        sem_destroy(&player1_is_in);
        sem_destroy(&player2_is_in);
        sem_destroy(&player3_is_in);
        sem_destroy(&player4_is_in);
        sem_destroy(&round_start);
        sem_destroy(&round_end);
        sem_destroy(&beast_finished);
        sem_destroy(&player1_finished);
        sem_destroy(&player2_finished);
        return 1;
    }
    if(sem_init(&player4_finished,0,0) == -1){
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
        return 1; 
    }
    return 0;
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
    if(server.players[1].is_in){
        //WAIT FOR PLAYER 2 TO FINISH
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
    collect_money();
    deposit_money();
    beast_kill_player();
    check_if_collission();

    //AFTER SERVER'S APPROVAL DISPLAY PLAYERS AND BEASTS
    display_beasts();
    display_players();

    for(int i = 0;i<server.num_of_beasts + server.num_of_players;i++){
        sem_post(&round_end);
    }
}


//PLAYERS FUNCTIONS

void spawn_players_main(){
    if(server.players[0].wait_spawn){
        spawn_player('1');
        server.players[0].wait_spawn = 0;
        sem_post(&player1_is_in);
    }
    if(server.players[1].wait_spawn){
        spawn_player('2');
        server.players[1].wait_spawn = 0;
        sem_post(&player2_is_in);
    }
    if(server.players[2].wait_spawn){
        spawn_player('3');
        server.players[2].wait_spawn = 0;
        sem_post(&player3_is_in);
    }
    if(server.players[3].wait_spawn){
        spawn_player('4');
        server.players[3].wait_spawn = 0;
        sem_post(&player4_is_in);
    }
}

void spawn_player(char character){
    int x, y;
    do{
        x = rand() % (MAP_WIDTH - 2);
        y = rand() % MAP_HEIGHT;
    }while(mvwinch(server.map,y,x) == (char)219 || (x == CAMP_X && y == CAMP_Y));
    int temp = server.num_of_players;
    server.num_of_players = character - '0' - 1;
    server.players[server.num_of_players].x = x;
    server.players[server.num_of_players].y = y;
    server.players[server.num_of_players].spawn_x = x;
    server.players[server.num_of_players].spawn_y = y;
    server.players[server.num_of_players].is_in = 1;
    server.players[server.num_of_players].money_brought = 0;
    server.players[server.num_of_players].money_carried = 0;
    server.players[server.num_of_players].deaths = 0;
    server.players[server.num_of_players].char_to_display = ' ';
    server.players[server.num_of_players].on_bush = 0;
    strcpy(server.players[server.num_of_players].type,"HUMAN");
    server.num_of_players = temp;
    server.num_of_players++;
    mvwaddch(server.map,y,x,character | COLOR_PAIR(PLAYER_PAIR));
}

void remove_players_main(){
    if(server.players[0].want_to_quit){
        clear_player_stats(0);
        server.players[0].want_to_quit = 0;
    }
    if(server.players[1].want_to_quit){
        clear_player_stats(1);
        server.players[1].want_to_quit = 0;
    }
    if(server.players[2].want_to_quit){
        clear_player_stats(2);
        server.players[2].want_to_quit = 0;
    }
    if(server.players[3].want_to_quit){
        clear_player_stats(3);
        server.players[3].want_to_quit = 0;
    }
}

void clear_player_stats(int index){
    struct player_t* player = &server.players[index];
    mvwaddch(server.map,player->y,player->x,' ');
    player->deaths = 0;
    player->is_in = 0;
    server.num_of_players--;
    player->money_brought = 0;
    player->money_carried = 0;
    player->PID = 0;
    player->x = 0;
    player->y = 0;
    player->spawn_x = 0;
    player->spawn_y = 0;
    strcpy(player->type,"-----");
}

void beast_kill_player(){
    for(int i = 0;i<server.num_of_beasts;i++){
        for(int j = 0;j<server.num_of_players;j++){
            if(server.beasts[i].x == server.players[j].x && server.beasts[i].y == server.players[j].y){
                set_death_point(j);
                server.players[j].x = server.players[j].spawn_x;
                server.players[j].y = server.players[j].spawn_y;
                server.players[j].money_carried = 0;
                server.players[j].deaths++;
                server.beasts[i].char_to_display = 'D';
            }
        }
    }
}

void set_player_fifos(){
    strcpy(server.players[0].fifo_to_read,"player1_moves_fifo");
    strcpy(server.players[0].fifo_to_write,"info_for_pla1_fifo");
    strcpy(server.players[1].fifo_to_read,"player2_moves_fifo");
    strcpy(server.players[1].fifo_to_write,"info_for_pla2_fifo");
    strcpy(server.players[2].fifo_to_read,"player3_moves_fifo");
    strcpy(server.players[2].fifo_to_write,"info_for_pla3_fifo");
    strcpy(server.players[3].fifo_to_read,"player4_moves_fifo");
    strcpy(server.players[3].fifo_to_write,"info_for_pla4_fifo");
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
    int index;
    int error = 1;

    make_fifos();
    set_player_fifos();
    while(1){
        int fd = open("connection_fifo", O_WRONLY);
        index = server.num_of_players + 1;
        if(server.players[0].is_in == 0){
            write(fd,&server.PID,sizeof(int));
            write(fd,&index,sizeof(int));
            write(fd,player1_moves_fifo_name,strlen(player1_moves_fifo_name));
            write(fd,info_for_player1_fifo_name,strlen(info_for_player1_fifo_name));
            server.players[0].wait_spawn = 1;
        }
        else if(server.players[1].is_in == 0){
            write(fd,&server.PID,sizeof(int));
            write(fd,&index,sizeof(int));
            write(fd,player2_moves_fifo_name,strlen(player2_moves_fifo_name));
            write(fd,info_for_player2_fifo_name,strlen(info_for_player2_fifo_name));
            server.players[1].wait_spawn = 1;
        }
        else if(server.players[2].is_in == 0){
            write(fd,&server.PID,sizeof(int));
            write(fd,&index,sizeof(int));
            write(fd,player3_moves_fifo_name,strlen(player3_moves_fifo_name));
            write(fd,info_for_player3_fifo_name,strlen(info_for_player3_fifo_name));
            server.players[2].wait_spawn = 1;
        }
        else if(server.players[3].is_in == 0){
            write(fd,&server.PID,sizeof(int));
            write(fd,&index,sizeof(int));
            write(fd,player4_moves_fifo_name,strlen(player4_moves_fifo_name));
            write(fd,info_for_player4_fifo_name,strlen(info_for_player4_fifo_name));
            server.players[3].wait_spawn = 1;
        }
        else{
            write(fd,&error,sizeof(int));
        }
        close(fd);
        sleep(1);
    }

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
    int flag = 0;
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
        //MOVE PLAYER IF PROCESS IS NOT TERMINATED
        if(kill((pid_t)server.players[index].PID,0) != -1){
            read_player_move(index,&move);
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player1_finished);
                sem_wait(&round_end);
                sem_wait(&player1_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);
                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player1_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                continue;
                //<-- END OF ROUND -->
            }
            if(server.players[index].on_bush == 1){
                move = 'b';
                flag = 1;
            }
            move_player(&move,index);
            check_if_bush(index);
            if(flag == 1 && move != -1){
                flag = 0;
                server.players[index].on_bush = 0;
                server.players[index].char_to_display = '#';
            }
            sem_post(&player1_finished);
            sem_wait(&round_end);

            //SEND MAP
            send_map_data_to_player(index);
        }
        else{
            if(errno == ESRCH){
                move = 'q';
            }
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player1_finished);
                sem_wait(&round_end);
                sem_wait(&player1_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player1_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                //<-- END OF ROUND -->
            }
        }
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
    int flag = 0;
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
        //MOVE PLAYER IF PROCESS IS NOT TERMINATED
        if(kill((pid_t)server.players[index].PID,0) != -1){
            read_player_move(index,&move);
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player2_finished);
                sem_wait(&round_end);
                sem_wait(&player2_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player2_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                continue;
                //<-- END OF ROUND -->
            }
            if(server.players[index].on_bush == 1){
                move = 'b';
                flag = 1;
            }
            move_player(&move,index);
            check_if_bush(index);
            if(flag == 1 && move != -1){
                flag = 0;
                server.players[index].on_bush = 0;
                server.players[index].char_to_display = '#';
            }
            sem_post(&player2_finished);
            sem_wait(&round_end);

            //SEND MAP
            send_map_data_to_player(index);
        }
        else{
            if(errno == ESRCH){
                move = 'q';
            }
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player2_finished);
                sem_wait(&round_end);
                sem_wait(&player2_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player2_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                //<-- END OF ROUND -->
            }
        }
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
    int flag = 0;
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
        //MOVE PLAYER IF PROCESS IS NOT TERMINATED
        if(kill((pid_t)server.players[index].PID,0) != -1){
            read_player_move(index,&move);
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player3_finished);
                sem_wait(&round_end);
                sem_wait(&player3_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player3_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                continue;
                //<-- END OF ROUND -->
            }
            if(server.players[index].on_bush == 1){
                move = 'b';
                flag = 1;
            }
            move_player(&move,index);
            check_if_bush(index);
            if(flag == 1 && move != -1){
                flag = 0;
                server.players[index].on_bush = 0;
                server.players[index].char_to_display = '#';
            }
            sem_post(&player3_finished);
            sem_wait(&round_end);

            //SEND MAP
            send_map_data_to_player(index);
        }
        else{
            if(errno == ESRCH){
                move = 'q';
            }
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player3_finished);
                sem_wait(&round_end);
                sem_wait(&player3_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player3_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                //<-- END OF ROUND -->
            }
        }
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
    int flag = 0;
    //SEND SPAWN MAP
    send_map_data_to_player(index);
    while(1){
        sem_wait(&round_start);
        //MOVE PLAYER IF PROCESS IS NOT TERMINATED
        if(kill((pid_t)server.players[index].PID,0) != -1){
            read_player_move(index,&move);
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player4_finished);
                sem_wait(&round_end);
                sem_wait(&player4_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player4_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                continue;
                //<-- END OF ROUND -->
            }
            if(server.players[index].on_bush == 1){
                move = 'b';
                flag = 1;
            }
            move_player(&move,index);
            check_if_bush(index);
            if(flag == 1 && move != -1){
                flag = 0;
                server.players[index].on_bush = 0;
                server.players[index].char_to_display = '#';
            }
            sem_post(&player4_finished);
            sem_wait(&round_end);

            //SEND MAP
            send_map_data_to_player(index);
        }
        else{
            if(errno == ESRCH){
                move = 'q';
            }
            if(move == 'q' || move == 'Q'){
                server.players[index].want_to_quit = 1;
                sem_post(&player4_finished);
                sem_wait(&round_end);
                sem_wait(&player4_is_in);
                int fd2 = open(server.players[index].fifo_to_read,O_RDONLY);
                read(fd2,&server.players[index].PID,sizeof(int));
                close(fd2);

                // <-- ROUND AFTER QUIT -->
                sem_wait(&round_start);
                send_map_data_to_player(index);
                read_player_move(index,&move);
                move_player(&move,index);
                if(server.players[index].on_bush == 1){
                    move = 'b';
                    flag = 1;
                }
                check_if_bush(index);
                if(flag == 1 && move != -1){
                    flag = 0;
                    server.players[index].on_bush = 0;
                    server.players[index].char_to_display = '#';
                }
                sem_post(&player4_finished);
                sem_wait(&round_end);
                send_map_data_to_player(index);
                //<-- END OF ROUND -->
            }
        }
    }
    return NULL;

}

int read_player_move(int index, int* move){
    int fd = open(server.players[index].fifo_to_read,O_RDONLY);
    if(read(fd,move,sizeof(int)) == -1){
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

int send_map_data_to_player(int index){
    
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
            if(write(fd,&map[i][j],sizeof(char)) == -1){
                close(fd);
                return 1;
            }
        }
    }
    //SEND DATA
    if(write(fd,&server.players[index].x,sizeof(int)) == -1){
        close(fd);
        return 1;
    }
    if(write(fd,&server.players[index].y,sizeof(int)) == -1){
        close(fd);
        return 1;
    }
    
    if(write(fd,&server.players[index].deaths,sizeof(int)) == -1){
        close(fd);
        return 1;
    }
    if(write(fd,&server.players[index].money_carried,sizeof(int)) == -1){
        close(fd);
        return 1;
    }
    if(write(fd,&server.players[index].money_brought,sizeof(int)) == -1){
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}



void move_player(int* move, int index){
    if(server.players[index].x == CAMP_X && server.players[index].y == CAMP_Y){
        server.players[index].char_to_display = 'A';
    }
    else if(server.players[index].char_to_display == ((index + '0') | COLOR_PAIR(PLAYER_PAIR)) || (server.players[index].x == server.players[index].spawn_x &&
    server.players[index].y == server.players[index].spawn_y)){
        server.players[index].char_to_display = ' ';
    }
    switch(*move){
        case KEY_UP:
            if(mvwinch(server.map,server.players[index].y-1,server.players[index].x) == (char)219){
                break;
            }
            if(server.players[index].char_to_display == 'A')
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display | COLOR_PAIR(CAMP_PAIR));
            else
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display);
            server.players[index].y--;
            break;
        case KEY_DOWN:
            if(mvwinch(server.map,server.players[index].y+1,server.players[index].x) == (char)219){
                break;
            }
            if(server.players[index].char_to_display == 'A')
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display | COLOR_PAIR(CAMP_PAIR));
            else
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display);
            server.players[index].y++;
            break;
        case KEY_LEFT:
            if(mvwinch(server.map,server.players[index].y,server.players[index].x-1) == (char)219){
                break;
            }
            if(server.players[index].char_to_display == 'A')
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display | COLOR_PAIR(CAMP_PAIR));
            else
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display);
            server.players[index].x--;
            break;
        case KEY_RIGHT:
            if(mvwinch(server.map,server.players[index].y,server.players[index].x+1) == (char)219){
                break;
            }
            if(server.players[index].char_to_display == 'A')
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display | COLOR_PAIR(CAMP_PAIR));
            else
                mvwaddch(server.map,server.players[index].y,server.players[index].x,server.players[index].char_to_display);
            server.players[index].x++;
            break;
        default:
            break;
    }
    if(server.players[index].char_to_display == '#' && *move != -1){
        server.players[index].char_to_display = ' ';
    }
    else if(server.players[index].char_to_display == 'A' && *move != -1){
        server.players[index].char_to_display = ' ';
    }
    else if(isdigit(server.players[index].char_to_display)){
        server.players[index].char_to_display = ' ';
    }
    else if(server.players[index].char_to_display != '#'){
        server.players[index].char_to_display = ' ';
    }
}

void make_fifos(){
    //CONNECCTION FIFO FOR ALL PLAYERS
    if(mkfifo("connection_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //PLAYER'S 1 ACTIONS SENT TO SERVER
    if(mkfifo("player1_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 1 
    if(mkfifo("info_for_pla1_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //PLAYER'S 2 ACTIONS SENT TO SERVER
    if(mkfifo("player2_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 2 
    if(mkfifo("info_for_pla2_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //PLAYER'S 3 ACTIONS SENT TO SERVER
    if(mkfifo("player3_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 3 
    if(mkfifo("info_for_pla3_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //PLAYER'S 4 ACTIONS SENT TO SERVER
    if(mkfifo("player4_moves_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
    //SERVER'S INFO SENT TO PLAYER 4 
    if(mkfifo("info_for_pla4_fifo",0777) == -1){
        if(errno != EEXIST){
            server_shut_down();
            exit(1);
        }
    }
}

void player_correct(int index){
    struct player_t* player = &server.players[index];
    char x = index + '0';
    char temp = mvwinch(server.map,player->y-1,player->x);
    if(temp == (x | COLOR_PAIR(PLAYER_PAIR))){
        mvwaddch(server.map,player->y-1,player->x,' ');
    }
    temp = mvwinch(server.map,player->y+1,player->x);
    if(temp == (x | COLOR_PAIR(PLAYER_PAIR))){
        mvwaddch(server.map,player->y+1,player->x,' ');
    }
    temp = mvwinch(server.map,player->y,player->x-1);
    if(temp == (x | COLOR_PAIR(PLAYER_PAIR))){
        mvwaddch(server.map,player->y,player->x-1,' ');
    }
    temp = mvwinch(server.map,player->y,player->x+1);
    if(temp == (x | COLOR_PAIR(PLAYER_PAIR))){
        mvwaddch(server.map,player->y,player->x+1,' ');
    }
}