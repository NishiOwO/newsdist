# $Id$

DOCS = newsdist.8

include $(TOPDIR)/config.mk

.SUFFIXES: .in1 .in2 .in3 .in4 .in5 .in6 .in7 .in8 .1 .2 .3 .4 .5 .6 .7 .8
.PHONY: all install clean

all: $(DOCS)

.in1.1 .in2.2 .in3.3 .in4.4 .in5.5 .in6.6 .in7.7 .in8.8:
	$(CPP) -DDATE="`date +"%Y-%m-%d"`" $< > $@ || ( rm -f $@ ; exit 1 )

install:
	mkdir -p $(DESTDIR)$(PREFIX)/man/man1
	mkdir -p $(DESTDIR)$(PREFIX)/man/man2
	mkdir -p $(DESTDIR)$(PREFIX)/man/man3
	mkdir -p $(DESTDIR)$(PREFIX)/man/man4
	mkdir -p $(DESTDIR)$(PREFIX)/man/man5
	mkdir -p $(DESTDIR)$(PREFIX)/man/man6
	mkdir -p $(DESTDIR)$(PREFIX)/man/man7
	mkdir -p $(DESTDIR)$(PREFIX)/man/man8
	-cp *.1 $(DESTDIR)$(PREFIX)/man/man1/
	-cp *.2 $(DESTDIR)$(PREFIX)/man/man2/
	-cp *.3 $(DESTDIR)$(PREFIX)/man/man3/
	-cp *.4 $(DESTDIR)$(PREFIX)/man/man4/
	-cp *.5 $(DESTDIR)$(PREFIX)/man/man5/
	-cp *.6 $(DESTDIR)$(PREFIX)/man/man6/
	-cp *.7 $(DESTDIR)$(PREFIX)/man/man7/
	-cp *.8 $(DESTDIR)$(PREFIX)/man/man8/

clean:
	-rm -f $(DOCS)
