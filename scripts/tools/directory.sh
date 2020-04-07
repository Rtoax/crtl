#!/bin/bash

function crtl_chk_dir {

	if [ $# -lt 1 ]; then
	    echo -e "\033[31mERROR: Need argumet\033[m"
	    return 1
	else
	if [ -d $1 ]; then
		echo "Directory $1 is OK!"
		return 0
	else
        echo -e "\033[31mERROR: Directory $1 not exist.\033[m"
		return 1
	fi
fi
}




