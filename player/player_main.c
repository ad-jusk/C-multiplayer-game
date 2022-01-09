#include "player_libs.h"


int main(){

    int fd = open("../server/connection_fifo",O_RDONLY);
    if(fd == -1){
        return 1;
    }
    int PID;
    read(fd,&PID,sizeof(int));
    close(fd);
    

    int fd2 = open("../server/player1_moves_fifo",O_WRONLY);
    int move;
    do{
        move = getchar();
        write(fd2,&move,sizeof(int));
    }while(move != 'q');
    close(fd);
    return 0;
}