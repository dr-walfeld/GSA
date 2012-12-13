#CFLAGS+=-Wall -Werror -O2 #-DUNIT_TEST

all: aligntest linspace_test fasta_test alignment_test 

fasta.o: fasta.h
alignment.o: alignment.h
linspace.o: alignment.h
optalign.o: optalign.h
alignmenttest.o: fasta.h alignment.h optalign.h linspace.h

fasta_test: fasta.c fasta.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $+
linspace_test.o: linspace.c alignment.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ linspace.c
alignment_test.o: alignment.c alignment.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ alignment.c
linspace_test: linspace_test.o alignment.o
	gcc ${LDFLAGS} -o $@ $+

alignment_test: alignment_test.o
	gcc ${LDFLAGS} -o $@ $+
aligntest: alignmenttest.o fasta.o alignment.o optalign.o linspace.o
	gcc ${LDFLAGS} -o $@ $+

clean:
	-rm -f linspace_test linspace_test.o optalign_test aligntest.o optalign.o alignmenttest alignmenttest.o linspace.o fasta_test alignment_test alignment_test.o fasta.o alignment.o
