CC = gcc
CFLAGS = -g -Wall -Werror -std=c11
LIB_PATH = ../lib
LIB_NAME = lib

MAIN_PATH = main

static:
	$(CC) $(CFLAGS) -c $(LIB_PATH).c -o $(LIB_PATH).o
	ar rcs lib$(LIB_NAME).a $(LIB_PATH).o

clean:
	rm -rf *.o *.a *.so main_* $(LIB_PATH).o

main_static: static
	$(CC) $(CFLAGS) -o $(MAIN_PATH)_static $(MAIN_PATH).c $(LIB_PATH).o

test1:
	make clean
	make main_static
	valgrind ./main_static "w" "tmp1.txt"
	make clean