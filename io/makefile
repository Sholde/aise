CC=gcc
CFLAGS=-Wall -O2 -march=native

.PHONY: clean particle pipe

all: particle pipe

particle: particle.c
	$(CC) $(CFLAGS) $< -o $@

pipe: pipe.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *~ particle pipe
