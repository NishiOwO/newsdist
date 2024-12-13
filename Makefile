# $Id$

.PHONY: all distclean clean ./Server

DIRS = ./Server

FLAGS = TOPDIR=..

all: $(DIRS)

./Server:: config.h
	$(MAKE) -C $@ $(FLAGS)

config.h config.mk:
	./config

clean:
	for i in $(DIRS); do \
		$(MAKE) -C $$i clean $(FLAGS) ; \
	done

distclean:
	touch config.mk
	for i in $(DIRS); do \
		$(MAKE) -C $$i clean $(FLAGS) ; \
	done
	rm -f config.h config.mk
