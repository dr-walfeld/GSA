CFLAGS=-Wall -Werror -O2

all: a32 

a32: a32.c
	gcc ${CFLAGS} -o $@ $<

clean: 
	find . -type f -executable -exec rm -f {} +
