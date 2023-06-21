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
    cities= [ ["paris",[48.856667, 2.352222]] , 
              ["barcelona" [41.363333, 2.166944]], 
              ["kolkata" ,[22.5726459, 88.3638953]],
              ["chicago", [41.86777778,-87.66500000]] #except for chicago cuz I couldn't find it
              ["teheran", [35.696111, 51.423056]]
            ]
    #fixed at 2000
    it_par = np.linspace(10, 200, num=20, dtype=int)
    gsize_par = np.linspace(200, 5000, num=49 , dtype=int)
    while True:
        
        
        #gen_graph and options 
        graph_size = np.random.choice(gsize_par)
        simul_opt = gen_opt()
        nb_it_gen= np.random.choice(it_par)
        city = cities[r.randint(0,4)]
        print(f"simul {nb_sim_ran} started with parameters:\nit: {nb_it_gen}\nopt: {simul_opt}\ngsize: {graph_size}\ncity : {city[0]}")
       
        #generates graph and retrieves number of nodes 
        gg.gen_graph(city[1][0] city[1][1], graph_size , 10 , "tmp_graph")
        with open("tmp_graph","r") as tgc:
            nb_nodes = tgc.readline().split(",")[0]
        tgc.close()
        
        #writes the parameters of nth simul
        with open("sim_par", "a") as param_file:
            param_file.write(f"{nb_sim_ran},{nb_it_gen},{simul_opt},{graph_size},{nb_nodes},{city[0]}\n")
        param_file.close()
        print(f"generated graph with {nb_nodes} nodes\n")   
           
        #runs simul and produces a result file
        start = t.time()
        ars.run_simul_nth(8,8,"tmp_graph",1, nb_it_gen, sim_opt ,"trace", "res_sim")
        print(f"finished simul in {t.time()-start} seconds, starting data analysis\n")
        
        #analyze the data of the simul and cleans the directory
        dt.mean_results(f"ressim_{nb_sim_ran}")
        dt.clean_results("res_sim")
        print(f"finished data analysis results are stored at ressim{nb_sim_ran}\n")
        
        nb_sim_ran+=1
        
def query(string):
    """
    parses a simple query string 
    currently not done 
    placeholder
    """

    return 0
    
        
if __name__=='__main__':
    gen_db()