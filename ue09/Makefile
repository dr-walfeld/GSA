#CFLAGS+=-Wall -Werror -O2 #-DUNIT_TEST

all: linspaceoptalign linspace_test fasta_test alignment_test optalign_test

fasta.o: fasta.h
alignment.o: alignment.h
linspace.o: alignment.h
optalign.o: optalign.h
linspaceoptalign.o: fasta.h alignment.h optalign.h linspace.h

fasta_test: fasta.c fasta.h
	cc ${CFLAGS} -DUNIT_TEST -o $@ $+
optalign_test.o: optalign.c optalign.h alignment.h
	cc ${CFLAGS} -c -DUNIT_TEST -o $@ optalign.c
linspace_test.o: linspace.c alignment.h
	cc ${CFLAGS} -c -DUNIT_TEST -o $@ linspace.c
alignment_test.o: alignment.c alignment.h
	cc ${CFLAGS} -c -DUNIT_TEST -o $@ alignment.c
linspace_test: linspace_test.o alignment.o
	cc ${LDFLAGS} -o $@ $+

alignment_test: alignment_test.o
	cc ${LDFLAGS} -o $@ $+
optalign_test: optalign_test.o alignment.o
	cc ${LDFLAGS} -o $@ $+
linspaceoptalign: linspaceoptalign.o fasta.o alignment.o optalign.o linspace.o
	cc ${LDFLAGS} -o $@ $+
aligntest: alignmenttest.o fasta.o alignment.o optalign.o linspace.o
	cc ${LDFLAGS} -o $@ $+

clean:
	-rm -f linspace_test linspace_test.o optalign_test.o optalign_test optalign.o linspaceoptalign linspaceoptalign.o linspace.o fasta_test alignment_test alignment_test.o fasta.o alignment.o

