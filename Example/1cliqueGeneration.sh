#!/bin/bash
if [ $# -ne 3 ]
  then
    echo "This script needs 3 argument: TotalNodes LeftNodeSize PartitionType"
		echo "Constraints: 0<LeftNodeSize<TotalNodes && PartitionType in {1,2,3}"
    exit -1
fi
if [ $3 ]
then
	partition=$3
else
	partition="1"
fi
N=$1
gauche=$2
overlap=$(( ($N - $gauche) ))

case "$partition" in
	'1')
		p1=1
		p2=1
		p3=1
	;;
	'2')
		p1=1
		p2=1
		p3=2
	;;
	'3')
		p1=1
		p2=2
		p3=3
	;;
	*)
		p1=1
		p2=1
		p3=1
	;;
esac


if [[ $gauche -gt 1 ]]
	then
	for i in $(seq 1 $(($gauche - 1)))
	do
		for j in $(seq $(($i + 1)) $gauche )
		do
			echo $i $j $p1 # Left part of the complete graph
		done
	done
fi


for i in $(seq 1 $gauche)
do
	for j in $(seq $(( $gauche + 1)) $(($gauche + $overlap)))
	do
		echo $i $j $p2 # Center part of the complete graph
	done
done

if [[ $overlap -gt 1 ]]
	then
	for i in $(seq $(( $gauche + 1)) $(($gauche + $overlap - 1)))
	do
		for j in $(seq $(($i + 1)) $(($gauche + $overlap)) )
		do
			echo $i $j $p3 # Right part of the complete graph
		done
	done
fi
