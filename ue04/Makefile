CFLAGS+=-Wall -Werror -O2 -DUNIT_TEST

all: alignment a43

a43: a43.c
	gcc ${CFLAGS} -o $@ $<

alignment: alignment.c
	gcc ${CFLAGS} -o $@ $<

clean: 
	find . -type f -executable -exec rm -f {} +
