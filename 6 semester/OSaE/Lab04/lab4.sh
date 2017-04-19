#!/bin/bash

reg=$1
dir=`pwd`
total=0

print_file() {
	local fullname="$1"
	empty_directory=`echo $fullname | sed -n '/^.*\*$/='`
	if [[ $empty_directory = '' ]]
	then
		file_size=$(stat "$fullname" -c %s)
		let total=$total+$file_size
		echo "$fullname ------ Size: $file_size"
	fi
}

scan() {
	for e in "$1"/*; do
		if [[ -d "$e" ]]
		then
			scan "$e"
		else
			needed_file=`echo $e | sed -n "/$reg/="`
			if [[ $needed_file != '' ]]
			then
				print_file "$e"
			fi
		fi
	done
}

scan "$dir"
echo "Total: $total"
