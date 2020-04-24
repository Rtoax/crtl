#!/bin/bash
# rongtao

CONFIG_FILE=config.h
CONFIG_LIB_NAME=LibCRTL
CONFIG_LIB_CHK_FILE=configlibchk.txt
CONFIG_HDR_CHK_FILE=confighdrchk.txt

CONFIG_USR_INCLUDE=/usr/include
CONFIG_USR_LOCAL_INCLUDE=/usr/local/include

function config_header()
{
	echo -e "/* This is $CONFIG_LIB_NAME config.h file, `date` */\n" >> $CONFIG_FILE
}


function config_libchk()
{
	if [ $# -lt 1 ]; then
		return 1
	fi
	
	libso=$1
	libname=`echo ${libso%.*} | tr '[a-z]' '[A-Z]'` #libssl.so -> LIBSSL
	libmacro="HAVE_$libname"
	echo -e "/* The $libname library */" >> $CONFIG_FILE
	ldconfig -p | grep $libso 2>&1 1>/dev/null
	if [ $? == 0 ]; then
		echo -e "#define $libmacro 1\n" >> $CONFIG_FILE
	else
		echo -e "#define $libmacro 0\n" >> $CONFIG_FILE
	fi

	return 0
}

function config_hdrchk()
{
	hdr=$1
	hdrmacro="HAVE_`echo $hdr | sed "s/[^[:alnum:]]/_/g" | sed -e 's/^[ \t]*//g'| sed 's/[[:space:]]//g' | tr '[a-z]' '[A-Z]'`" #sys/socket.h -> HAVE_SYS_SOCKET_H

	#现在 /usr/include 中查找， 再去 /usr/local/include 查找
	echo -e "/* The $hdr header */" >> $CONFIG_FILE
	if [ -f $CONFIG_USR_INCLUDE/$hdr ]; then
		echo -e "#define $hdrmacro 1\n" >> $CONFIG_FILE
	else
		if [ -f $CONFIG_USR_LOCAL_INCLUDE/$hdr ]; then
			echo -e "#define $hdrmacro 1\n" >> $CONFIG_FILE
		else
			echo -e "#define $hdrmacro 0\n" >> $CONFIG_FILE
		fi
	fi
	
	return 0
}

function config_libchkall()
{
	if [ ! -f $CONFIG_LIB_CHK_FILE ]; then
		echo "Not exist $CONFIG_LIB_CHK_FILE"
		return 1
	fi
	libs=`cat $CONFIG_LIB_CHK_FILE`
	for lib in $libs; do
		config_libchk $lib
	done
}


function config_hdrchkall()
{
	if [ ! -f $CONFIG_HDR_CHK_FILE ]; then
		echo "Not exist $CONFIG_HDR_CHK_FILE"
		return 1
	fi
	hdrs=`cat $CONFIG_HDR_CHK_FILE`
	for hdr in $hdrs; do
		config_hdrchk $hdr
	done
}

#删除已存在的 config.h 文件
rm -f $CONFIG_FILE

#生成 config.h 文件描述
config_header

#查询 config.h 中的依赖头文件
config_hdrchkall

#查询 config.h 中的依赖库
config_libchkall


