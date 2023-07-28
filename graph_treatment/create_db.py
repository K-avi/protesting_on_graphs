#this script will call autorun w different parameters to 
#generate a LOT of stats 
import autorun_simul as ars
import graph_generator as gg 
import data_analysis as dt
import random as r
import time as t
import numpy as np
import networkx as nx 
import sim_plot as sp
import itertools as it 


def parse_seen_cpt(path): 
    """
    str -> set , num
    """
    cpt=0
    ret= set()
    with open(path, "r") as file:
        
        for line in file: 
            if not line.strip():
                continue
            s = line.split(",")[1::]
            l = []
            for field in s: 
                l.append(int( float(field.split(":")[1])*10))
            if(len(l)!=4):
                print("line:\n",line)
                print("error in parse seen")
                exit(-1)
            ret.add(tuple(l))
            cpt+=1
        
        
    return ret, cpt


 
def gen_db_var(graph_path, nb_it):
    """ 
    graphml_path , nb_it -> db of simul results
    
    generates the db w iterations instead of 
    randomly pulling graphs
    """

    s = [i for i in it.combinations_with_replacement(np.linspace(0,10,11), r=4) if sum(i) == 10]
    f = sum([ list(set(it.permutations(j, r=len(j)))) for j in s ],[])
    
    seen, cpt = parse_seen_cpt("index_base.csv")
    print(seen)
    for a,b,c,d in f: 
        
        if(a,b,c,d) in seen: 
            continue 
        
        print(f"simul running with par :\nrand:{a} attra:{b} align:{c} propu:{d}")
        ars.run_simul_nth(1,1, graph_path, 1 , nb_it,
                                      f"rand:{a} attco:{b} align:{c} propu:{d}",
                                      graph_path, f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"base/{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},rand:{a},attra:{b},align:{c},propu:{d}\n")
        cpt+=1  

    for a,b,c,d,e in f:             
        
        print(f"simul running with par :\nrand:{a} attra:{b} align:{c} propu:{d}")
        ars.run_simul_nth(1,1, graph_path, 1 , nb_it, f"rand:{a} attco:{b} align:{c} propu:{d}",
                                      graph_path, f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"base/{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},rand:{a},attra:{b},align:{c},propu:{d}\n")
        cpt+=1  
    
    close(index)
    
    

def gen_small(graph_path, nb_it):
    """ 
    graphml_path , nb_it -> db of simul results
    
    generates a base of about 30 sims exploring different variations 
    """

     #permutations from (0,10) to (5,5)
    s2 = [i for i in it.combinations_with_replacement(np.linspace(0,10,11), r=2) if sum(i) == 10]
    f2 = s2
    f2.sort()
    
    
    s3 = [i for i in it.combinations_with_replacement(np.linspace(0,10,11), r=3) if sum(i) == 10]
    f3 = sum([ list(set(it.permutations(j, r=len(j)))) for j in s3 ],[])
    f3 = [ (i,j,k) for (i,j,k) in f3 if j >= 5 and k != 0]
    f3.sort()
    
    cpt = 0 
    for (a,b) in f2: 
              
        print(f"simul running with par :\nattra:{a} align:{b}")
        ars.run_simul_nth(4,4, graph_path, 1 , nb_it,
                                      f"attra:{a} align:{b}",
                                      "trace", f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"base/{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},attra:{a} align:{b}\n")
        cpt+=1 
    
    for (a,b) in f2: 
       
        print(f"simul running with par :\nattco:{a} alico:{b}")
        ars.run_simul_nth(4,4, graph_path, 1 , nb_it,
                                      f"attco:{a} alico:{b}",
                                      "trace", f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"base/{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},attco:{a} alico:{b}\n")
        cpt+=1 
    
    for (a,b,c) in f3: 
            
        print(f"simul running with par :\nattra:{a} align:{b} rand:{c}")
        ars.run_simul_nth(4,4, graph_path, 1 , nb_it,
                                      f"attra:{a} align:{b} rand:{c}",
                                      "trace", f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"base{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},attra:{a} align:{b} rand:{c}\n")
        cpt+=1   
    close(index)
    

def get_simpar(index_path):
    """
    index base path -> array of strings
    """
    strings = []
    with open(index_path, "r") as f: 
        for line in f: 
            if not line.strip(): 
                continue
            
            strings.append("".join([ i+" " for i in line.split(",")[1::] ]))
           
    return strings
            
def gen_newflux(graph_path, index_path, nb_it_flux):
    """
    generate new flux data and so on
    """
    simstrings = get_simpar(index_path)
    for i in simstrings: 
        
        print(f"simul running with par :{i}")
        ars.run_simul_nth_flux(1,1, graph_path, 1 , nb_it, i, graph_path, f"{cpt}_run", nb_it - 10)
        dt.mean_results_flux( f"{cpt}_run", f"{cpt}_res")
        dt.clean_results( f"{cpt}_run")
    

        
if __name__=='__main__':
    gen_db_var("paris_5k_10m.csv",100)