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
    server.camp_x = 26;
    server.camp_y = 14;
    server.PID = (int)getpid();
    server.round = 0;
    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        server.players[i].deaths = 0;
        server.players[i].x = 0;
        server.players[i].y = 0;
        server.players[i].PID = 0;
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
    mvwprintw(server.status,2,1,"Camp pos: %d/%d",server.camp_x,server.camp_y);
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
        mvwprintw(server.status,12,12+9*i,"%7d",server.players[i].money_carried);
        mvwprintw(server.status,13,12+9*i,"%7d",server.players[i].money_brought);

    }
    wrefresh(server.map);
    wrefresh(server.status);
}