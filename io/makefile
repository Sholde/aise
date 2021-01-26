CC=gcc
CFLAGS=-Wall -O2 -march=native

.PHONY: clean

all: particle

particle: particle.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *~ particle
