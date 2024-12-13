# $Id$

include config.mk

.PHONY: all distclean clean ./Server

FLAGS = TOPDIR=$(TOPDIR)

all: ./Server

./Server:: config.h
	$(MAKE) -C $@ $(FLAGS)

config.h config.mk:
	./config

clean:
	$(MAKE) -C ./Server clean $(FLAGS)

distclean: clean
	rm -f config.h config.mk
