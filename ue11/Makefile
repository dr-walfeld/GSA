CFLAGS+=-Wall -Werror -g -O2

all: scorefasta createmsp

scorefasta.o: encode.h linkedlist.h


scorefasta: scorefasta.o encode.o linkedlist.o
	cc ${LDFLAGS} -o $@ $+


createmsp.o: fasta.h

createmsp: createmsp.o fasta.o

clean:
	-rm -f scorefasta.o scorefasta encode.o linkedlist.o fasta.o createmsp.o createmsp

