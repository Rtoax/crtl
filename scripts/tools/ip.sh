#!/bin/bash

function crtl_ip_ip2num(){
#Rong Tao 2019.06.12
	if [ $# -lt 1 ]; then
	    echo -e "\033[31mERROR: Need argumet\033[m"
	    return 1
	fi

	local ip=$1
	local a=$(echo $ip | awk -F '.' '{print $1}')
	local b=$(echo $ip | awk -F '.' '{print $2}')
	local c=$(echo $ip | awk -F '.' '{print $3}')
	local d=$(echo $ip | awk -F '.' '{print $4}')
	echo "$(( (a<<24)+(b<<16)+(c<<8)+d ))"
}

function crtl_ip_num2ip(){
#Rong Tao 2019.06.12
	if [ $# -lt 1 ]; then
	    echo -e "\033[31mERROR: Need argumet\033[m"
	    return 1
	fi
	
	local num=$1
	local a=$((num>>24))
	local b=$((num>>16&0xff))
	local c=$((num>>8&0xff))
	local d=$((num&0xff))
	echo "$a.$b.$c.$d"
}
#Demo
#num2ip `ip2num 10.170.6.66`
#echo `ip2num 10.170.6.66`

