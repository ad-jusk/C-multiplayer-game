#include "player_libs.h"

struct player_t player;

int main(){

    //CONNECT
    if(connect_to_server()){
        printf("Server is full.\nTry again later\n");
        return 1;
    }

    initscr();
    halfdelay(10);
    noecho();
    curs_set(0);
    start_color();
    use_default_colors();
    
    player.map = newwin(5,5,2,2);
    player.stats = newwin(14,30,2,10);
    box(player.stats,0,0);
    keypad(player.map,1);
    refresh();

    //SET COLOR PAIRS
    init_colors();

    mvwprintw(player.stats,1,1,"Server's PID: %d",player.server_PID);
    //SEND PLAYER'S PID TO SERVER
    send_PID();

    //GET AND SET SPAWN MAP
    get_map_data();
    set_current_map_data();

    int move;

    do{
        move = wgetch(player.map);
        if(move == 'q' || move == 'Q'){
            send_move(&move);
            break;
        }
        send_move(&move);
        player.round++;
        get_map_data();
        set_current_map_data();
    }while(1);

    endwin();
    
    return 0;
}