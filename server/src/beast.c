#include "beast.h"


void beast_init(){
    int x, y;
    do{
        x = rand() % (MAP_WIDTH - 2);
        y = rand() % MAP_HEIGHT;

    }while(mvwinch(server.map,y,x) == (char)219);
    server.beasts[server.num_of_beasts].x = x;
    server.beasts[server.num_of_beasts].y = y;
    server.num_of_beasts++;
    mvwaddch(server.map,y,x,'*');
}

void* beast_move(void* index){
    int i = *(int*)index;
    beast_init();
    while(1){
        make_a_move(i);
        sleep(1);
    }
}

void make_a_move(int i){
    int x = server.beasts[i].x;
    int y = server.beasts[i].y;
    int direction;
    if(mvwinch(server.map,y+1,x) == ' '){
        direction = 0;
    }
    else if(mvwinch(server.map,y-1,x) == ' '){
        direction = 1;
    }
    else if(mvwinch(server.map,y,x+1) == ' '){
        direction = 2;
    }
    else if(mvwinch(server.map,y,x-1) == ' '){
        direction = 3;
    }
    
    switch(direction){
        //MOVE DOWN
        case 0:
            mvwaddch(server.map,y,x,' ');
            mvwaddch(server.map,y+1,x,'*');
            server.beasts[i].y++;
            break;
        //MOVE UP
        case 1:
            mvwaddch(server.map,y,x,' ');
            mvwaddch(server.map,y-1,x,'*');
            server.beasts[i].y--;
            break;
        case 2:
         //MOVE RIGHT
            mvwaddch(server.map,y,x,' ');
            mvwaddch(server.map,y,x+1,'*');
            server.beasts[i].x++;
            break;
        case 3:
        //MOVE LEFT
            mvwaddch(server.map,y,x,' ');
            mvwaddch(server.map,y,x-1,'*');
            server.beasts[i].x--;
            break;
    }
}