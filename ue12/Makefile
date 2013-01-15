CFLAGS+=-Wall -Werror -g -O2

all: envir

envir.o: scorematrix.h linkedlist.h stack.h trie.h

envir: envir.o scorematrix.o linkedlist.o stack.o trie.o
	cc ${LDFLAGS} -o $@ $+


clean:
	-rm -f linkedlist.o envir envir.o scorematrix.o stack.o trie.o

