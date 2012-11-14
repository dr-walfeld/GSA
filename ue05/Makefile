CFLAGS+=-Wall -Werror -O2 #-DUNIT_TEST

objectsa51 = alignment.o optalign.o fasta.o main_a51.o
objectsa53 = alignment.o optalign.o fasta.o main_a53.o

a51 = a51
a53 = a53

all: $(a51) $(a53) fasta_test alignment_test optalign_test

$(a51): $(objectsa51)
	gcc ${LDFLAGS} -o $@ $+
$(a53): $(objectsa53)
	gcc ${LDFLAGS} -o $@ $+

fasta.o: fasta.h
alignment.o: alignment.h
optalign.o: optalign.h alignment.h
main_a53.o: optalign.h alignment.h fasta.h
main_a51.o: optalign.h alignment.h fasta.h

fasta_test: fasta.c fasta.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $+
alignment_test: alignment.c alignment.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $+
optalign_test.o: optalign.c optalign.h alignment.h
	gcc ${CFLAGS} -c -DUNIT_TEST -o $@ optalign.c
optalign_test: optalign_test.o alignment.o
	gcc ${LDFLAGS} -o $@ $+


clean:
	-rm -f $(a51) $(a53)  $(objectsa51) $(objectsa53)	fasta_test alignment_test optalign_test optalign_test.o
