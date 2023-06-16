#this script will call autorun w different parameters to 
#generate a LOT of stats 
import autorun_simul as ars
import graph_generator as gg 
import data_analysis as dt
import random as r
import time as t
import numpy as np

def gen_opt(): 
    """
        -> str 
    produces a string of option to give to walking_on_graphs 
    """
    str_ret=""
    nb_opt= r.randint(1,3)
    meta_opt= r.randint(0,1)
    opt_strarr= [ "rand:", "attra:", "align:", "attco:"]
    meta_strarr= ["mconst:", "mprop:"]
    
    #builds the options
    for i in range (0, nb_opt):
        index=r.randint(0, len(opt_strarr)-1)
        str_ret+=opt_strarr[index]+str(r.randint(1,10))+" "
        opt_strarr.pop(index)
    
    #builds the meta option    
    if meta_opt==0:
        str_ret+=meta_strarr[0]+str(r.randrange(0,1))+" "
    else: 
        str_ret+=meta_strarr[1]+str(r.randint(1,100))+" "

    return str_ret

def auto_stat():
    """
    infinitely generates a graph of size between 500m and 5000m ; 
    runs a simulation between 100 and 2000 steps on it ;
    generates parameters randomly as well ; 
    puts the outputs in a file and dumps the parameter in another one
    """
    
    #param changing at each sim
    nb_sim_ran=1
    nb_it_gen=0
    nb_nodes=0
    sim_opt=""
    
    #constants
    latt=48.866667
    long=2.333333

    it_par = np.linspace(100, 2000, num=20, dtype=int)
    gsize_par = np.linspace(200, 5000, num=49 , dtype=int)
    while(True):
       
        #gen_graph and options 
        graph_size = np.random.choice(gsize_par)
        simul_opt = gen_opt()
        nb_it_gen= 10 #np.random.choice(it_par)
        print(f"simul {nb_sim_ran} started with parameters:\nit: {nb_it_gen}\nopt: {simul_opt}\ngsize: {graph_size}")
       
        #generates graph and retrieves number of nodes 
        gg.gen_graph(latt, long, graph_size , 10 , "tmp_graph")
        with open("tmp_graph","r") as tgc:
            nb_nodes = tgc.readline().split(",")[0]
        tgc.close()
        
        #writes the parameters of nth simul
        with open("sim_par", "a") as param_file:
            param_file.write(f"{nb_sim_ran},{nb_it_gen},{simul_opt},{graph_size},{nb_nodes}\n")
        param_file.close()
        print(f"generated graph with {nb_nodes} nodes\n")   
           
        #runs simul and produces a result file
        start = t.time()
        ars.run_simul_nth(2,2,"tmp_graph",1, nb_it_gen, sim_opt ,"trace", "res_sim"+str(nb_sim_ran), "res_file_mean")
        print(f"finished simul in {t.time()-start} seconds, starting data analysis\n")
        
        #analyze the data of the simul and cleans the directory
        dt.mean_results("res_sim"+str(nb_sim_ran), "res_mean"+str(nb_sim_ran))
        dt.clean_results("res_sim"+str(nb_sim_ran))
        print(f"finished data analysis results are stored at res_sim{nb_sim_ran}\n")
        
        nb_sim_ran+=1
        
if __name__=='__main__':
    auto_stat()