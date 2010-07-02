CFLAGS := -g -O3
LDFLAGS :=

CFLAGS += `pkg-config --cflags libftdi`
LDFLAGS += `pkg-config --libs libftdi`

pcsconf: pcsconf.c

.PHONY: clean

clean:
	-rm -f pcsconf
