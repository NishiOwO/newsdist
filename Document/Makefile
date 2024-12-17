# $Id$

.PHONY: all

all: ../README ../README.build ../README.md

../README: README.in
	cat README.in | sed -E 's/^# *//g' | sed "s/@DATE@/`date +'%b %Y'`/g" | fold -s > $@

../README.md: README.in
	cat README.in | sed "s/@DATE@//g" | sed -E 's/[ \t]+$$//g' | fold -s > $@

../README.build: README.build.in
	cat README.build.in | sed -E 's/^# *//g' | sed "s/@DATE@/`date +'%b %Y'`/g" | fold -s > $@
