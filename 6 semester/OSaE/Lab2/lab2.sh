#!/bin/bash
K=3
M=100
X=`date +%s`

while true
do
	X=$(( (K * X) % M ))
	echo $X >> rnd
	sleep 10
done &

