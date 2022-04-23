CC = gcc
FLAGS = -g
main: main.c
	$(CC) $(FLAGS) main.c -o main.out

clean:
	rm -Rf *.out

test: main
	./main.out commands.txt

test1: main
	valgrind ./main.out commands.txt