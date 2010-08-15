ARCH = msp430x169
CC := msp430-gcc

CFLAGS := -g -mmcu=${ARCH} -Wall -O3
LDFLAGS :=

O_FILES = main.o sric.o hostser.o crc16.o usart.o gw.o smps.o sric-mux.o

all: pcs

include depend

pcs: ${O_FILES}
	${CC} -o $@ ${O_FILES} ${CFLAGS} ${LDFLAGS}

depend: *.c
	rm -f depend
	for file in $^; do \
		${CC} ${CFLAGS} -MM $$file -o - >> $@ ; \
	done ;

gw-fsm.pdf: gw-fsm.dot
	dot -Tpdf gw-fsm.dot -o gw-fsm.pdf

.PHONY: clean

clean:
	-rm -f pcs depend *.o

