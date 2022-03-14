CC = gcc
CFLAGS = -g -Wall -Werror -std=c11 $(OPT)
LIB_PATH = lib
MAIN_PATH = main

static:
	$(CC) $(CFLAGS) -c $(LIB_PATH).c -o $(LIB_PATH).o
	ar rcs lib$(LIB_PATH).a $(LIB_PATH).o

shared:
	$(CC) $(CFLAGS) -fPIC -c $(LIB_PATH).c -o $(LIB_PATH).o
	$(CC) $(CFLAGS) -shared -o lib$(LIB_PATH).so $(LIB_PATH).o

clean:
	rm -rf *.o *.a *.so main_*

main_dynamic: dynamic
	$(CC) $(CFLAGS) -o $(MAIN_PATH)_dynamic $(MAIN_PATH).c $(LIB_PATH).o

main_static: static
	$(CC) $(CFLAGS) -o $(MAIN_PATH)_static $(MAIN_PATH).c $(LIB_PATH).o

main_shared: shared
	$(CC) $(CFLAGS) -o $(MAIN_PATH)_shared $(MAIN_PATH).c $(LIB_PATH).o

test1_static:
	make clean
	make main_static
	valgrind ./main_static "create_table" "1" "wc_files" "small.txt big.txt"
	make clean

test2_static:
	make clean
	make main_static
	valgrind ./main_static "create_table" "5" "wc_files" "small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt"
	make clean

test3_static:
	make clean
	make main_static
	valgrind ./main_static "create_table" "10" "wc_files" "small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt"
	make clean

test4_static:
	make clean
	make main_static
	valgrind ./main_static "create_table" "2" "wc_files" "small.txt big.txt" "wc_files" "small.txt big.txt medium.txt big.txt" "remove_block" "1" "wc_files" "small.txt big.txt medium.txt big.txt"
	make clean

test1_shared:
	make clean
	make main_shared
	valgrind ./main_shared "create_table" "1" "wc_files" "small.txt big.txt"
	make clean

test2_shared:
	make clean
	make main_shared
	valgrind ./main_shared "create_table" "5" "wc_files" "small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt"
	make clean

test3_shared:
	make clean
	make main_shared
	valgrind ./main_shared "create_table" "10" "wc_files" "small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt" "wc_files" "small.txt big.txt medium.txt small.txt big.txt medium.txt small.txt big.txt medium.txt"
	make clean

test4_shared:
	make clean
	make main_shared
	valgrind ./main_shared "create_table" "2" "wc_files" "small.txt big.txt" "wc_files" "small.txt big.txt medium.txt big.txt" "remove_block" "1" "wc_files" "small.txt big.txt medium.txt big.txt"
	make clean
