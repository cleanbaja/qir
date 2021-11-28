# Makefile for qirk - Copyright (c) 2021 Yusuf M (cleanbaja)

# POSIX Compatible mode. Also build silently
.POSIX:
.SILENT:

VERSION := 0.1.0

include config.mk

SRC = qirk.c    \
      window.c \
      client.c 
OBJ = ${SRC:.c=.o}

all: config.mk qirk

.c.o:
	echo CC $<
	$(CC) $(QIRK_CFLAGS) -o $@ -c $<

qirk: $(OBJ) 
	echo LINK qirk
	$(CC) -o $@ $(OBJ) $(QIRK_LDFLAGS)

clean:
	rm -f qirk $(OBJ)

install: all
	install -p qirk $(DESTDIR)/bin/
	mkdir -p "$(DESTDIR)/man/man1"
	sed "s/VERSION/$(VERSION)/g" < man/qirk.1 > "$(DESTDIR)/man/man1/qirk.1"
	chmod 644 "$(DESTDIR)/man/man1/qirk.1"

uninstall:
	rm -f $(DESTDIR)/qirk $(DESTDIR)/man/man1/qirk.1

.PHONY: all clean install uninstall

