#!/bin/bash 
#bash script to start a bunch of simulation on different cpus

#$2: path graphs
#$3: coeff wk 
#$4: nb it 
#$5: options
#$6: trace file 
i=0

while [[ i -lt $1 ]]; do

./walking_on_graphs "$2" "$3" "$4" "$5" -d "$6" &
i=$i+1

done 

wait