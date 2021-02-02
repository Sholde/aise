CC=gcc
CFLAGS=-O1 -march=native -Wall -g3

.PHONY: clean libstatic libdynamic main_static main_dynamic

all: main_static main_dynamic

libstatic: custom.c
	$(CC) $(CFLAGS) -c custom.c -o custom.o
	ar rcs libcustom.a custom.o

libdynamic: custom.c
	$(CC) $(CFLAGS) -fPIC -c custom.c -o custom.o
	$(CC) -shared -o libcustom.so custom.o

main_static: main.c libstatic
	$(CC) $(CFLAGS) $< -o $@ -static -I. -L. -lcustom 

main_dynamic: main.c libdynamic
	$(CC) $(CFLAGS) $< -o $@ -I. -L. -lcustom
	@echo "Need an export of LD_LIBRARY_PATH"

clean:
	rm -Rf *~ *.o *.a *.so main_static main_dynamic
