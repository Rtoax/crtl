#! /bin/bash

all_cpp_files=`ls *.c`
exclude_files="exercise_1_07.cpp exercise_1_15.cpp"

is_exclude_file() {
	for file in $exclude_files; do
		if [ "$file" = "$1" ]; then
			return 0
		fi
	done

	return 1
}

main() {
	for cpp_file in $all_cpp_files; do
		exe_file=${cpp_file%%.c*}

		if [ "$1" == "clear" ]; then
			rm -f $exe_file
			continue
		fi

		if is_exclude_file $cpp_file; then
			continue
		fi

		if [ $exe_file -nt $cpp_file ] && [ "$1" != "rebuild" ]; then
			continue
		fi

		echo "[BUILDING] $cpp_file -> $exe_file"
		gcc $cpp_file -o $exe_file `pkg-config --libs libcrtl`  -I ../../

		if [ "$?" != "0" ]; then
			return 1
		fi
	done

	return 0
}

main $1

exit $?

