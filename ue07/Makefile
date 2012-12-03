CFLAGS+=-Wall -Werror -O2 #-DUNIT_TEST

all: swalign scorematrix_test fasta_test alignment_test optalign_test approxmatch

swalignO = alignment.o optalign.o fasta.o scorematrix.o swmain.o

swalign: $(swalignO)
		gcc ${LDFLAGS} -o $@ $+

fasta.o: fasta.h
alignment.o: alignment.h
optalign.o: optalign.h alignment.h scorematrix.h
scorematrix.o: scorematrix.h
swmain.o: fasta.h alignment.h optalign.h scorematrix.h

fasta_test: fasta.c fasta.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $+
alignment_test.o: alignment.c alignment.h scorematrix.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ alignment.c
alignment_test: alignment_test.o scorematrix.o
	gcc ${LDFLAGS} -o $@ $+
optalign_test.o: optalign.c optalign.h alignment.h scorematrix.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ optalign.c
optalign_test: optalign_test.o alignment.o scorematrix.o
	gcc ${LDFLAGS} -o $@ $+
scorematrix_test: scorematrix.c scorematrix.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $+

approxmatch: approxmatch.c
	gcc ${CFLAGS} -o $@ $+

clean:
	-rm -f $(swalignO) approxmatch scorematrix_test fasta_test alignment_test optalign_test optalign_test.o alignment_test.o
