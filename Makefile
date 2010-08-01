ARCH = msp430x169
CC := msp430-gcc

CFLAGS := -g -mmcu=${ARCH} -Wall -O3
LDFLAGS :=

H_FILES = leds.h fields.h sric.h hostser.h crc16.h usart.h gw.h
C_FILES = main.c sric.c hostser.c crc16.c usart.c gw.c

pcs: ${H_FILES} ${C_FILES}
	${CC} -o $@ ${C_FILES} ${CFLAGS} ${LDFLAGS}

gw-fsm.pdf: gw-fsm.dot
	dot -Tpdf gw-fsm.dot -o gw-fsm.pdf

.PHONY: clean

clean:
	-rm -f pcs

