INSTALLDIR ?= /root/bin

getaddr: main.c
	cc -std=c99 -static -o $@ main.c

install: getaddr
	install -v getaddr $(INSTALLDIR)
