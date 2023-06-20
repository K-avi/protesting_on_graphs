# file containing the functions to load the traces of a simulation
import numpy as np
import scipy.sparse as sp
import os


def load_trace_elem(path, nb_it):
    """
    reads the content of the binary file stored at path
    as a matrix of nb_elem*nb_int 32 bytes integers
    and stores it in a numpy matrix
    """
    dt_arr = np.fromfile(path, dtype=np.int32)
    return dt_arr.reshape(int(nb_it), -1)


def load_adj_mat(path):
    """
    str -> sp.sparse.csgraph

    creates a dict of the graph rep
    contained at path (assuming the
    graph is stored as the custom csv rep)
    
    function was rewritten by https://github.com/Pacidus
    """

    with open(path, "r") as file:
        text = file.readlines()
        nv = len(text)
        ne = 0
        col = []
        row = []
        for line in text:
            match line.split(","):
                case [node, *edges]:
                    ne += len(edges)
                    col += [int(node)] * len(edges)
                    row += map(int, edges)
                case _:
                    nv -= 1
    return sp.csr_array((np.ones(ne), (col, row)), dtype="int8")


def merge_wknum_adj_mat(node_walker_num_arr, adj_mat):
    """
    updates a sp.sparse.csgraph representing an adj mat by 
    removing the nodes w/o walker
    
    function was rewritten by https://github.com/Pacidus
    """
    nadj = adj_mat.copy()
    mask = node_walker_num_arr > 0
    nadj *= mask
    nadj *= nadj.transpose()
    nadj.eliminate_zeros()
    return nadj


def load_trace(trace_name, nb_it, query=[]):
    """
    loads the formatted trace files corresponding to the trace name
    given as argument from the current directory
    and returns it as a tuple of numpy matrixes.
    the first element of the tuple is a nb_iteration * nb_nodes matrix
    containing the evolution of the number of walkers at each node during the simulation
    the second one is a nb_iteration * nb_lines matrix containing the evolution
    of the flux stored at each line during the simulation
    the last one is a  nb_iteration * nb_walkers matrix
    containing the evolution of the position of each walker during the simulation
    """

    t_curnum = load_trace_elem(trace_name + "_curnum", nb_it)
    t_flux = load_trace_elem(trace_name + "_flux", nb_it)
    t_wkpos = load_trace_elem(trace_name + "_wkpos", nb_it)
    graph = load_adj_mat(trace_name + "_hr")

    return (t_curnum, t_flux, t_wkpos, graph)


def clean_trace(trace_name):
    os.remove(trace_name + "_curnum")
    os.remove(trace_name + "_flux")
    os.remove(trace_name + "_wkpos")
    os.remove(trace_name + "_hr")
