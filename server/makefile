OBJ=./obj
SRC=./src
CFLAGS=-g -c -Wall
RIGHT_FLAGS=-lpthread -lncurses
CC=gcc

all: $(OBJ) $(OBJ)/server

#Create object folder
$(OBJ):
	mkdir -p $(OBJ)

$(OBJ)/server: $(SRC)/server_main.o $(SRC)/server_functions.o $(SRC)/beast.o $(SRC)/server_map.o
	$(CC) -g -Wall $^ -o server $(RIGHT_FLAGS)
	mv $(SRC)/*.o $(OBJ)

$(OBJ)/server_main.o: $(SRC)/server_main.c $(SRC)/server_libs.h
	$(CC) $(CFLAGS) $< $(RIGHT_FLAGS)

$(OBJ)/map.o: $(SRC)/server_map.c $(SRC)/server_map.h
	$(CC) $(CFLAGS) $< $(RIGHT_FLAGS)

$(OBJ)/server_functions.o: $(SRC)/server_functions.c $(SRC)/server_functions.h
	$(CC) $(CFLAGS) $< $(RIGHT_FLAGS)

$(OBJ)/beast.o: $(SRC)/beast.c $(SRC)/beast.h
	$(CC) $(CFLAGS) $< $(RIGHT_FLAGS)

.PHONY: clean

clean:
	-rm $(OBJ)/*.o server
