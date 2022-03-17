CC = gcc
CFLAGS = -g -Wall -Werror -std=c11
LIB_PATH = lib
MAIN_PATH = main

static:
	$(CC) $(CFLAGS) -c $(LIB_PATH).c -o $(LIB_PATH).o
	ar rcs lib$(LIB_PATH).a $(LIB_PATH).o

clean:
	rm -rf *.o *.a *.so main_*

main_static: static
	$(CC) $(CFLAGS) -o $(MAIN_PATH)_static $(MAIN_PATH).c $(LIB_PATH).o