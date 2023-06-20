# this file contains the functions to analyze the data produced by the simulation
# WARNING: the functions are currently prototypes / placeholders; do NOT use them
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
    N, labels = cg.connected_components(nadj, directed=False)
    uni, idx, inv, count = np.unique(labels, True, True, True)

    # Generation of the new unique labels steadily increasing
    # from 0 to n-1 the number of new graphs. and -1 for the removed nodes.
    mask = ~((count == 1) * (node_walker_num_arr[idx] <= 1))
    Nun = (np.cumsum(mask) * mask) - 1

    # reevaluate stuff
    N = mask.sum()

    labels = Nun[inv]
    return N, labels,(node_walker_num_arr[labels==-1]).sum()


def spreading_groups(adj_mat):
    """
    cg sparce matrix -> num

    returns the mean spread
    (number of nodes occupied)
    of the groups of walkers in a
    group array

    O(1)
    """
    a, b ,s= adj_mat
    v, c = np.unique(b, return_counts=True)
    trash_gp_size = np.count_nonzero(b == -1)
    return (sum(c) - trash_gp_size) / (a)


def get_mean_group_size(wlkr_num_arr, adj_mat):
    """
    cg sparce matrix -> num

    given an array containing every group
    of walkers in a simulation
    calculates the mean of the nb of
    walkers present in the groups
    """
    a, b,s = adj_mat
    return wlkr_num_arr[b >= 0].sum() / a


def stat_mobility(wlkr_pos_mat, Nnodes):
    """
    np.array(2D) -> np.array(1D)
    """
    itt, Nw = wlkr_pos_mat.shape
    pos_mat = wlkr_pos_mat.copy()
    pos_mat += np.arange(Nw) * Nnodes
    u, idx = np.unique(pos_mat, return_index=True)
    mean_inst = np.bincount(idx % itt, minlength=itt) / Nw
    return np.cumsum(mean_inst)


def mean_results(simul_name, res_name):
    """
    str -> file

    reads through every file at the current dir ,
    if they match "simul_name*" attemps to load them
    generate the numpy matrix corresponding to the mean of all
    of these results and writes it
    """
    ret_mat = np.array([])
    nb_file = 0
    for i in os.listdir("."):

        if simul_name in i:
            if not ret_mat.any():
                ret_mat = np.loadtxt(i)
            else:
                ret_mat += np.loadtxt(i)
            nb_file += 1
    if ret_mat.any():
        ret_mat /= nb_file
        np.savetxt(res_name, ret_mat)
    del ret_mat


def clean_results(simul_name):
    """
    str ->

    erases the files matching starting
    matching "simul_name*" from the
    currrent directory
    """
    for i in os.listdir("."):
        if simul_name in i:
            os.remove(i)
