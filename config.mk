# Respect any user-specified flags
QIRK_CFLAGS = $(CFLAGS)
QIRK_LDFLAGS = $(LDFLAGS)

# Release build with LTO by default
QIRK_CFLAGS += -std=gnu11 \
	      -Og     \
	      -g      \
	      -I.
QIRK_LDFLAGS += -lncurses

# Install to /usr/local by default
DESTDIR := /usr/local

# Use GCC by default
CC := gcc

