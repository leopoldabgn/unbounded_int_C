clean:
	rm -f prog *.o
	rm -f unbounded_int
	rm -f test_unbounded
	rm -f calc_unbounded_int
unbounded: unbounded_int.c
	gcc -Wall -g -pedantic unbounded_int.c -o unbounded_int
test_unbounded: test_unbounded.c
	gcc -Wall -g -pedantic test_unbounded.c -o test_unbounded
calc: calc_unbounded_int.c
	gcc -Wall -g -pedantic calc_unbounded_int.c -o calc_unbounded_int