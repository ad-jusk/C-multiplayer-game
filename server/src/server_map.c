#include "server_map.h"

int load_map(){
    FILE* f = fopen("map.txt","r");
    if(!f){
        return 1;
    }
    while(1){
        char a = (char)fgetc(f);
        if(feof(f))
            break;
        if(a == 'b')
            a = (char)219;
        waddch(server.map,a);
    }
    fclose(f);
    return 0;
}

int start_server(){
    server.num_of_beasts = 0;
    server.num_of_players = 0;
    server.PID = (int)getpid();
    server.round = 0;
    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        server.players[i].deaths = 0;
        server.players[i].x = 0;
        server.players[i].y = 0;
        server.players[i].PID = 0;
        server.players[i].is_in = 0;
        server.players[i].money_brought = 0;
        server.players[i].money_carried = 0;
        server.players[i].wait_spawn = 0;
        server.players[i].want_to_quit = 0;
        strcpy(server.players[i].type,"-----");
        //DEATH POINTS
        server.death_points[i].x = 0;
        server.death_points[i].y = 0;
        server.death_points[i].money = 0;
    }
    if(load_map()){
        return 1;
    }
    set_current_server_status_and_map();
    return 0;
}

void init_colors(){
    init_pair(BEAST_PAIR,COLOR_RED,-1);
    init_pair(PLAYER_PAIR,COLOR_MAGENTA,-1);
    init_pair(TREASURE_PAIR,COLOR_YELLOW,-1);
    init_pair(CAMP_PAIR,COLOR_CYAN,-1);
}

void set_current_server_status_and_map(){
    mvwprintw(server.status,1,1,"Server's PID: %d",server.PID);
    mvwprintw(server.status,2,1,"Camp pos: %d/%d",CAMP_X,CAMP_Y);
    mvwprintw(server.status,3,1,"Round: %d",server.round);

    mvwprintw(server.status,5,1,"Players:");
    mvwprintw(server.status,6,4,"PID:");
    mvwprintw(server.status,7,3,"TYPE:");
    mvwprintw(server.status,8,4,"POS:");
    mvwprintw(server.status,9,1,"DEATHS:");
    mvwprintw(server.status,11,2,"COINS:");
    mvwprintw(server.status,12,3,"carried:");
    mvwprintw(server.status,13,3,"brought:");

    mvwprintw(server.status,5,12,"Player1");
    mvwprintw(server.status,5,21,"Player2");
    mvwprintw(server.status,5,30,"Player3");
    mvwprintw(server.status,5,39,"Player4");

    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        mvwprintw(server.status,6,12+9*i,"%7d",server.players[i].PID);
        mvwprintw(server.status,7,12+9*i,"%7s",server.players[i].type);
        mvwprintw(server.status,8,12+9*i,"  %02d/%02d",server.players[i].y,server.players[i].x);
        mvwprintw(server.status,9,12+9*i,"%7d",server.players[i].deaths);
        if(server.players[i].x == CAMP_X && server.players[i].y == CAMP_Y){
            wmove(server.status,12,12+9*i);
            wclrtoeol(server.status);
            mvwprintw(server.status,12,3,"carried:");
            box(server.status,0,0);
        }
        mvwprintw(server.status,12,12+9*i,"%7d",server.players[i].money_carried);
        mvwprintw(server.status,13,12+9*i,"%7d",server.players[i].money_brought);

    }
    wrefresh(server.map);
    wrefresh(server.status);
}

void fill_command_window(){
    mvwprintw(server.commands,1,1,"Available commands:");
    mvwprintw(server.commands,2,1,"b/B - add one beast");
    mvwprintw(server.commands,3,1,"c - add one coin");
    mvwprintw(server.commands,4,1,"t - add one treasure");
    mvwprintw(server.commands,5,1,"T - add one large treasure");
    mvwprintw(server.commands,6,1,"q/Q - shut down server");
    wrefresh(server.commands);
}

