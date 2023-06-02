#!/bin/python3.10
#this is the file doing to the retrieval/ treatment of city graphs using OSMNX 
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


def main():
    """
    creates a MG from the args passed; turns it into a normal directed graph 
    """
    """
    parser = arg.ArgumentParser(prog="graph_parser",description="script retrieving a graph from \
        osm and treating it for use with the graph_walker program")
    parser.add_argument('lattitude',metavar='latt',type=float,nargs=1,help="floating point number corresponding \
        to the lattitude of a geographical point")
    parser.add_argument('longitude',metavar='long',type=float,nargs=1,help="floating point number corresponding \
        to the longitude of a geographical point")
    parser.add_argument('radius', metavar='radius', type=int, nargs=1, help="floating point number corresponding \
        to the radius (in meters) of a circle around a geograpgical point used to generate a graph")
    parser.add_argument('step', metavar='step', type=int, nargs=1, help="the step used when discretising the \
        graph")
    parser.add_argument('path', metavar='path', type=str, nargs=1, help="the path of the file where the csv rep \
        of the graph will be stored")

    args = parser.parse_args()
    (latt, long, rad, step, path)= (args.lattitude[0] ,  args.longitude[0], args.radius[0], args.step[0], args.path[0])

    center= (latt,long)

    MG= ox.graph.graph_from_point( center_point=center, dist=rad, retain_all=False) #retrieves multigraph
    dict(MG.degree(weight='weight')) 
    
    GG= nx.Graph(MG) #turns it into a normal graph 
    DGG= discretise( GG, step) # let's go     
    #nx.draw(GG)
    #nx.draw(DGG)
    
    makeCSV(DGG, path) 
    """
    t=loadCSV("paris_test.csv")
    makeCSV(t, "paris_test_cp.csv")
    
    return 0

   
   


if __name__=='__main__' :
    main()