CFLAGS+=-Wall -Werror -O2

all: qgram

qgram.o: linkedlist.h

qgram: qgram.o linkedlist.o
	cc ${LDFLAGS} -o $@ $+

clean:
	-rm -f qgram.o qgram linkedlist.o