void player_correct(int index, char x){
    struct player_t* player = &server.players[index];
    char temp = mvwinch(server.map,player->y-1,player->x);
    if(temp == x){
        mvwaddch(server.map,player->y-1,player->x,' ');
    }
    temp = mvwinch(server.map,player->y+1,player->x);
    if(temp == x){
        mvwaddch(server.map,player->y+1,player->x,' ');
    }
    temp = mvwinch(server.map,player->y,player->x-1);
    if(temp == x){
        mvwaddch(server.map,player->y,player->x-1,' ');
    }
    temp = mvwinch(server.map,player->y,player->x+1);
    if(temp == x){
        mvwaddch(server.map,player->y,player->x+1,' ');
    }
}

void display_players(){
    char x;
    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        if(server.players[i].x != 0 && server.players[i].y != 0){
            mvwaddch(server.map,server.players[i].y,server.players[i].x,((i+1) + '0')  | COLOR_PAIR(PLAYER_PAIR));
            x = mvwinch(server.map,server.players[i].y,server.players[i].x);
            player_correct(i,x);
        }
    }
}

void beast_correct(int index, char x){
    struct beast_t* beast = &server.beasts[index];
    char temp = mvwinch(server.map,beast->y-1,beast->x);
    if(temp == x){
        mvwaddch(server.map,beast->y-1,beast->x,' ');
    }
    temp = mvwinch(server.map,beast->y+1,beast->x);
    if(temp == x){
        mvwaddch(server.map,beast->y+1,beast->x,' ');
    }
    temp = mvwinch(server.map,beast->y,beast->x-1);
    if(temp == x){
        mvwaddch(server.map,beast->y,beast->x-1,' ');
    }
    temp = mvwinch(server.map,beast->y,beast->x+1);
    if(temp == x){
        mvwaddch(server.map,beast->y,beast->x+1,' ');
    }
}

void display_beasts(){
    char x;
    for(int i = 0;i<server.num_of_beasts;i++){
        mvwaddch(server.map,server.beasts[i].y,server.beasts[i].x,'*' | COLOR_PAIR(BEAST_PAIR));
        x = mvwinch(server.map,server.beasts[i].y,server.beasts[i].x);
        beast_correct(i,x);
    }
}

void set_collectibles(int num_of_coins, int num_of_treasures, int num_of_large_treasures){
    int x, y;
    //SET COINS
    for(int i = 0;i<num_of_coins;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'c' | COLOR_PAIR(TREASURE_PAIR));
    }
    //SET TREASURES
    for(int i = 0;i<num_of_treasures;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'t' | COLOR_PAIR(TREASURE_PAIR));
    }
    //SET LARGE TREASURES
    for(int i = 0;i<num_of_large_treasures;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'T' | COLOR_PAIR(TREASURE_PAIR));
    }
}

void collect_money(){
    char temp;
    for(int i = 0;i<server.num_of_players;i++){
        temp = mvwinch(server.map,server.players[i].y,server.players[i].x);
        switch(temp){
            case 'c':
                server.players[i].money_carried++;
                break;
            case 't':
                server.players[i].money_carried += 10;
                break;
            case 'T':
                server.players[i].money_carried += 50;
                break;
            case 'D':
                pick_up_loot(i);
                break;
            default:
                break;
        }
    }
}

void deposit_money(){
    for(int i = 0;i<server.num_of_players;i++){
        if(server.players[i].x == CAMP_X && server.players[i].y == CAMP_Y){
            server.players[i].money_brought += server.players[i].money_carried;
            server.players[i].money_carried = 0;
        }
    }
}

void set_camp(){
    mvwaddch(server.map,CAMP_Y,CAMP_X,'A' | COLOR_PAIR(CAMP_PAIR));
}

void set_bushes(){
    int x,y;
    for(int i = 0;i<BUSHES_AMOUNT;i++){
        do{
            x = rand() % (MAP_WIDTH - 2);
            y = rand() % MAP_HEIGHT;
        }while(mvwinch(server.map,y,x) == (char)219);
        mvwaddch(server.map,y,x,'#');
    }
}

void check_if_bush(int index){
    if(mvwinch(server.map,server.players[index].y,server.players[index].x) == '#'){
         server.players[index].on_bush = 1;
    }
}

