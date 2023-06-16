#this script will call autorun w different parameters to 
#generate a LOT of stats 
import autorun_simul as ars
import graph_generator as gg 
import data_analysis as dt
import random as r

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
        index=r.randint(1, len(opt_strarr))
        str_ret+=opt_strarr[index]+str(r.randint(1,10))
        opt_strarr.pop(index)
    
    #builds the meta option    
    if meta_opt==0:
        str_ret+=meta_strarr[0]+str(r.randrange(0,1))
    else: 
        str_ret+=meta[1]+str(r.randint(1,100))

    return str_ret

def auto_stat():
    """
    infinitely generates a graph of size between 500m and 5000m ; 
    runs a simulation between 100 and 2000 steps on it ;
    generates parameters randomly as well ; 
    puts the outputs in a file and dumps the parameter in another one
    """
    
    #param changing at each sim
    nb_sim_ran=0
    nb_it_gen=0
    nb_nodes=0
    sim_opt=""
    
    #constants
    latt=48.866667
    long=2.333333

    while(True):
       
        #gen_graph and options 
        graph_size = r.randint(1, 50)*100
        simul_opt = gen_opt()
        nb_it_gen= r.randint(1,9)*100**r.randint(1,2)
        
        #generates graph and retrieves number of nodes 
        gg.gen_graph(latt, long, graph_size , 10 , "tmp_graph.csv")
        with open("tmp_graph.csv","r") as tgc:
            nb_nodes = tgc.readline()
        tgc.close()
        #writes the parameters of nth simul
        print(f"simul started with parameters:\n{nb_sim_ran},{nb_it},{simul_opt},{graph_size},{nb_nodes}")
        with open("sim_par", "a") as param_file:
            param_file.write(f"{nb_sim_ran},{nb_it},{simul_opt},{graph_size},{nb_nodes}")
        param_file.close()   
           
        #runs simul and produces a result file
        ars.run_simul_nth(8,8,"tmp_graph.csv",1, nb_it_gen, sim_opt ,"trace", "res_file_gp", "res_file_mean")
        dt.mean_results("res_group_data", "res_sim"+str(nb_sim_ran))
        dt.clean_results("res_group_data")
        print(f"finished simul result stored at res_sim{nb_sim_ran}")
        nb_sim_ran+=1
        
if __name__=='__main__':
    auto_stat()