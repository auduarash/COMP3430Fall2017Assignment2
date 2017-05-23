GCC=gcc-4.8
CFLAGS=-Wall
LINKFLAGS=-lcurses -pthread


run: main
	./main


main: main.o example.o console.o
	$(GCC) $(CFLAGS) console.o example.o main.o -o main $(LINKFLAGS)

main.o: main.c
	$(GCC) $(CFLAGS) -c main.c

example.o: example.h example.c
	$(GCC) $(CFLAGS) -c example.c

console.o: console.h console.c
	$(GCC) $(CFLAGS) -c console.c

clean:
	rm -rf main *.o