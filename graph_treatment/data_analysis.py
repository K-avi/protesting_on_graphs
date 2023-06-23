# this file contains the functions to analyze the data produced by the simulation
import os
import numpy as np
import load_trace as lt
import scipy.sparse.csgraph as cg
import argparse as arg

def get_adj_group(node_walker_num_arr, nadj):
    """
    np.array[1D], sp.sparse.csgrap  -> sc.sparse.connected_comp
    
    creates the subgraphs of graph
    representing the groups of walkers
    relies on the csgraph connected
    componnents function
    
    function was rewritten by https://github.com/Pacidus
    """
    N, labels = cg.connected_components(nadj, directed=False)
    uni, idx, inv, count = np.unique(labels, True, True, True)

    # Generation of the new unique labels steadily increasing
    # from 0 to n-1 the number of new graphs. and -1 for the removed nodes
    mask = ~((count == 1) * (node_walker_num_arr[idx] <= 1))
    Nun = (np.cumsum(mask) * mask) - 1

    # reevaluate stuff
    N = mask.sum()

    labels = Nun[inv]
    return N, labels


def spread_gp(nb_gp, labels):
    """
    num , np.array[1D] -> num 
    calculates the spread of the
    node groups
    
    function was improved by https://github.com/Pacidus
    """
    return (labels >= 0).sum() / nb_gp


def size_gp(wlkr_num_arr, nb_gp, labels):
    """
    np.array[1D] , num , np.array[1D] -> num 
    calculates the size of the node groups
    
    function was improved by https://github.com/Pacidus
    """
    return wlkr_num_arr[labels >= 0].sum() / nb_gp


def stat_mobility(wlkr_pos_mat, Nnodes):
    """
    np.array(2D) int -> np.array(1D)
    function was rewritten by https://github.com/Pacidus
    """
    itt, Nw = wlkr_pos_mat.shape
    pos_mat = wlkr_pos_mat.copy() + (np.arange(Nw, dtype="int") * Nnodes)
    u, idx = np.unique(pos_mat.T.flatten(), return_index=True)
    mean_inst = np.bincount(idx % itt, minlength=itt)
    return np.cumsum(mean_inst) / Nw


def mean_results(simul_name, res_name):
    """
    str, str -> writes file

    reads through every file at the current dir ,
    if they match "simul_name*" attemps to load them
    generate the numpy matrix corresponding to the mean of all
    of these results and writes it
    
    function was improved by https://github.com/Pacidus
    """
    ret_mat = np.array([])
    n = 0
    for fname in os.listdir("."):
        if simul_name in fname:
            if n:
                cur = np.loadtxt(fname)
                ret_mat += cur
                del(cur)
            else:
                ret_mat = np.loadtxt(fname)
            n += 1
    if n:
        ret_mat /= n
        np.savetxt(res_name, ret_mat)



def clean_results(simul_name):
    """
    str -> rm files

    erases the files matching starting
    matching "simul_name*" from the
    currrent directory

    function was improved by https://github.com/Pacidus 
    """
    for fname in os.listdir("."):
        if simul_name in fname:
            os.remove(fname)
            

def gen_data_groups(t_curnum, adj, mobility_mean):
    """
    np.array[1D], scipy.sparse.csgraph , np.array[2D] ->
    array[6] 
    nb_group, spread_group , group size, nb lonely wk,
    density of groups (nb walkers / nodes / groups)
    nb occupied nodes
    generates the data related to group for
    one iteration
    
    function was rewritten by https://github.com/Pacidus
    """
    Nitt = t_curnum.shape[0]
    ret = np.zeros((Nitt,7  ))
    ret[:, 3] = mobility_mean
    for itt, cur in enumerate(t_curnum):
        nadj = lt.merge_wknum_adj_mat(cur, adj)
        nb_gp, labels = get_adj_group(cur, nadj)

        ret[itt, 0] = nb_gp
        ret[itt, 1] = spread_gp(nb_gp, labels)
        ret[itt, 2] = size_gp(cur, nb_gp, labels)
        ret[itt, 4] = cur[labels == -1].sum()
        ret[itt, 5] = ret[itt, 2]/ ret[itt, 1]      
        ret[itt, 6] = (cur > 0).sum()
        
    return ret


def adj_from_flux(lines, flux):
    """
    np.array[2D] -> scipy.sparse.csgraph 
    returns the sparse matrix of flux 
    given an array of flux and an 
    array of lines indexed the same way
    (ie : flux[i] is the flux at lines[i])
    """
    
    size = int(lines/2)
    mat = np.array((size,size), dtype=int)
    
    for i,j in enumerate(lines):
        (a,b) = i #i is a line so it SHOULD be of len 2 
        mat[a][b] = j
    
    return ret 

def mean_flux( lines, flux_mat, nb_wk ): 
    """
    np.array[1D] , np.array[2D] , int -> int 
    
    calculates the mean flux of the simul between 0
    and 1 or something idk 
    """
    s = 0
    
    N = len(lines)
    Idab, Idba = np.zeros((2, N//2), dtype=np.int32)
    Dab = dict()
    itt = 0
    for i, (a, b) in enumerate(lines):
        ba = b < a
        if ba:
            a, b = b, a
        if (a, b) not in Dab:
            Dab[(a,b)] = itt
            itt += 1
        if ba:
            
            Idba[Dab[(a,b)]] = int(i)
        else:
            Idab[Dab[(a,b)]] = int(i) 
    

    s =  np.abs(flux_mat[:,Idab] - flux_mat[:,Idba]).sum(1).mean() 
        
    return s/(nb_wk)

def main():
    """
    placeholder; just call 
    analysis on sim name given in stdin
    """
    
    print("parsing arguments")
    parser = args.ArgumentParser(
        prog="sim_analyzer",
        description="analyzes the results of an already already ran \
        simulation"
    )
    return 0
    
    
if __name__=='__main__':
    main()