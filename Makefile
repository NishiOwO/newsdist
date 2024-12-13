# $Id$

.PHONY: all clean ./Server

all: ./Server

./Server: config.h

config.h:
	./config

clean:
	rm -f config.h
	$(MAKE) -C ./Server $(FLAGS)
