# Makefile for qirk - Copyright (c) 2021 Yusuf M (cleanbaja)

# POSIX Compatible mode. Also build silently
.POSIX:
.SILENT:

VERSION := 0.1.1
NOTIFY := printf "  \\033[1;96m%8s\\033[0m  \\033[1;m%s\\033[0m\\n"

include config.mk

SRC = qirk.c 
OBJ = ${SRC:.c=.o}

all: config.mk qirk

.c.o:
	$(NOTIFY) "CC" "$<"
	$(CC) $(QIRK_CFLAGS) -o $@ -c $<

qirk: $(OBJ) 
	$(NOTIFY) "LINK" "qirk"
	$(CC) -o $@ $(OBJ) $(QIRK_LDFLAGS)

clean:	notags
	$(NOTIFY) "CLEAN" "qirk"
	rm -f qirk $(OBJ)

install: all
	install -p qirk $(DESTDIR)/bin/
	mkdir -p "$(DESTDIR)/man/man1"
	sed "s/VERSION/$(VERSION)/g" < man/qirk.1 > "$(DESTDIR)/man/man1/qirk.1"
	chmod 644 "$(DESTDIR)/man/man1/qirk.1"

uninstall:
	rm -f $(DESTDIR)/qirk $(DESTDIR)/man/man1/qirk.1

tags:
	$(NOTIFY) "GEN" "tags"
	ctags ./*.c

notags:
	$(NOTIFY) "CLEAN" "tags"
	rm -f tags

.PHONY: all clean install uninstall

