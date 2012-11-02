CFLAGS=-Wall -Werror -O2

all: a21 

a21: a21.c
	gcc ${CFLAGS} -o $@ $<

clean: 
	find . -type f -executable -exec rm -f {} +
