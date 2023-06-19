#!/bin/bash

python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 8 8 1 100 "align:4 rand:1"
wait 
mv res_mean res_mean_noisy_align

