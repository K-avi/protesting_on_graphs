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

def gen_opt(): 
    """
        -> str 
    produces a string of option to give to walking_on_graphs 
    """
    str_ret=""
    nb_opt= r.randint(1,3)
    meta_opt= r.randint(0,2)
    opt_strarr= [ "rand:", "attra:", "align:", "attco:"]
    meta_strarr= ["mconst:", "mprop:", "mcrowd"]
    
    #builds the options
    for i in range (0, nb_opt):
        index=r.randint(0, len(opt_strarr)-1)
        str_ret+=opt_strarr[index]+str(r.randint(1,10))+" "
        opt_strarr.pop(index)
    
    #builds the meta option    
    if meta_opt==0:
        str_ret+=meta_strarr[0]+str(r.randrange(0,1))+" "
    elif meta_opt==1: 
        str_ret+=meta_strarr[1]+str(r.randint(1,100))+" "
    else: 
        str_ret += meta_strarr[2]
    
    return str_ret

def gen_db():
    """
    infinitely generates a graph of size between 200m and 5000m ; 
    runs a simulation between 10 and 200 steps on it ;
    generates parameters randomly as well ; 
    puts the outputs in a file and dumps the parameter in another one
    """
    
    #param changing at each sim
    nb_sim_ran=1
    nb_it_gen=0
    nb_nodes=0
    sim_opt=""
    
    #constants
    
    #coordinates from geohack
    cities= [
              ["paris", [48.856667, 2.352222] ] , 
              ["barcelona", [41.363333, 2.166944] ], 
              ["kolkata" ,[22.5726459, 88.3638953] ],
              ["chicago", [41.86777778, -87.66500000] ], #except for chicago cuz I couldn't find it
              ["teheran", [35.696111, 51.423056]],
            ]
    #fixed at 2000
    it_par = 2000
    gsize_par = np.linspace(500, 5000, num=46 , dtype=int)
    
    while True:
        
        
        #gen_graph and options 
        graph_size = np.random.choice(gsize_par)
        simul_opt = gen_opt()
        nb_it_gen= np.random.choice(it_par)
        city = cities[r.randint(0,4)]
        print(f"simul {nb_sim_ran} started with parameters:\nit: {nb_it_gen}\nopt: {simul_opt}\ngsize: {graph_size}\ncity : {city[0]}")
       
        #generates graph and retrieves number of nodes 
        gg.gen_graph(city[1][0], city[1][1], graph_size , 10 , "tmp_graph")
        with open("tmp_graph","r") as tgc:
            nb_nodes = tgc.readline().split(",")[0]
        
        #writes the parameters of nth simul
        with open("index_base.csv", "a") as param_file:
            param_file.write(f"{nb_sim_ran},{nb_it_gen},{simul_opt},{graph_size},{nb_nodes},{city[0]}")
        
        print(f"generated graph with {nb_nodes} nodes\n")   
           
        #runs simul and produces a result file
        start = t.time()
        ars.run_simul_nth(1,1,"tmp_graph",1, nb_it_gen, sim_opt ,"trace", "res_sim")
        print(f"finished simul in {t.time()-start} seconds, starting data analysis\n")
        
        #analyze the data of the simul and cleans the directory
        dt.mean_results("res_sim", f"base/{nb_sim_ran}_simul")
        dt.clean_results("res_sim")
        print(f"finished data analysis results are stored at base/{nb_sim_ran}_simul\n")
        
        nb_sim_ran+=1


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
                                      graph_path, f"base/{cpt}_simul", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},rand:{a},attra:{b},align:{c},propu:{d}\n")
        cpt+=1  

    for a,b,c,d,e in f:             
        
        print(f"simul running with par :\nrand:{a} attra:{b} align:{c} propu:{d}")
        ars.run_simul_nth(1,1, graph_path, 1 , nb_it, f"rand:{a} attco:{b} align:{c} propu:{d}",
                                      graph_path, f"base/{cpt}_simul", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f:
            f.write(f"{cpt},rand:{a},attra:{b},align:{c},propu:{d}\n")
        cpt+=1  
    
    close(index)
    
def query(query, db_name):
    """
    parses a simple query string 
    currently not done 
    placeholder
    """
    if query.startwith("or_select"):
        
        ret=""
        args = query.split(" ")[1::]
        
        with open(f"index_base.csv") as index:
            for line in index : 
               if any( arg in line for arg in args ):
                   ret+= f"{line}\n"
        print("sim matching query are :\n",ret)
    
    elif query.startwith("and_select"): 
        
        ret=""    
        args = query.split(" ")[1::]
        
        with open(f"index_base.csv") as index:
            for line in index : 
               if all( arg in line for arg in args ):
                   ret+= f"{line}\n"
        print("sim matching query are :\n",ret)
   
    elif string.startwith("plot"):   
        
        ret=[]
        for file in os.listdir(db_name):
            args = query.split(" ")[1::]
            if(file.split("_")[1] in  args):
                ret.append(file)
        print(ret)
        sp.plot_list(ret)
        
    else : 
        print("invalid query please print a valid query")
        
    return 0 
    
        
if __name__=='__main__':
    gen_db_var("paris_5k_10m.csv",100)