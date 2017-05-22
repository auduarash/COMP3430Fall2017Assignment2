GCC=gcc-4.8
CFLAGS=-Wall -lcurses -pthread


run: main
	$(GCC) -v && ./main


main: main.o example.o console.o
	$(GCC) $(CFLAGS) console.o example.o main.o -o main

main.o: main.c
	$(GCC) $(CFLAGS) -c main.c

example.o: example.h example.c
	$(GCC) $(CFLAGS) -c example.c

console.o: console.h console.c
	$(GCC) $(CFLAGS) -c console.c

clean:
	rm -rf main *.o