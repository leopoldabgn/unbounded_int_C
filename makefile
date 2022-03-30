clean:
	rm -f prog *.o
	rm -f test_unbounded
	rm -f calc_unbounded_int
test_unbounded: test_unbounded.c
	gcc -Wall -g -pedantic unbounded_int.c test_unbounded.c -o test_unbounded
calc: calc_unbounded_int.c
	gcc -Wall -g -pedantic unbounded_int.c calc_unbounded_int.c -o calc_unbounded_int