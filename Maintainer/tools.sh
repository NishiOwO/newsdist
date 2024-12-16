#!/bin/sh
# $Id$

DIRS=`cat Makefile | grep "DIRS = " | sed "s/DIRS = //g"`

list () {
	find $1 "(" -name "*.c" -or -name "*.h" ")" -and -not -name "*.tab.?" -and -not -name "*.yy.?" 2>/dev/null
}
