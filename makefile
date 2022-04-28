# This is temporary. Modifications are still required.
CC=gcc
CFLAGS=-Wall -pedantic -g

test_unbounded: test_unbounded.c
	$(CC) $(CFLAGS) -o $@ unbounded_int.c $<
calc: calc_unbounded_int.c
	$(CC) $(CFLAGS) -o $@ unbounded_int.c $<
clean:
	rm -f prog *.o
	rm -f test_unbounded
	rm -f calc