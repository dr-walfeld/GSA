CFLAGS+=-Wall -Werror -O2 #-DUNIT_TEST

objects = alignment.o dynamicprog.o fasta.o
progname = align

all: $(progname) fasta_test alignment_test

$(progname): $(objects)
	gcc ${LDFLAGS} -o $@ $(objects)

fasta.o: fasta.h

alignment.o: alignment.h

dynamicprog.o: alignment.h fasta.h

fasta_test: fasta.c fasta.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $<
alignment_test: alignment.c alignment.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $<

clean:
	-rm $(progname)  $(objects)	fasta_test alignment_test
