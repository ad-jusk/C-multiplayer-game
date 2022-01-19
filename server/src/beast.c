#include "beast.h"


void beast_init(){
    int x, y;
    do{
        x = rand() % (MAP_WIDTH - 2);
        y = rand() % MAP_HEIGHT;

    }while(mvwinch(server.map,y,x) == (char)219);
    server.beasts[server.num_of_beasts].x = x;
    server.beasts[server.num_of_beasts].y = y;
    server.beasts[server.num_of_beasts].char_to_display = ' ';
    server.num_of_beasts++;
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
    
    if(search_for_player(i,&direction)){
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
    }

    switch(direction){
        //MOVE DOWN
        case 0:
            if(isalpha(server.beasts[i].char_to_display)){
                if(server.beasts[i].char_to_display == 'A')
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(CAMP_PAIR));
                else
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(TREASURE_PAIR));
            }
            else{
                mvwaddch(server.map,y,x,server.beasts[i].char_to_display);
            }
            server.beasts[i].y++;
            break;
        //MOVE UP
        case 1:
            if(isalpha(server.beasts[i].char_to_display)){
                if(server.beasts[i].char_to_display == 'A')
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(CAMP_PAIR));
                else
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(TREASURE_PAIR));
            }
            else{
                mvwaddch(server.map,y,x,server.beasts[i].char_to_display);
            }
            server.beasts[i].y--;
            break;
        case 2:
         //MOVE RIGHT
            if(isalpha(server.beasts[i].char_to_display)){
                if(server.beasts[i].char_to_display == 'A')
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(CAMP_PAIR));
                else
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(TREASURE_PAIR));
            }
            else{
                mvwaddch(server.map,y,x,server.beasts[i].char_to_display);
            }
            server.beasts[i].x++;
            break;
        case 3:
        //MOVE LEFT
            if(isalpha(server.beasts[i].char_to_display)){
                if(server.beasts[i].char_to_display == 'A')
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(CAMP_PAIR));
                else
                    mvwaddch(server.map,y,x,server.beasts[i].char_to_display | COLOR_PAIR(TREASURE_PAIR));
            }
            else{
                mvwaddch(server.map,y,x,server.beasts[i].char_to_display);
            }
            server.beasts[i].x--;
            break;
    }
    char temp = mvwinch(server.map,server.beasts[i].y,server.beasts[i].x);
    if(isalpha(temp) || temp == '#'){
        server.beasts[i].char_to_display = temp;
    }
    else if(isdigit(temp) && temp != 'D'){
        server.beasts[i].char_to_display = 'D';
    }
    else{
        server.beasts[i].char_to_display = ' ';
    }
}

int search_for_player(int index, int* direction){
    int x = server.beasts[index].x;
    int y = server.beasts[index].y;
    int start_x = x - 2;
    int start_y = y - 2;
    int end_x = x + 2;
    int end_y = y + 2;
    if(start_x < 1)
        start_x = 1;
    if(start_y < 1)
        start_y = 1;
    if(end_x > MAP_WIDTH - 3)
        end_x = MAP_WIDTH - 3;
    if(end_y > MAP_HEIGHT - 2)
        end_y = MAP_HEIGHT - 2;
    for(int i = start_y;i<=end_y;i++){
        for(int j = start_x;j<=end_x;j++){
            if(isdigit(mvwinch(server.map,i,j))){
                if(y == i && j < x && mvwinch(server.map,i,j+1) != (char)219){
                    //MOVE LEFT
                    *direction = 3;
                    return 0;
                }
                if(y == i && j > x && mvwinch(server.map,i,j-1) != (char)219){
                    //MOVE RIGHT
                    *direction = 2;
                    return 0;
                }
                if(x == j && i > y && mvwinch(server.map,i-1,j) != (char)219){
                    //MOVE UP
                    *direction = 0;
                    return 0;
                }
                if(x == j && i < y && mvwinch(server.map,i+1,j) != (char)219){
                    //MOVE DOWN
                    *direction = 1;
                    return 0;
                }

            }
        }
    }
    return 1;
}