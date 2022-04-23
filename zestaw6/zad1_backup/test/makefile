CC = gcc
FLAGS = -std=c99 -Wall
ESSENTIALS = keygen/keygen.c que/que.c utils/utils.c

all:
	make server
	make client

server:
	$(CC) $(FLAGS) server.c $(ESSENTIALS) -o server

client:
	$(CC) $(FLAGS) client.c $(ESSENTIALS) -o client

clean:
	rm server client