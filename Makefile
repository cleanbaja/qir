# Makefile for qir - Copyright (c) 2021 Yusuf M (cleanbaja)

# POSIX Compatible mode. Also build silently
.POSIX:
# .SILENT:

include config.mk

SRC = qir.c
OBJ = ${SRC:.c=.o}

all: config.mk qir

.c.o:
	echo CC $<
	$(CC) $(QIR_CFLAGS) -o $@ -c $<

qir: $(OBJ)
	echo LINK qir
	$(CC) -o $@ $< $(QIR_LDFLAGS)

clean:
	rm -f qir $(OBJ)

install: all
	install -p qir $(DESTDIR)/bin/
	mkdir -p "$(DESTDIR)/man/man1"
	sed "s/VERSION/$(VERSION)/g" < qir.1 > "$(DESTDIR)/man/man1/qir.1"
	chmod 644 "$(DESTDIR)/man/man1/qir.1"

uninstall:
	rm -f $(DESTDIR)/qir $(DESTDIR)/man/man1/qir.1

.PHONY: all clean install uninstall

