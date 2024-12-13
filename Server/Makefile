# $Id$

include $(TOPDIR)/config.mk

OBJS = main.o

.PHONY: all clean
.SUFFIXES: .c .o

all: newsdist$(EXEC)

newsdist$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.exe newsdist
