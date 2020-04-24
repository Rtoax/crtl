#!/bin/bash
# rongtao

CONFIG_FILE=config.h



function config_header()
{
	echo -e "/* This is LibCRTL config.h file, `date` */\n" >> $CONFIG_FILE
}


function config_libchk()
{
	libname=$1
	libso=$2
	libmacro=$3
	echo -e "/* The $libname library */" >> $CONFIG_FILE
	ldconfig -p | grep $libso 2>&1 1>/dev/null
	if [ $? == 0 ]; then
		echo -e "#define $libmacro 1\n" >> $CONFIG_FILE
	else
		echo -e "#define $libmacro 0\n" >> $CONFIG_FILE
	fi
}

function config_hdrchk()
{
	hdr=$1
	hdrname=$2
	hdrmacro=$3
}


rm -f $CONFIG_FILE

config_header
config_libchk cURL libcurl.so HAVE_LIBCURL
config_libchk CSTL libcstl.so HAVE_LIBCSTL
config_libchk libUV libuv.so HAVE_LIBUV


