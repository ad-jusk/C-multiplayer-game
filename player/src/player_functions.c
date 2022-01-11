#include "player_libs.h"


void connect_to_server(){
    int fd = open("../server/connection_fifo",O_RDONLY);
    if(fd == -1){
        return;
    }
    read(fd,&player.server_PID,sizeof(int));
    read(fd,&player.index,sizeof(int));
    read(fd,player.fifo_to_write,sizeof(char)*28);
    read(fd,player.fifo_to_read,sizeof(char)*28);
    player.fifo_to_read[28] = '\0';
    player.fifo_to_write[28] = '\0';
    close(fd);

    player.round = 0;
    player.is_camp_discovered = 0;
}

void get_map_data(){
    int fd = open(player.fifo_to_read,O_RDONLY);
    char temp;
    for(int i = 0;i<5;i++){
        for(int j = 0;j<5;j++){
            read(fd,&temp,sizeof(char));
            mvwaddch(player.map,i,j,temp);
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
    wrefresh(player.map);
    mvwprintw(player.stats,2,2,"Campsite X/Y: unknown");
    mvwprintw(player.stats,3,2,"Round: %d",player.round);
    mvwprintw(player.stats,5,1,"Player:");
    mvwprintw(player.stats,6,2,"Number: %d",player.index);
    mvwprintw(player.stats,7,2,"Type: HUMAN");
    mvwprintw(player.stats,8,2,"Curr Y/X: %02d/%02d",player.y,player.x);
    mvwprintw(player.stats,9,2,"Deaths: %d",player.deaths);
    mvwprintw(player.stats,11,2,"Coins found: %d",player.money_carried);
    mvwprintw(player.stats,12,2,"Coins brought: %d",player.money_brought);
    wrefresh(player.stats);
    
}
