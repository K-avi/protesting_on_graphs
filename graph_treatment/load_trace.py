#file containing the functions to load the traces of a simulation
import  numpy as np
import sys
import os

def load_trace_elem( path, nb_it ):
    """
    reads the content of the binary file stored at path 
    as a matrix of nb_elem*nb_int 32 bytes integers 
    and stores it in a numpy matrix
    """
    dt_arr= np.fromfile(path, dtype=np.int32)
    return dt_arr.reshape( int(nb_it), -1)

def load_dict_graph(path):
    """
    str -> dict_graph
    
    creates a dict of the graph rep 
    contained at path (assuming the 
    graph is stored as the custom csv rep)
    really ugly
    """
    
    dict_graph= dict()
    
    with open (path, "r") as file: 
    
        for line in list(file):
           
            if len(line.strip())!=0:
                node_from=int(line.split(",")[0]) 
                if int(line.split(",")[1]) > 0: 
                    nodes_to=line.split(",")[2].split(";")
                    line_array=[]
                    for i in nodes_to:  
                        node_to = int(i.split(":")[0])
                        line_array.append(node_to)
                    dict_graph.update({node_from:[line_array,0]})
                else :
                    dict_graph.update({node_from:[[],0]})
    file.close
    return dict_graph


def merge_wkpos_row_dictgraph( walker_pos_row , dict_graph): 
    """
    np.array , dict_graph -> dict_graph
    updates the nb_elem stored in the dict graph given a walker pos_row
    complexity is terrible; will have to make this good at some point
    
    deprecated
    
    O(nb_walkers* nb_gentry)
    """
    for j in dict_graph: 
        dict_graph[j][1]=0
        
    for i in walker_pos_row:
        dict_graph[i][1]+=1

    return dict_graph

def merge_wknum_row_dictgraph( node_walker_num_arr , dict_graph):
    """
    updates the num of walkers at each entry of dict_graph
    with a row of the walker trace matrix 
    """
  
    for i in range(0, len(node_walker_num_arr)):
        if not i in dict_graph: 
            print("missing key: ," ,i)
        dict_graph[i][1]=node_walker_num_arr[i]
    
    return dict_graph


def load_trace(trace_name, nb_it):
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
  
    t_curnum = load_trace_elem( trace_name+"_curnum", nb_it)
    t_wkpos = load_trace_elem( trace_name+"_wkpos", nb_it)
    t_graph = load_dict_graph(trace_name+"_hr")
    
    return (t_curnum,t_wkpos, t_graph)

def clean_trace(trace_name):
    os.remove(trace_name+"_curnum")
    os.remove(trace_name+"_flux")
    os.remove(trace_name+"_wkpos")
    os.remove(trace_name+"_hr")
    