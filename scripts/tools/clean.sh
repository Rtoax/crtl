#!/bin/bash
# Rong Tao 2018.1.16

function crtl_clean(){
	filename=($*)
	for name in ${filename[@]}; do
	    #echo $name
	    find  .  -name  $name  -type  f  -print  -exec  rm  -rf  {} \;
	done

	if [ $# -eq 0 ];then
	    echo $*
	    pwd
	fi
}
