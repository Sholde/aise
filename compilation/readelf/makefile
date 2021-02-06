CC=gcc
CFLAGS=-Wall -g3 -march=native -O1

.PHONY: clean

all: main

main: main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *~ *.o main