void check_if_collission(){
    for(int i = 0;i<server.num_of_players;i++){
        for(int j = 0;j<server.num_of_players;j++){
            if(j == i){
                continue;
            }
            if(server.players[j].x == server.players[i].x && server.players[j].y == server.players[i].y){
                set_death_point_collission(i,j);
            }
        }
    }
}

void set_death_point(int index){
    struct death_point_t* dp = &server.death_points[index];
    if(dp->x != 0 && dp->y != 0){
        mvwaddch(server.map,dp->y,dp->x,' ');
    }
    dp->money = server.players[index].money_carried;
    if(server.players[index].x != 0 && server.players[index].y != 0){
        dp->x = server.players[index].x;
        dp->y = server.players[index].y;
    }
}

void set_death_point_collission(int index1, int index2){
    struct death_point_t* dp1 = &server.death_points[index1];
    struct death_point_t* dp2 = &server.death_points[index2];
    if(dp1->x != 0 && dp1->y != 0){
        mvwaddch(server.map,dp1->y,dp1->x,' ');
    }
    if(dp2->x != 0 && dp2->y != 0){
        mvwaddch(server.map,dp2->y,dp2->x,' ');
    }
    dp1->x = server.players[index1].x;
    dp1->y = server.players[index1].y;
    dp1->money = server.players[index1].money_carried;
    dp2->x = server.players[index2].x;
    dp2->y = server.players[index2].y;
    dp2->money = server.players[index2].money_carried;
    if(dp1->x != 0 && dp1->y != 0){
        mvwaddch(server.map,dp1->y,dp1->x,'D' | COLOR_PAIR(TREASURE_PAIR));
    }
    server.players[index1].x = server.players[index1].spawn_x;
    server.players[index1].y = server.players[index1].spawn_y;
    server.players[index1].money_carried = 0;
    server.players[index1].deaths++;
    server.players[index2].x = server.players[index2].spawn_x;
    server.players[index2].y = server.players[index2].spawn_y;
    server.players[index2].money_carried = 0;
    server.players[index2].deaths++;
}

void pick_up_loot(int index){
    struct player_t* player = &server.players[index];
    for(int i = 0;i<MAX_PLAYER_NUM;i++){
        if(server.death_points[i].x == player->x && server.death_points[i].y == player->y){
            player->money_carried += server.death_points[i].money;
            server.death_points[i].money = 0;
            server.death_points[i].x = 0;
            server.death_points[i].y = 0;
        }
    }
}

void set_legend(){
    WINDOW* win = server.legend;
    mvwprintw(win,1,2,"LEGEND:");
    //PLAYERS
    mvwaddch(win,2,2,'1' | COLOR_PAIR(PLAYER_PAIR));
    mvwaddch(win,2,3,'2' | COLOR_PAIR(PLAYER_PAIR));
    mvwaddch(win,2,4,'3' | COLOR_PAIR(PLAYER_PAIR));
    mvwaddch(win,2,5,'4' | COLOR_PAIR(PLAYER_PAIR));
    mvwprintw(win,2,6," - players");
    //BEAST
    mvwaddch(win,3,2,'*' | COLOR_PAIR(BEAST_PAIR));
    mvwprintw(win,3,3," - wild beast");
    //BUSH
    mvwaddch(win,4,2,'#');
    mvwprintw(win,4,3," - bush (slow down)");
    //BUSH
    mvwaddch(win,5,2,'A' | COLOR_PAIR(CAMP_PAIR));
    mvwprintw(win,5,3," - campsite");
    //COINS
    mvwaddch(win,2,24,'c' | COLOR_PAIR(TREASURE_PAIR));
    mvwprintw(win,2,25," - coin");
    //TREASURE
    mvwaddch(win,3,24,'t' | COLOR_PAIR(TREASURE_PAIR));
    mvwprintw(win,3,25," - treasure");
    //BIG TREASURE
    mvwaddch(win,4,24,'T' | COLOR_PAIR(TREASURE_PAIR));
    mvwprintw(win,4,25," - big treasure");
    wrefresh(win);
    //DROPPED TREASURE
    mvwaddch(win,5,24,'D' | COLOR_PAIR(TREASURE_PAIR));
    mvwprintw(win,5,25," - dropped treasure");
    wrefresh(win);
}