CFLAGS+=-Wall -Werror -g -O2

all: envir pssm_test pssmlook fasta_test scorematrix_test linkedlist_test

envir.o: scorematrix.h linkedlist.h trie.h fasta.h encode.h

envir: envir.o scorematrix.o linkedlist.o trie.o fasta.o encode.o
	cc ${LDFLAGS} -o $@ $+

pssm_test: pssm.c pssm.h
	cc ${CFLAGS} -DUNIT_TEST -o $@ $+

fasta_test: fasta.c fasta.h
	cc ${CFLAGS} -DUNIT_TEST -o $@ $+

scorematrix_test: scorematrix.c scorematrix.h
	cc ${CFLAGS} -DUNIT_TEST -o $@ $+

linkedlist_test: linkedlist.c linkedlist.h
	cc ${CFLAGS} -DUNIT_TEST -o $@ $+

pssmlook: pssm.o fasta.o

clean:
	-rm -f linkedlist_test scorematrix_test fasta_test linkedlist.o envir envir.o scorematrix.o trie.o pssm_test pssm.o pssmlook fasta.o encode.o

