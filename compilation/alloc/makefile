CC=gcc
CFLAGS=-Wall

.PHONY: clean lib

all: lib main

lib: alloc_debugger.c
	$(CC) -shared -fPIC -o liballoc_debbuger.so $< -ldl

main: main.c
	$(CC) $(CFLAGS) $@.c -o $@

run: main
	@LD_PRELOAD=./liballoc_debbuger.so ./main

clean:
	rm -Rf *~ *.so *.o main
