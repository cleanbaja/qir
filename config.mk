# Respect any user-specified flags
QIR_CFLAGS = $(CFLAGS)
QIR_LDFLAGS = $(LDFLAGS)

# Release build with LTO by default
QIR_CFLAGS += -std=c11 \
	      -Ofast   \
	      -flto    \
	      -I.
QIR_LDFLAGS += -flto -lncurses

# Install to /usr/local by default
DESTDIR := /usr/local

# Use GCC by default
CC := gcc

