#!/bin/sh
# $Id$
CONFIG="GET"
OLD=`pwd`

. ./configure
make distclean || true
echo "Packing for version $VERSION"
rm -rf /tmp/newsdist-$VERSION
mkdir -p /tmp/newsdist-$VERSION
cp -rf * /tmp/newsdist-$VERSION/
rm -f *.tar.gz
cd /tmp
tar czvf $OLD/newsdist-$VERSION.tar.gz newsdist-$VERSION
rm -rf newsdist-$VERSION
