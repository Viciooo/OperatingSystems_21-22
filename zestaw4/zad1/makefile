CC = gcc
CFLAGS = -Wall -Werror -std=c11 -D_GNU_SOURCE
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

test:
	make main_static
	./main_static fork ignore
	./main_static fork handler
	./main_static fork mask
	./main_static fork pending
	./main_static exec ignore
	./main_static exec handler
	./main_static exec mask
	make clean
