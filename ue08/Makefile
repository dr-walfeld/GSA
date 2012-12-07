CFLAGS+=-Wall -Werror -O2 #-DUNIT_TEST

all: substr affinealign fasta_test alignment_test optalign_test

fasta.o: fasta.h
alignment.o: alignment.h
optalignaffine.o: optalignaffine.h alignment.h
affinealign.o: fasta.h alignment.h optalignaffine.h

fasta_test: fasta.c fasta.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $+
alignment_test.o: alignment.c alignment.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ alignment.c
alignment_test: alignment_test.o
	gcc ${LDFLAGS} -o $@ $+
optalign_test.o: optalignaffine.c optalignaffine.h alignment.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ optalignaffine.c
optalign_test: optalign_test.o alignment.o
	gcc ${LDFLAGS} -o $@ $+
affinealign: affinealign.o optalignaffine.o alignment.o fasta.o
	gcc ${LDFLAGS} -o $@ $+

substr: substr.c
	gcc ${CFLAGS} -o $@ $+


clean:
	-rm -f $(swalignO) substr affinealign fasta_test alignment_test optalign_test optalign_test.o alignment_test.o fasta.o alignment.o optalignaffine.o affinealign.o
