#!/bin/bash 
#bash script to start a bunch of simulation on different cpus

#$2: path graphs
#$3: coeff wk 
#$4: nb it 
#$5: options
#$6: trace file 
i=0
echo "what $1"
while [[ i -lt $1 ]]; do
echo "hey $i"
./walking_on_graphs "$2" "$3" "$4" $5 -d "$6$i" & 
sleep 1
#$5 isnt quoted cuz I want the C program to read it as more than 1 argument
#this might be dangerous idk
i=$((i+1))

done 

wait