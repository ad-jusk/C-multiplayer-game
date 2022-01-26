#include "player_libs.h"


int connect_to_server(){
    int fd = open("../server/connection_fifo",O_RDONLY);
    if(fd == -1){
        if(errno != EEXIST)
            return 1;
    }
    read(fd,&player.server_PID,sizeof(int));
    if(player.server_PID == 1){
        return 2;
    }
    read(fd,&player.index,sizeof(int));
    read(fd,player.fifo_to_write,sizeof(char)*28);
    read(fd,player.fifo_to_read,sizeof(char)*28);
    player.fifo_to_read[28] = '\0';
    player.fifo_to_write[28] = '\0';
    close(fd);

    player.round = 0;
    player.is_camp_discovered = 0;
    return 0;
}

void get_map_data(){
    int fd = open(player.fifo_to_read,O_RDONLY);
    char temp;
    for(int i = 0;i<5;i++){
        for(int j = 0;j<5;j++){
            read(fd,&temp,sizeof(char));
            if(temp == 'A'){
                mvwaddch(player.map,i+2,j+2,temp | COLOR_PAIR(CAMP_PAIR));
                player.is_camp_discovered = 1;  
            }
            else if(isalpha(temp)){
                mvwaddch(player.map,i+2,j+2,temp | COLOR_PAIR(TREASURE_PAIR)); 
            }
            else if(isdigit(temp)){
                mvwaddch(player.map,i+2,j+2,temp | COLOR_PAIR(PLAYER_PAIR));
            }
            else if(temp == '*'){
                mvwaddch(player.map,i+2,j+2,temp | COLOR_PAIR(BEAST_PAIR)); 
            }
            else{
                mvwaddch(player.map,i+2,j+2,temp); 
            }
        }
    }
    read(fd,&player.x,sizeof(int));
    read(fd,&player.y,sizeof(int));
    read(fd,&player.deaths,sizeof(int));
    read(fd,&player.money_carried,sizeof(int));
    read(fd,&player.money_brought,sizeof(int));
    close(fd);
}

void send_move(int* move){
    int fd = open(player.fifo_to_write,O_WRONLY);
    write(fd,move,sizeof(int));
    close(fd);
}

void send_PID(){
    int PID = (int)getpid();
    int fd = open(player.fifo_to_write,O_WRONLY);
    write(fd,&PID,sizeof(int));
    close(fd);
}

void set_current_map_data(){
    if(player.is_camp_discovered == 0){
        mvwprintw(player.stats,2,2,"Campsite Y/X: unknown");
    }
    else{
        mvwprintw(player.stats,2,2,"Campsite Y/X: %02d/%02d   ",CAMP_Y,CAMP_X);
    }
    mvwprintw(player.stats,3,2,"Round: %d",player.round);
    mvwprintw(player.stats,5,1,"Player:");
    mvwprintw(player.stats,6,2,"Number: %d",player.index);
    mvwprintw(player.stats,7,2,"Type: HUMAN");
    mvwprintw(player.stats,8,2,"Curr Y/X: %02d/%02d",player.y,player.x);
    mvwprintw(player.stats,9,2,"Deaths: %d",player.deaths);
    wmove(player.stats,11,0);
    wclrtoeol(player.stats);
    box(player.stats,0,0);
    mvwprintw(player.stats,11,2,"Coins found: %d",player.money_carried);
    mvwprintw(player.stats,12,2,"Coins brought: %d",player.money_brought);
    wrefresh(player.map);
    wrefresh(player.stats);
    
}

void init_colors(){
    init_pair(BEAST_PAIR,COLOR_RED,-1);
    init_pair(PLAYER_PAIR,COLOR_MAGENTA,-1);
    init_pair(TREASURE_PAIR,COLOR_YELLOW,-1);
    init_pair(CAMP_PAIR,COLOR_CYAN,-1);
}
