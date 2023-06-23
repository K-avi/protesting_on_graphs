#!/bin/bash

python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 1 1 1 100 "propu:1"
wait 
mv res_mean res_100_propu
mv res_meanfluxmean flux_propu

echo "done noisy attra , starting attra"
python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 1 1 1 100 "attra:4 rand:1"
wait 
mv res_mean res_100_paris_noisy_attra
mv res_meanfluxmean flux_noisy_attra
echo "done noisy attra , starting attra"

python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 1 1 1 100 "attra:1"
wait
mv res_mean res_100_paris_attra

mv res_meanfluxmean flux_attra
echo "done attra starting noisy align"

python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 1 1 1 100 "align:4 rand:1"
wait 
mv res_mean res_100_paris_noisy_align

mv res_meanfluxmean flux_noisy_align
echo "done noisy align starting align" 

python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 1 1 1 100 "align:1"
wait
mv res_mean res_100_paris_align_

mv res_meanfluxmean flux_align
echo "done align starting attra coeff" 

python3 ../graph_treatment/autorun_simul.py 48.8667 2.3333 5000  10 graph_paris.csv res_file_align_bigsim 1 1 1 100 "attco:1"
wait
mv res_mean res_100_paris_attco
mv res_meanfluxmean flux_attco
