#this file contains the functions to analyze the data produced by the simulation
#WARNING: the functions are currently prototypes / placeholders; do NOT use them 
import numpy as np
import networkx as nx
from statistics import mean

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


def load_csv_dict_graph(path):
    """
    str -> dict_graph
    
    creates a dict of the graph rep 
    contained at path (assuming the 
    graph is stored as the custom csv rep)
    """
    
    dict_graph= dict()
    
    with open (path, "r") as file: 
        next(file)
        for line in file:
            node_from=int(line.split(",")[0])   
            nodes_to=line.split(",")[2].split(";")
            line_array=[]
            for i in nodes_to:  
                node_to = int(i.split(":")[0])
                line_array.append(node_to)
            dict_graph.update({node_from:[line_array,0]})
    file.close
    
    return dict_graph

def merge_wkpos_dictgraph( walker_pos_row , dict_graph): 
    """
    np.array , dict_graph -> dict_graph
    updates the nb_elem stored in the dict graph given a walker pos_row
    
    O(nb_walkers)
    """
    
    for i in walker_pos_row:
        dict_graph[i][1]+=1
    
    return dict_graph


def get_adj_group( dict_graph ):
    """
    dict_graph -> group_array
    
    creates the subgraphs of graph
    representing the groups of walkers
    
    
    should be like a normal graph search except u generate 
    a bunch of smaller dict that are like subgraphs of dict_graph where 
    each node has at least 1 guy and is linked to another one 
    
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


def main():
    """
    count_groups(["0"])
    spreading_groups(["0"])
    #merge_wkpos_dictgraph([],[])
    get_adj_group(["0"])
    
    get_mean_group_size(["0"])
    get_mean_nodes_visited(["0"])
    """
    graph_dict = load_csv_dict_graph("../test_graph/gt_test3.csv")
    test_arr= np.concatenate( (np.ones(2,dtype=int) , np.zeros(2, dtype=int)) )
    merge_wkpos_dictgraph( test_arr  , graph_dict)
    print(graph_dict)
    group_array = get_adj_group(graph_dict)
    
    return 0

if __name__=='__main__':
    main()