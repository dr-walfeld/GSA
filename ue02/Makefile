CFLAGS=-Wall -Werror -O2

all: a21 a21opt a23 

a21: a21.c
	gcc ${CFLAGS} -o $@ $<

a21opt: a21opt.c
	gcc ${CFLAGS} -o $@ $<

a23: a23.c
	gcc ${CFLAGS} -o $@ $<
clean: 
	find . -type f -executable -exec rm -f {} +
