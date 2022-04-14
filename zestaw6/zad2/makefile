CC = gcc
FLAGS = -g
main: main.c
	$(CC) $(FLAGS) main.c -o main.out

clean:
	rm -Rf *.out

test1: main
	./main.out "sender"

test2: main
	./main.out "date"

test3: main
	./main.out viciooo mock_topic "content content content"

test_1: main
	valgrind ./main.out "date"
