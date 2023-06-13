#this file contains the functions to analyze the data produced by the simulation
#WARNING: the functions are currently prototypes / placeholders; do NOT use them 
import numpy as np
import networkx as nx

def count_groups(group_array):
    """
    group_array -> num
    
    returns the number of walker
    groups 
    
    O(1)
    """
    return len(group_array)

def spreading_groups(group_array): 
    """
    group_array -> num
    
    returns the mean spread
    (number of nodes occupied)
    of the groups of walkers in a 
    group array
    
    O(1)
    """
    return mean([len(i) for i in group_array])


def load_csv_set_graph(path):
    """
    str -> set_graph
    
    creates a set of the graph rep 
    contained at path (assuming the 
    graph is stored as the custom csv rep)
    """
    return 0

def get_adj_group(walker_pos_mat , graph ):
    """
    np.array , set_graph -> group_array
    
    creates the subgraphs of graph
    representing the groups of walkers
    
    O(?)
    """
    
    return 0

def get_mean_group_size(group_array):
    """
    group_array -> num
    
    given an array containing every group
    of walkers in a simulation 
    calculates the mean of the nb of 
    walkers present in the groups
    
    O(group_array*mean(nb_elem_in_groups)) (not sure about that)
    """
    return mean([sum(i) for i in group_array])

def get_mean_nodes_visited(walker_pos_mat): 
    """
    np.array -> num
    
    given a numpy matrix of nb_iterations * nb_walker dimensions
    calculates the mean of the number of distinct nodes visited by the walkers 
    during the simulation
    
    O(i*w)
    """
    val_list=[]
    for column in walker_pos_mat.T:
        nb_diff_nodes=0
        i=set()
        for elem in column:
            if not elem in i:
               nb_diff_nodes+=1
               i.add(elem)
        val_list.append(nb_diff_nodes)
    
    return mean(val_list)

