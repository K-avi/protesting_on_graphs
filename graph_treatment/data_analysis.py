#this file contains the functions to analyze the data produced by the simulation
#WARNING: the functions are currently prototypes / placeholders; do NOT use them 
import numpy as np
from statistics import mean
import os
import scipy.sparse.csgraph as cg

def get_adj_group(node_walker_num_arr, nadj):
    """
    np.array[1D]  , adj_matrix -> cg.sparse.csgraph
    
    creates the subgraphs of graph
    representing the groups of walkers 
    relies on the csgraph connected
    componnents function
    fast
    """
    N, labels = cg.connected_components(nadj, directed=False, return_labels=True)
    n = 0
    n_labels = []
    alone = 0
    nb_wk=0
    for i in range(N):
        mask = (labels == i)
        if node_walker_num_arr[mask].sum() > 1:
            n_labels += [n] * mask.sum()
            n += 1
            nb_wk += node_walker_num_arr[mask].sum()
   
    return n, np.array(n_labels) , len(labels)-nb_wk

    

def count_groups(adj_mat):
    """
    cg sparce matrix -> num
    
    returns the number of walker
    groups 
    
    O(1)
    """
    a,b,n = adj_mat
    return a

def spreading_groups(adj_mat): 
    """
    cg sparce matrix -> num
    
    returns the mean spread
    (number of nodes occupied)
    of the groups of walkers in a 
    group array
    
    O(1)
    """
    a,b,n = adj_mat
    v,c = np.unique(b , return_counts=True)

    return sum(c)/len(c)


def get_mean_group_size(nb_wk, adj_mat):
    """
    cg sparce matrix -> num
    
    given an array containing every group
    of walkers in a simulation 
    calculates the mean of the nb of 
    walkers present in the groups
    """
    a,b,n = adj_mat
    print("nbwk, a", nb_wk,a)
    return nb_wk/a


def stat_mobility(wlkr_pos_mat):
    """
    np.array(2D) -> np.array(1D)
    """
    itt, Nw = wlkr_pos_mat.shape
    N_visit = np.zeros(wlkr_pos_mat.shape)
    for i, wlkr in enumerate(wlkr_pos_mat.T):
        u, idx = np.unique(wlkr, return_index=True)
        n = u.size
        R = np.arange(1, n + 1)
        idx = np.sort(idx)
        ntime = list(np.diff(idx))
        ntime.append(itt - idx[-1])
        N_visit[:, i] = np.repeat(R, ntime)

    return N_visit.mean(1)


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
    del(ret_mat)

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
