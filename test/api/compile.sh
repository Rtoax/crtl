#!/bin/bash
if [ $# -lt 1 ]; then
	echo "Usage: $0 source.c must be input"
	exit
fi

cd ..
./install_crtl.sh 
cd -

echo "Compile >> $*"
gcc $* `pkg-config --libs libcrtl`

