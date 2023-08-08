#!/bin/python3.10
#this is the file doing to the retrieval/ treatment of city graphs using OSMNX and networkx
#import osmnx as ox
import networkx as nx
import numpy as np

def discretisePA(Graph, step): 
    """
    nxGraph, step -> graph_string
    discretises a graph passed as arg with the step
    i.e : creates new nodes when the distance between the nodes a,b in a line 
    is bigger than step  
    returns new graph
    function was rewritten by https://github.com/Pacidus
    """ 
    N = Graph.number_of_nodes()
    lengths = Graph.edges(data="length")
    
    id_nodes = {j: i for i,j in enumerate(Graph.nodes())}
    al = [""] * N
    gd = [0] * N
    Ne = Graph.number_of_edges()
    for a, b, length in lengths:
        ns = int(float(length)/step)
        Ne += ns
        new_ed = [id_nodes[a], *list(range(N, N + ns)), id_nodes[b]]
        al += [""] * ns
        gd += [2] * ns
        gd[id_nodes[a]] += 1
        gd[id_nodes[b]] += 1
        for i in range(ns + 1):
            al[new_ed[i]] += f"{new_ed[i+1]}:0;"
            al[new_ed[-(i+1)]] += f"{new_ed[-(i+2)]}:0;"
        N += ns 
    
    ret_graph = f"{N},{Ne * 2}\n"
    for i in range(N):
        ret_graph += f"{i},{gd[i]},{al[i][:-1]}\n"

    return ret_graph

def read_degrees(Path): 
    """ 
    path of csv graph -> list 
    returns the list of degrees 
    of the custom csv of a graph 
    stored at path
    """
    
    with open(Path,"r") as f:
        l = []
        for i in f: 
            if not i.strip(): 
                continue 
            
            l.append(int(i.split(",")[1]))
    return l
                
        

def makeCSVPA(Graph, path):
    """
    graph_string , path
    writes the custom csv corresponding to the graph passed
    as argument in the file at path
    function was rewritten by https://github.com/Pacidus
    """ 
    with open(path, "w") as file: 
        file.write(Graph)
        
def makeCSV(Graph, path):
    """
    nxGraph , path -> custom csv graph file 
    writes the custom csv corresponding to the graph passed
    as argument in the file at path
    """ 
    with open (path, "w") as file: 
     
        file.write(f"{Graph.number_of_nodes()},{Graph.number_of_edges()*2}\n") #2 times nb of edges cuz need (a,b) and (b,a)
        for i in Graph.nodes:
         
            file.write(f'{i},{Graph.degree(i)},{":0;".join( str(i) for i in Graph.neighbors(i))+":0"}\n')
    file.close



def gen_graph(latt, long , rad, step, path):
    """
    creates a MG from the args passed; turns it into a normal directed graph 
    and writes it into the file passed as argument
    """
    
    center= (latt,long)

    MG= ox.graph.graph_from_point( center_point=center, dist=rad, network_type='drive',retain_all=False) #retrieves multigraph
    dict(MG.degree(weight='weight')) 
    
    GG= nx.Graph(MG) #turns it into a normal graph 
    DGG= discretisePA( GG, step) # let's go     
    makeCSVPA(DGG, path) 
 
    return 0
