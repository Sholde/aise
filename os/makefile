CC=gcc
CFLAGS=-Wall -O2 -march=native

.PHONY= whinch alarm randsig clean infinite killme

all: whinch alarm randsig infinite killme

whinch: whinch.c
	$(CC) $(CFLAGS) $< -o $@

alarm: alarm.c
	$(CC) $(CFLAGS) $< -o $@

randsig: randsig.c
	$(CC) $(CFLAGS) $< -o $@

infinite: infinite.c
	$(CC) $(CFLAGS) $< -o $@

killme: killme.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *~ whinch alarm randsig infinite killme
