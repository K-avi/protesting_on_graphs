#!/bin/bash 
#bash script to start a bunch of simulation on different cpus

#$2: path graphs
#$3: coeff wk 
#$4: nb it 
#$5: options
#$6: trace file
#$7: start of flux dump 
i=0
while [[ i -lt $1 ]]; do
./walking_on_graphs "$2" "$3" "$4" $5 -d "$6$i" -l "$7" -v "0" & 
sleep 0.5
#$5 isnt quoted cuz I want the C program to read it as more than 1 argument
#this might be dangerous idk
i=$((i+1))

done 

wait
