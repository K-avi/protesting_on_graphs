import networkx as nx 
import graph_generator as gg 
import random as r 
import math

def make_small_world(size, rm_coeff=0.7):
    """
    int -> nx.Graph 
    """

    g = nx.Graph(nx.navigable_small_world_graph(size))
    d = dict()
    
    #calculates and adds the length attribute to the graph
    for i, j in g.edges : 
        x1,y1 = i 
        x2,y2 = j
        d[(i,j)] = math.sqrt((x2-x1)**2+(y2-y1)**2)*10

    nx.set_edge_attributes(g, d, "length")

    # removes 1- rm_coeff % of nodes
    rm = []
    for i in g.nodes: 
        if r.random() > rm_coeff:
            rm.append(i)
    
    g.remove_nodes_from(rm)
    
    #removes nodes of degree 1 iteratively
    rm = [ i for i in g.nodes if g.degree[i] == 1]
    while len(rm) != 0 : 
        g.remove_nodes_from(rm)
        rm = [ i for i in g.nodes if g.degree[i] == 1]
    
    #filters out elements not in the largest cc of g 
    largest_cc = max(nx.connected_components(g), key=len)
    g.remove_nodes_from([ i for i in g if i not in largest_cc])
    
    return nx.convert_node_labels_to_integers(g)


def create_csv_small_world(size, path="small_world.csv", rm_coeff = 0.7, step=10):
    
    g = make_small_world(size, rm_coeff)
    print(len(g),len(g.edges))
    dg = gg.discretisePA(g,step)
    gg.makeCSVPA(dg, path)

create_csv_small_world(100)