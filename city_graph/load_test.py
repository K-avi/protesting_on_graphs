#!/bin/python3.10
import networkx as nx

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
    
    g= loadCSV("city_graph/paris_test.csv")
    return 0 

if __name__=='__main__':
    main()