#this file contains the functions to analyze the data produced by the simulation
#WARNING: the functions are currently prototypes / placeholders; do NOT use them 
import numpy as np
from statistics import mean
import os


def is_in_group(dict_graph , entry):
    """
    dict_graph , num -> boolean
    
    returns True if the node 'entry' is part of a group 
    false otherwise 
    """
    return (sum([dict_graph[i][1] for i in dict_graph[entry][0]])+dict_graph[entry][1])>=2

def modified_bfs(visited, dict_graph, node): #function for BFS
    """
    set , array , dict_graph , num -> set_nodes
    
    returns the group that node is a part of if
    it hasn't been explored already
    
    O(?)
    """
    if node in visited: 
        return (visited, set())
    visited.add(node)
    
    if dict_graph[node][1]==0 or not is_in_group(dict_graph, node) :
        return (visited,set())
    
    queue=[node]
    a=0
    set_nodes={ (node,dict_graph[node][1])}
    while queue: # Creating loop to visit each node
        m = queue.pop(0) 
        #print(queue)
        for neighbor in dict_graph[m][0]:
            if neighbor not in visited and dict_graph[neighbor][1]>0:
                #print(a, neighbor)
                a+=1
                queue.append(neighbor)
                set_nodes.add( (neighbor,dict_graph[neighbor][1]))
           
            visited.add(neighbor)
    return (visited,set_nodes )

def get_adj_group( dict_graph ):
    """
    dict_graph -> group_array
    
    creates the subgraphs of graph
    representing the groups of walkers
    
    
    should be like a normal graph search except u generate 
    a bunch of smaller dict that are like subgraphs of dict_graph where 
    each node has at least 1 guy and is linked to another one 
    
    relies on BFS cuz I think it's cooler than DFS
    
    O(?)
    """
    
    visited= set()
    group_array=[]
   
    for m in dict_graph:
        (visited_new , set_graph)= modified_bfs(visited,  dict_graph , m)
        visited = visited | visited_new
        
        if set_graph:
            group_array.append(set_graph)
    del(visited)
    return group_array

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


def get_mean_group_size(group_array):
    """
    group_array -> num
    
    given an array containing every group
    of walkers in a simulation 
    calculates the mean of the nb of 
    walkers present in the groups
    
    O(group_array*mean(nb_elem_in_groups)) (not sure about that)
    """
    return sum([ (j[1]) for i in group_array for j in i])/len(group_array)

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
