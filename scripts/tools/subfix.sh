#!/bin/bash


# Rong tao 2019.09.20
# 获取该路径下的所有 扩展名 的文件（带相对路径）
# 例：
#	crtl_subfix_files sh
#	a.sh
#	b.sh ...
function crtl_subfix_files()
{
	if [ $# -lt 1 ]; then
		echo "usage: $0 [findSubfix]"
		return
	fi
    local list
    local path
    if [ $# -eq 2 ]; then
        list=$(ls $2)
        path="$2/"
    else
        list=$(ls)
        path=""
    fi
    
	for f in $list; do
    
		if [ -d $path$f ]; then
			crtl_subfix_files $1 $path$f
            
		elif [ -f $path$f ]; then
            # rongtao.c -> c; get rongtao.c 's subfix
			local _subfix=${f##*.} 
			if [ $_subfix = $1 ]; then
				echo $path$f
			fi
		fi
	done
}



