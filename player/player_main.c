#include "player_libs.h"


int main(){

    int fd = open("../server/connection_fifo",O_RDONLY);
    if(fd == -1){
        return 1;
    }
    int PID;
    char moves_fifo[29];
    char info_fifo[29];
    read(fd,&PID,sizeof(int));
    read(fd,moves_fifo,sizeof(char)*28);
    read(fd,info_fifo,sizeof(char)*28);
    moves_fifo[28] = '\0';
    info_fifo[28] = '\0';
    close(fd);
    
    printf("%d\n",PID);
    printf("%s\n%s\n",moves_fifo,info_fifo);

    int fd2 = open(moves_fifo,O_WRONLY);
    if(fd2 == -1){
        printf("CHUJ\n");
        return 1;
    }
    int pid = (int)getpid();
    write(fd2,&pid,sizeof(int));
    int m = 'w';
    while(1){
        write(fd2,&m,sizeof(int));
        sleep(1);
    }
    close(fd);

    
    return 0;
}