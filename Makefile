# $Id$

include config.mk

.PHONY: all clean ./Server

all: ./Server

./Server: config.h
	$(MAKE) -C $@ $(FLAGS)

config.h config.mk:
	./config

clean:
	rm -f config.h config.mk
	$(MAKE) -C ./Server $(FLAGS)
