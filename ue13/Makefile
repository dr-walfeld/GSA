CFLAGS+=-Wall -Werror -g -O2

all: fasta_test msa_test

fasta_test: fasta.c fasta.h
	cc ${CFLAGS} -DUNIT_TEST -o $@ $+

msa_test.o: msa.c
	cc ${CFLAGS} -DUNIT_TEST -c -o $@ $+

msa_test: msa_test.o fasta.o
	cc ${LDFLAGS} -o $@ $+

clean:
	-rm -f fasta_test msa_test msa_test.o fasta.o

