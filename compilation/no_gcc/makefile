PROG=main

all: program

program: $(PROG).c
	cpp $< -o $(PROG).i
	/usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/cc1 -fno-verbose-asm $(PROG).i -o $(PROG).s
	as $(PROG).s -o $(PROG).o
	ld $(PROG).o -o $@ /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o -lc --dynamic-linker /usr/lib/ld-linux-x86-64.so.2

clean:
	rm -Rf *~ *.i *.s *.o program
