# $Id$

.PHONY: all

all: ../README ../README.build

../README: README.in
	cat README.in | sed "s/@DATE@/`date +'%b %Y'`/g" > $@

../README.build: README.build.in
	cat README.build.in | sed "s/@DATE@/`date +'%b %Y'`/g" > $@
