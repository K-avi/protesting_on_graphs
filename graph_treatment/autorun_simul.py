#this files contains the function to automate the run of simulations 
import data_analysis as dt
import graph_generator as gg
import load_trace as lt
import subprocess as sb
import numpy as np
import time as t
import argparse as args


def gen_data_groups( t_curnum, graph_dict):
    """
    curnum matrix , gr - finish testing dt analysisaph_dict -> array of tuples (nb_group, spread_group , group size)
    generates the data related to group for 
    one iteration
    """
    ret= np.array([])
    for i in t_curnum:
        graph_dict=lt.merge_wknum_row_dictgraph(i,graph_dict)
        group_array=dt.get_adj_group(graph_dict)
   
        ret= np.append(ret , np.array([dt.count_groups(group_array) , dt.spreading_groups(group_array), \
                    dt.get_mean_group_size(group_array), 0]))
     
    return ret.reshape(-1, 4)
        


def run_simul_once(nb_threads, path_graph, coeff_wk, nb_it , sim_opt ,trace_name, trace_num,result_file_gp, result_file_mean):
    """
    string, simul_file_name -> simul_file
    the main function to run a simulation 
    starts it w the little shell script , 
    reads from the trace , write in result files
    """
    
    #start simulation 
  
    sb.run(f'bash batch_launch.sh "{nb_threads}" "{path_graph}" "{coeff_wk}" "{nb_it}" "{sim_opt}" "{trace_name}{str(trace_num)}"',shell=True)
    
    #loads the trace
    for i in range (0, nb_threads):
        tr_comp_name=trace_name+str(trace_num)+str(i)
        
        (t_curnum, t_flux ,t_wkpos, dict_graph)= lt.load_trace(tr_comp_name, nb_it)
        lt.clean_trace(tr_comp_name)
    #append the results of analysis functions to a file used to generate mean results of the 
    #simulation
        mean_wkmvmt= dt.get_mean_nodes_visited(t_wkpos)
        group_data_mat=gen_data_groups(t_curnum, dict_graph)
        with open(result_file_mean+str(i), "a") as file_wk:

            file_wk.write(str(mean_wkmvmt))
        file_wk.close

        np.savetxt(result_file_gp+str(i), group_data_mat)
    
def run_simul_nth(num,nb_threads,path_graph , coeff_wk, nb_it, simul_opt ,trace_name, result_file_gp, result_file_mean):
    """
    runs a simulation num times
    on nb_threads with 
    """
    
    a=0
    i=num- (num%nb_threads)
    while i-nb_threads >= 0:
        
        run_simul_once(nb_threads, path_graph , coeff_wk , nb_it, simul_opt, trace_name, a, f"{result_file_gp}_{i}", f"{result_file_mean}_{i}")
        i-=nb_threads
        a+=1
    run_simul_once(num%nb_threads, path_graph , coeff_wk , nb_it, simul_opt, trace_name, a, f"{result_file_gp}_{i}", f"{result_file_mean}_{i}")
    print("simulation are done running; starting data analysis")