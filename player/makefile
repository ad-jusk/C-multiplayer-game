OBJ=./obj
SRC=./src
CFLAGS=-g -c -Wall
RIGHT_FLAGS=-lpthread -lncurses
CC=gcc

all: $(OBJ) $(OBJ)/player

#Create object folder
$(OBJ):
	mkdir -p $(OBJ)

$(OBJ)/player: $(OBJ)/player_main.o $(OBJ)/player_functions.o
	$(CC) -g -Wall $^ -o player $(RIGHT_FLAGS)

$(OBJ)/player_main.o: $(SRC)/player_main.c $(SRC)/player_functions.c $(SRC)/player_libs.h
	$(CC) $(CFLAGS) $< $(RIGHT_FLAGS)
	mv player_main.o $(OBJ)

$(OBJ)/player_functions.o: $(SRC)/player_functions.c $(SRC)/player_libs.h
	$(CC) $(CFLAGS) $< $(RIGHT_FLAGS)
	mv player_functions.o $(OBJ)

.PHONY: clean

clean:
	-rm $(OBJ)/*.o
	-rm player