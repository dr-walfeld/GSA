CFLAGS+=-Wall -Werror -g -O2

all: scorefasta

scorefasta.o: encode.h linkedlist.h

scorefasta: scorefasta.o encode.o linkedlist.o
	cc ${LDFLAGS} -o $@ $+

clean:
	-rm -f scorefasta.o scorefasta encode.o linkedlist.o

