#!/bin/bash
if [ $# -lt 1 ]; then
	echo "Usage: $0 source.c must be input"
	exit
fi

 ../install_crtl.sh > /dev/null

echo "Compile >> $*"
gcc $* -lcrtl -lcmocka `pkg-config --libs libcrtl`

