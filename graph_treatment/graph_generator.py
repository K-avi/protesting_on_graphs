#!/bin/python3.10
#this is the file doing to the retrieval/ treatment of city graphs using OSMNX and networkx
import osmnx as ox
import networkx as nx
import argparse as arg
import numpy as np
import matplotlib.pyplot as plt

def discretise(Graph, step): 
    """
    nxGraph, step-> nxGraph  
    
    discretises a graph passed as arg with the step
    i.e : creates new nodes when the distance between the nodes a,b in a line 
    is bigger than step  
    returns new graph
    """ 
    ret_graph=nx.Graph()
    
    for lines in Graph.edges(data="length"): 
        (a,b,length)=lines
        if(length > step ): 
        
            size= int(length/step)
            new_graph= nx.Graph()
            nx.add_path(new_graph, [a]+ [str(i)+str(a)+str(b) for i in range(0,size) ] +[b])
            
            ret_graph = nx.compose(ret_graph ,new_graph)      
        else : 
            ret_graph.add_edge(a, b)
    
    ret_graph= nx.convert_node_labels_to_integers(ret_graph,ordering='default', label_attribute=None)
    return ret_graph

def makeCSV(Graph, path):
    """
    nxGraph , str 
    writes the custom csv corresponding to the graph passed
    as argument in the file at path
    """ 
    with open (path, "w") as file: 
     
        file.write(f"{Graph.number_of_nodes()},{Graph.number_of_edges()*2}\n") #2 times nb of edges cuz need (a,b) and (b,a)
        for i in Graph.nodes:
            file.write(f'{i},{Graph.degree(i)},{":0;".join( str(i) for i in Graph.neighbors(i))+":0"}\n')
    file.close

def loadCSV(path):
    """
    str -> nxGraph
    creates a networx graph from the custom csv stored at path
    should prolly use a real csv loader but this function is somewhat 
    trivial so it doesn't really mater I guess
    """
    ret_graph = nx.Graph()
    
    with open (path, "r") as file: 
        next(file)
        for line in file:
            node_from=line.split(",")[0]
            nodes_to=line.split(",")[2].split(";")
            for i in nodes_to:  
                node_to = i.split(":")[0]
                ret_graph.add_edge(int(node_from), int(node_to))         
    file.close
    #ret_graph= nx.convert_node_labels_to_integers(ret_graph,ordering='default', label_attribute=None)
    return ret_graph

def discretisePA(Graph, step): 
    """
    nxGraph, step-> graph_string
    discretises a graph passed as arg with the step
    i.e : creates new nodes when the distance between the nodes a,b in a line 
    is bigger than step  
    returns new graph
    by https://github.com/Pacidus
    """ 
    N = Graph.number_of_nodes()
    lengths = Graph.edges(data="length")
    
    id_nodes = {j: i for i,j in enumerate(Graph.nodes())}
    al = [""] * N
    gd = [0] * N
    Ne = Graph.number_of_edges()
    for a, b, length in lengths:
        ns = int(length/step)
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

def makeCSVPA(Graph, path):
    """
    graph_string , path
    writes the custom csv corresponding to the graph passed
    as argument in the file at path
    by https://github.com/Pacidus
    """ 
    with open(path, "w") as file: 
        file.write(Graph)

def gen_graph(latt, long , rad, step, path):
    """
    creates a MG from the args passed; turns it into a normal directed graph 
    and writes it into the file passed as argument
    """
    
    center= (latt,long)

    MG= ox.graph.graph_from_point( center_point=center, dist=rad, retain_all=False) #retrieves multigraph
    dict(MG.degree(weight='weight')) 
    
    GG= nx.Graph(MG) #turns it into a normal graph 
    DGG= discretisePA( GG, step) # let's go     
    makeCSVPA(DGG, path) 
 
    return 0