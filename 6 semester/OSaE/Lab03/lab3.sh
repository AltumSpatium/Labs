#!/bin/bash

if [[ $1 = "-n" ]]
then
	end=$2
	filename=$3
else
	end=10
	filename=$1
fi

result=`cat $filename | sed '/^$/d'`
result=`echo "$result" | sed -n 1,"$end"p`

echo "$result"
