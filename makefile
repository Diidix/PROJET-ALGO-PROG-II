appli: main.o autocomplete.o
	gcc main.o autocomplete.o -o appli

main.o: autocomplete.h main.c
	gcc -c main.c

autocomplete.o: autocomplete.h autocomplete.c
	gcc -c autocomplete.c


