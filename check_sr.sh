#!/bin/bash
# Check Script for SR Rdt Protocol
appname='./bin/sr'
inputname='./input.txt'
outputname='./output.txt'
resultname='./result.txt'

for ((i=1;i<=10;i++))
do
echo Test $appname $i
./$appname > $resultname 2>&1
cmp $inputname $outputname
echo Test $i over
done
