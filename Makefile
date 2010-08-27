ARCH = msp430x169
CC := msp430-gcc

CFLAGS := -g -mmcu=${ARCH} -Wall -Werror -O3 -std=gnu99
CFLAGS += -include `pwd`/config.h
LDFLAGS :=

O_FILES = main.o smps.o sric-mux.o cmds.o
SUBDIRS = drivers libsric

LDFLAGS += -Ldrivers -ldrivers
LDFLAGS += -Llibsric -lsric

all: pcs

include depend

pcs: ${O_FILES} ${SUBDIRS}
	${CC} -o $@ ${O_FILES} ${LIB_FILES} ${CFLAGS} ${LDFLAGS}

drivers:
	$(MAKE) -C $@ CC=${CC} ARCH=${ARCH} CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"

libsric:
	$(MAKE) -C $@ CC=${CC} ARCH=${ARCH} CFLAGS="${CFLAGS} -I`pwd`" LDFLAGS="${LDFLAGS}"

depend: *.c
	rm -f depend
	for file in $^; do \
		${CC} ${CFLAGS} -MM $$file -o - >> $@ ; \
	done ;

gw-fsm.pdf: gw-fsm.dot
	dot -Tpdf gw-fsm.dot -o gw-fsm.pdf

.PHONY: clean ${SUBDIRS}

clean:
	-rm -f pcs depend *.o
	for d in ${SUBDIRS} ; do\
		${MAKE} -C $$d clean ; \
	done ;
