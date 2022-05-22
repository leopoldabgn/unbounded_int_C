# Instuction: Run make. That's it. 
CC=gcc
CFLAGS=-Wall -g -pedantic 
all: message clean calc_unbounded_int test_unbounded run_tests run_calc
$(VERBOSE).SILENT:

message:
	printf "\tWelcome to the Project C.\n"
	sleep 1
	printf "\t  by Léopold & Paris\n"
	sleep 0.25
	printf "[LOG]: Starting compiling"
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	sleep 0.25
	printf "Ready!\n"

calc_unbounded_int: calc_unbounded_int.o unbounded_int.o
	sleep 0.25
	printf "[LOG]: Creating calc_unbounded"
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	$(CC) $(CFLAGS) -o $@ $^ -lm
	sleep 0.25
	printf "Ok!\n"

calc_unbounded_int.o: calc_unbounded_int.c unbounded_int.h
	$(CC) $(CFLAGS) -o $@ -c $< -lm

test_unbounded: test_unbounded.o unbounded_int.o
	sleep 0.25
	printf "[LOG]: Creating test_unbounded"
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	$(CC) $(CFLAGS) -o $@ $^ -lm
	sleep 0.25
	printf "Ok!\n"

test_unbounded.o: test_unbounded.c unbounded_int.h
	$(CC) $(CFLAGS) -o $@ -c $< -lm

unbounded_int.o: unbounded_int.c unbounded_int.h
	$(CC) $(CFLAGS) -o $@ -c $< -lm

run_tests:
	sleep 1
	./test_unbounded
	printf "\n\n\n"

run_calc:
	printf "\t[LOG]: Running calculator"
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	sleep 0.25
	printf ".\n"
	./calc_unbounded_int

clean:
	sleep 0.25
	printf "[LOG]: Cleaning files"
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	sleep 0.25
	printf "."
	rm -f *.o
	rm -f test_unbounded
	rm -f calc_unbounded_int
	sleep 0.25
	printf 'OK!\n'