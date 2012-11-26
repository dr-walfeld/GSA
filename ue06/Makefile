CFLAGS+=-Wall -Werror -O2

all: substringmatching scorematrix

substringmatching: substringmatching.c
	gcc ${CFLAGS} -o $@ $<

scorematrix: scorematrix.c scorematrix.h
	gcc ${CFLAGS} -DUNIT_TEST -o $@ $<

clean: 
	rm -f substringmatching scorematrix
