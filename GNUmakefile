.POSIX:
.SUFFIXES:
.PHONY: all clean install check
all:
PROJECT=c-win-mutexed
VERSION=1.0.0
CC     = x86_64-w64-mingw32-gcc -Wall -std=c99
PROGS  =pkg/mutexed.exe
PREFIX =/usr/local

all: $(PROGS)
clean:
	rm -f $(PROGS)
install: $(PROGS)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROGS) $(DESTDIR)$(PREFIX)/bin
check:

$(PROGS): mutexed.c
	$(CC) -o $@ $<

check-syntax:
	$(CC) -o /dev/null -S ${CHK_SOURCES} || true

