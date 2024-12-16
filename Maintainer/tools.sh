#!/bin/sh
# $Id$

DIRS=`cat Makefile | grep "DIRS = " | sed "s/DIRS = //g"`

list () {
	find $1 "(" -name "*.c" ")" -and -not -name "*.tab.?" -and -not -name "*.yy.?" 2>/dev/null
}

list_header () {
	find $1 "(" -name "*.h" ")" -and -not -name "*.tab.?" -and -not -name "*.yy.?" 2>/dev/null
}
