ARCH = msp430x169
CC := msp430-gcc

CFLAGS := -g -mmcu=${ARCH} -Wall -O3
LDFLAGS :=

H_FILES = leds.h fields.h sric.h hostser.h crc16.h usart.h
C_FILES = main.c sric.c hostser.c crc16.c usart.c

pcs: ${H_FILES} ${C_FILES}
	${CC} -o $@ ${C_FILES} ${CFLAGS} ${LDFLAGS}

.PHONY: clean

clean:
	-rm -f pcs

