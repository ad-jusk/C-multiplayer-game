#include "player_libs.h"

struct player_t player;

int main(){

    initscr();
    halfdelay(10);
    noecho();
    curs_set(0);
    
    player.map = newwin(5,5,2,1);
    player.stats = newwin(14,30,2,10);
    box(player.stats,0,0);
    keypad(player.map,1);
    refresh();

    int move;

    //CONNECT
    connect_to_server();
    mvwprintw(player.stats,1,1,"Server's PID: %d",player.server_PID);
    //SEND PLAYER'S PID TO SERVER
    send_PID();

    //GET AND SET SPAWN MAP
    get_map_data();
    set_current_map_data();

    do{
        move = wgetch(player.map);
        send_move(&move);
        player.round++;
        get_map_data();
        set_current_map_data();
    }while(move != 'q');

    endwin();
    
    return 0;
}