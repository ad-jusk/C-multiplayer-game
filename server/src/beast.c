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
    while(1){
        sem_wait(&round_start);      // WAIT FOR ROUND TO START
        
        make_a_move(i);

        sem_post(&beast_finished);   // POST THAT BEAST HAS FINISHED ITS MOVE
        sem_wait(&round_end);        // WAIT FOR ROUND TO END
    }
}

void make_a_move(int i){
    int x = server.beasts[i].x;
    int y = server.beasts[i].y;
    char flag;
    int direction;

    do{
        direction = rand() % 4;
        if(direction == 0){
            if(mvwinch(server.map,y+1,x) == (char)219){
                flag = 1;
            }
            else{
                flag = 0;
            }
        }
        else if(direction == 1){
            if(mvwinch(server.map,y-1,x) == (char)219){
                flag = 1;            
            }
            else{
                flag = 0;
            }
        }
        else if(direction == 2){
            if(mvwinch(server.map,y,x+1) == (char)219){
                flag = 1;            
            }
            else{
                flag = 0;
            }
        }
        else if(direction == 3){
            if(mvwinch(server.map,y,x-1) == (char)219){
                flag = 1;            
            }
            else{
                flag = 0;
            }
        }

    }while(flag);
    
    
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