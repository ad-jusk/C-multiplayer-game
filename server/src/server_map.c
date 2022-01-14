#include "server_map.h"

int load_map(){
    FILE* f = fopen("map.txt","r");
    while(1){
        char a = (char)fgetc(f);
        if(feof(f))
            break;
        if(a == 'b')
            a = (char)219;
        waddch(server.map,a);
    }
    fclose(f);
    return 0;
}

int start_server(){
    server.num_of_beasts = 0;
    server.num_of_players = 0;
    server.PID = (int)getpid();
    server.round = 0;
    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        server.players[i].deaths = 0;
        server.players[i].x = 0;
        server.players[i].y = 0;
        server.players[i].PID = 0;
        server.players[i].is_in = 0;
        server.players[i].money_brought = 0;
        server.players[i].money_carried = 0;
        strcpy(server.players[i].type,"-----");
    }
    if(load_map()){
        return 1;
    }
    set_current_server_status_and_map();
    return 0;
}

void set_current_server_status_and_map(){
    mvwprintw(server.status,1,1,"Server's PID: %d",server.PID);
    mvwprintw(server.status,2,1,"Camp pos: %d/%d",CAMP_X,CAMP_Y);
    mvwprintw(server.status,3,1,"Round: %d",server.round);

    mvwprintw(server.status,5,1,"Players:");
    mvwprintw(server.status,6,4,"PID:");
    mvwprintw(server.status,7,3,"TYPE:");
    mvwprintw(server.status,8,4,"POS:");
    mvwprintw(server.status,9,1,"DEATHS:");
    mvwprintw(server.status,11,2,"COINS:");
    mvwprintw(server.status,12,3,"carried:");
    mvwprintw(server.status,13,3,"brought:");

    mvwprintw(server.status,5,12,"Player1");
    mvwprintw(server.status,5,21,"Player2");
    mvwprintw(server.status,5,30,"Player3");
    mvwprintw(server.status,5,39,"Player4");

    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        mvwprintw(server.status,6,12+9*i,"%7d",server.players[i].PID);
        mvwprintw(server.status,7,12+9*i,"%7s",server.players[i].type);
        mvwprintw(server.status,8,12+9*i,"  %02d/%02d",server.players[i].y,server.players[i].x);
        mvwprintw(server.status,9,12+9*i,"%7d",server.players[i].deaths);
        if(server.players[i].x == CAMP_X && server.players[i].y == CAMP_Y){
            wmove(server.status,12,0);
            wclrtoeol(server.status);
            box(server.status,0,0);
        }
        mvwprintw(server.status,12,12+9*i,"%7d",server.players[i].money_carried);
        mvwprintw(server.status,13,12+9*i,"%7d",server.players[i].money_brought);

    }
    wrefresh(server.map);
    wrefresh(server.status);
}


void display_players(){
    for(int i = 0;i<server.num_of_players;i++){
        mvwaddch(server.map,server.players[i].y,server.players[i].x,(i+1) + '0');
    }
}

void display_beasts(){
    for(int i = 0;i<server.num_of_beasts;i++){
        mvwaddch(server.map,server.beasts[i].y,server.beasts[i].x,'*');
    }
}

void set_collectibles(int num_of_coins, int num_of_treasures, int num_of_large_treasures){
    int x, y;
    //SET COINS
    for(int i = 0;i<num_of_coins;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'c');
    }
    //SET TREASURES
    for(int i = 0;i<num_of_treasures;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'t');
    }
    //SET LARGE TREASURES
    for(int i = 0;i<num_of_large_treasures;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'T');
    }
}

void collect_money(){
    char temp;
    for(int i = 0;i<server.num_of_players;i++){
        temp = mvwinch(server.map,server.players[i].y,server.players[i].x);
        switch(temp){
            case 'c':
                server.players[i].money_carried++;
                break;
            case 't':
                server.players[i].money_carried += 10;
                break;
            case 'T':
                server.players[i].money_carried += 50;
                break;
            default:
            break;
        }
    }
}

void deposit_money(){
    for(int i = 0;i<server.num_of_players;i++){
        if(mvwinch(server.map,server.players[i].y,server.players[i].x) == 'A'){
            server.players[i].money_brought += server.players[i].money_carried;
            server.players[i].money_carried = 0;
        }
    }
}

void set_camp(){
    mvwaddch(server.map,CAMP_Y,CAMP_X,'A');
}

void set_bushes(){
    int x,y;
    for(int i = 0;i<BUSHES_AMOUNT;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'#');
    }
}

void check_if_bush(int index){
    if(mvwinch(server.map,server.players[index].y,server.players[index].x) == '#'){
         server.players[index].on_bush = 1;
    }
}