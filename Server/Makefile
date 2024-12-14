# $Id$

include $(TOPDIR)/config.mk

OBJS = main.o yconf.tab.o yconf.yy.o

.PHONY: all clean
.SUFFIXES: .c .o

all: newsdist$(EXEC)

newsdist$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

yconf.tab.c: config.y
	$(YACC) -p yyconf -d -b yconf config.y

yconf.yy.c: config.l yconf.tab.c
	$(LEX) -P yyconf -t config.l > $@ || rm -f $@

clean:
	rm -f *.o *.exe newsdist *.tab.* *.yy.*
