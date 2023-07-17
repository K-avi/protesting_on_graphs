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


def load_line_trace(path, nb_lines):
    """
    str , int -> np.array[2D]
    
    loads the human readable 
    trace for the lines 
    (used to build adj mat with flux)
    """
    arr = np.loadtxt(path, dtype=np.int32)
    return arr.reshape(int(nb_lines), -1 )

def load_nbwk_nblines( path, coeff_wk):
    """
    simple helper fn to read nbwk 
    and nblines
    """
    with open(path,"r") as f: 
        line = f.readline()
      
        a,b = line.split(",")[0:2]
        return (int(a)*coeff_wk,int(b))

def load_adj_mat(path):
    """
    str -> sp.sparse.csgraph

    creates a sparse csgraphs matrix
    of the graph stored at path
    
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

def clean_trace(trace_name):
    os.remove(trace_name + "_curnum")
    #os.remove(trace_name + "_wkpos")
    os.remove(trace_name + "_hr")
    os.remove(trace_name + "_hrend")
   
    #os.remove(trace_name + "_wkend")
    
    
def clean_var(trace_name):
    os.remove(trace_name + "_flux")
    os.remove(trace_name + "_lines")
  
