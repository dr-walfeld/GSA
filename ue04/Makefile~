CFLAGS=-Wall -Werror -O2

all: a41 a43

a43: a43.c
	gcc ${CFLAGS} -o $@ $<

a41: a41.c
	gcc ${CFLAGS} -o $@ $<

clean: 
	find . -type f -executable -exec rm -f {} +
