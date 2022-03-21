clean:
	rm unbounded_int
unbounded: unbounded_int.c
	gcc -Wall -g -pedantic unbounded_int.c -o unbounded_int
calc: calc_unbounded_int.c
	gcc -Wall -g -pedantic calc_unbounded_int.c -o calc_unbounded_int