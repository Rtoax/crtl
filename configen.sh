#!/bin/bash
# rongtao

CONFIG_FILE=config.h
CONFIG_LIB_NAME=LibCRTL
CONFIG_LIB_CHK_FILE=config/configlibchk.txt
CONFIG_HDR_CHK_FILE=config/confighdrchk.txt

CONFIG_USR_INCLUDE=/usr/include
CONFIG_USR_LOCAL_INCLUDE=/usr/local/include

function config_header()
{
	echo -e "/* This is $CONFIG_LIB_NAME config.h file, `date` */\n" >> $CONFIG_FILE

	echo -e "\n/* LibCRTL Name */"	>> $CONFIG_FILE
	echo -e "#define LIBCRTL_NAME \"Libcrtl\""	>> $CONFIG_FILE
	
	echo -e "\n/* LibCRTL Author */"	>> $CONFIG_FILE
	echo -e "#define LIBCRTL_AUTHOR \"Koma Rong\""	>> $CONFIG_FILE
	
	echo -e "\n/* LibCRTL Version Info */"	>> $CONFIG_FILE
	echo -e "#define LIBCRTL_VERSION_MAJOR 1"	>> $CONFIG_FILE
	echo -e "#define LIBCRTL_VERSION_MINOR 0" 	>> $CONFIG_FILE
	echo -e "#define LIBCRTL_VERSION_PATCH 0" 	>> $CONFIG_FILE
	echo -e "\n"	>> $CONFIG_FILE
}


function config_libchk()
{
	if [ $# -lt 1 ]; then
		return 1
	fi
	
	libso=`echo ${1%.*}| xargs| tr '\t' ' ' | tr '\n' ' '| sed 's/[[:space:]]//g'`
	libname=`echo $libso| sed "s/[^[:alnum:]]/_/g" | tr '[a-z]' '[A-Z]'` #libssl.so -> LIBSSL
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
	hdr=`echo $1| xargs| tr '\t' ' ' | tr '\n' ' ' | sed 's/[[:space:]]//g'`
	hdrmacro="HAVE_`echo $hdr | sed "s/[^[:alnum:]]/_/g"  | tr '[a-z]' '[A-Z]'`" #sys/socket.h -> HAVE_SYS_SOCKET_H

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
	libs=`cat $CONFIG_LIB_CHK_FILE| xargs | tr '\t' ' ' | tr '\n' ' '`
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
	hdrs=`cat $CONFIG_HDR_CHK_FILE| xargs | tr '\t' ' ' | tr '\n' ' '`
	for hdr in $hdrs; do
		config_hdrchk $hdr
	done
}


function config_help()
{
	echo  "Usage:"
	echo  "  -f config :  生成config.h 文件"
	echo  "  -h header :  对 $CONFIG_HDR_CHK_FILE 进行排序"
	echo  "  -l library:  对 $CONFIG_LIB_CHK_FILE 进行排序"
}

if [ $# == 0 ]; then
	echo  "$0 "
	config_help
	exit 1
fi

case $1 in
-f|config)
	#删除已存在的 config.h 文件
	rm -f $CONFIG_FILE
	#生成 config.h 文件描述
	config_header
	#查询 config.h 中的依赖头文件
	config_hdrchkall
	#查询 config.h 中的依赖库
	config_libchkall
;;
-h|header)
	cat $CONFIG_HDR_CHK_FILE | xargs | tr ' ' '\n' | sort | uniq > $CONFIG_HDR_CHK_FILE
;;
-l|library)
	cat $CONFIG_LIB_CHK_FILE | xargs | tr ' ' '\n' | sort | uniq > $CONFIG_LIB_CHK_FILE
;;
*)
	config_help
;;
esac



