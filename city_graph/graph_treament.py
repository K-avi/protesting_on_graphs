#this is the file doing to the retrieval/ treatment of city graphs using OSMNX 
import osmnx as ox
import networkx as nx
import argparse as arg
import numpy as np

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
         
            #makes newgraph from adj mat 
            """mat = np.eye(N=size, k=1, dtype=int)
            sym_mat= mat + mat.T
            G=nx.from_numpy_array(sym_mat) 
            
            mapping = dict()
            G = nx.relabel_nodes(G, mapping, copy=False)
            
            #RetGraph.add_edge(a, G.)
            print(G)"""
            
            nx.add_path(RetGraph, a+[]+b )
            
        else : 
            ret_graph.add_edge(a, b)
    
    nx.convert_node_labels_to_integers(ret_graph)
    return ret_graph

def makeCSV(Graph, path):
    """
    build the string corresponding to the custom csv rep 
    of a graph and then dumps it in the file stored at "path"
    """ 
    print("bonjour, je ne suis pas un csv")


def main():
    """
    creates a MG from the args passed; turns it into a normal directed graph 
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
    
    
    #print(MG.edges(data="length"))
    GG= nx.Graph(MG) #turns it into a normal graph 
    
    DGG= discretise( GG, step) # :(
   # makeCSV(GG, path) 
    

if __name__=='__main__' :
    main()