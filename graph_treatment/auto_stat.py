#this script will call autorun w different parameters to 
#generate a LOT of stats 
import autorun_simul as ats
import graph_generator as gg 
import multi_sim_analysis as ms
import random as r

def gen_opt(): 
    """
        -> str 
    produces a string of option to give to walking_on_graphs 
    """
    str_ret=""
    nb_opt= r.randint(1,3)


    return str_ret

    



def auto_stat():
    """
    infinitely generates a graph of size between 500m and 5000m ; 
    runs a simulation between 100 and 2000 steps on it ;
    generates parameters randomly as well ; 
    puts the outputs in a file and dumps the parameter in another one
    """
    nb_sim_ran=0
    nb_it_gen=0
    sim_opt=""
    latt=48.866667
    long=2.333333
    cur_rad=0

    while(True):
        with open("sim_par", "a") as param_file:
            param_file.write(f"placeholder")
        
        gg.gen_graph(latt, long, cur_rad, 10 , "tmp_graph.csv")
        ats.run_simul_nth(8,8,"tmp_graph.csv",1, nb_it_gen, "trace", "res_file_gp", "res_file_mean")
        ms.mean_results("res_group_data", "res_sim"+str(nb_sim_ran))
        ms.clean_results("res_group_data")

        nb_sim_ran+=1

    

