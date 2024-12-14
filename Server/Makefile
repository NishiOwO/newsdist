# $Id$

include $(TOPDIR)/config.mk

OBJS = main.o yconf.tab.o

.PHONY: all clean
.SUFFIXES: .c .o

all: newsdist$(EXEC)

newsdist$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

yconf.tab.c: config.y
	$(YACC) -d -b yconf config.y

clean:
	rm -f *.o *.exe newsdist *.tab.*
