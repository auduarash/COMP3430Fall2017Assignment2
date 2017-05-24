GCC=gcc-4.8
CFLAGS=-Wall
LINKFLAGS=-lcurses -pthread


run: main
	./main


main: main.o console.o draw_screen.o player.o
	$(GCC) $(CFLAGS) console.o draw_screen.o player.o main.o -o main $(LINKFLAGS)

main.o: main.c
	$(GCC) $(CFLAGS) -c main.c

# example.o: example.h example.c
# 	$(GCC) $(CFLAGS) -c example.c

console.o: console.h console.c
	$(GCC) $(CFLAGS) -c console.c

player.o: player.h player.c
	$(GCC) $(CFLAGS) -c player.c

draw_screen.o: draw_screen.h draw_screen.c
	$(GCC) $(CFLAGS) -c draw_screen.c

clean:
	rm -rf main *.o