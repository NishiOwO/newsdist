# $Id$

include $(TOPDIR)/config.mk

OBJS  = 

.PHONY: all install clean
.SUFFIXES: .c .o

all: common.a

common.a: $(OBJS)
	$(AR) rc $@ $(OBJS)
	$(RANLIB) $@

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.a

install: all
	@true
