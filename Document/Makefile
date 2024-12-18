# $Id$

MD = sed -E 's/Note: /\n   Note: /g' | sed "s/@DATE@//g" | sed -E 's/[ \t]+$$//g' | tr "'" "\`"
PLAIN = sed -E 's/^\# *//g' | sed "s/@DATE@/`date +'%b %Y'`/g" | sed 's/```//g' | fold -s

.PHONY: all

all: ../README ../README.build ../README.md ../README.build.md

../README: README.in
	cat README.in | $(PLAIN) > $@

../README.md: README.in
	cat README.in | $(MD) > $@

../README.build: README.build.in
	cat README.build.in | $(PLAIN) > $@

../README.build.md: README.build.in
	cat README.build.in | $(MD) > $@
