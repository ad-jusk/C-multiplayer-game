#include "server_libs.h"
#include "beast.h"
#include "server_map.h"
#include "server_functions.h"

struct server_t server;

int main(){

    initscr();
    curs_set(0);

    //SERVER MAP
    server.map = newwin(MAP_HEIGHT,MAP_WIDTH,0,0);
    //SERVER STATUS
    server.status = newwin(STATUS_BAR_HEIGHT,STATUS_BAR_WIDTH,0,MAP_WIDTH+2);
    box(server.status,0,0);

    if(start_server()){
        endwin();
        return 1;
    }

    getchar();
    endwin();

    return 0;
}