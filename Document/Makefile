# $Id$

DOCS = newsdist.8

include $(TOPDIR)/config.mk

.SUFFIXES: .in1 .in2 .in3 .in4 .in5 .in6 .in7 .in8 .1 .2 .3 .4 .5 .6 .7 .8
.PHONY: all install clean

all: $(DOCS)

.in1.1 .in2.2 .in3.3 .in4.4 .in5.5 .in6.6 .in7.7 .in8.8:
	$(CPP) $< > $@ || ( rm -f $@ ; exit 1 )

install:
	-cp *.1 $(DESTDIR)$(PREFIX)/share/man/man1/
	-cp *.2 $(DESTDIR)$(PREFIX)/share/man/man2/
	-cp *.3 $(DESTDIR)$(PREFIX)/share/man/man3/
	-cp *.4 $(DESTDIR)$(PREFIX)/share/man/man4/
	-cp *.5 $(DESTDIR)$(PREFIX)/share/man/man5/
	-cp *.6 $(DESTDIR)$(PREFIX)/share/man/man6/
	-cp *.7 $(DESTDIR)$(PREFIX)/share/man/man7/
	-cp *.8 $(DESTDIR)$(PREFIX)/share/man/man8/

clean:
	-rm -f $(DOCS)
