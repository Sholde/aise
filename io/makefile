CC=gcc
CFLAGS=-Wall -O2 -march=native

.PHONY: clean particle pipe redirect

all: particle pipe redirect

particle: particle.c
	$(CC) $(CFLAGS) $< -o $@

pipe: pipe.c
	$(CC) $(CFLAGS) $< -o $@

redirect: redirect.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *~ particle pipe redirect toto.txt
