#this file contains the functions used 
#to process multiple simulation results and create
#the mean of those results 
import numpy as np
import os 

def mean_results(simul_name, res_name):
    """
    str -> file 
    
    reads through every file at the current dir , 
    if they match "simul_name*" attemps to load them 
    generate the numpy matrix corresponding to the mean of all 
    of these results and writes it 
    """
    ret_mat=np.array([])
    nb_file=0
    for i in os.listdir("."):
      
        if simul_name in i:
            if not ret_mat.any():      
                ret_mat= np.loadtxt(i)
            else: 
                ret_mat+= np.loadtxt(i)       
            nb_file+=1
    if ret_mat.any():
        ret_mat/=nb_file
        np.savetxt(res_name, ret_mat)

def clean_results(simul_name):
    """
    str -> 
    
    erases the files matching starting 
    matching "simul_name*" from the 
    currrent directory
    """
    for i in os.listdir("."):
        if simul_name in i :
            os.remove(i)
            
