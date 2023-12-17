CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2

all: pong

pong: main.o utils.o game.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: src/main.c src/utils.h src/game.h src/constants.h
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: src/utils.c src/utils.h src/constants.h src/game.h
	$(CC) $(CFLAGS) -c -o $@ $<

game.o: src/game.c src/game.h src/constants.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o pong
