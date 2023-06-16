#this files contains the function to automate simulations 
import data_analysis as dt
import graph_generator as gg
import load_trace as lt
import subprocess as sb
import numpy as np
import time as t
import argparse as args
import multi_sim_analysis as ms


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
  
    sb.run(f'bash graph_treatment/batch_launch.sh "{nb_threads}" "{path_graph}" "{coeff_wk}" "{nb_it}" "{sim_opt}" "{trace_name}{str(trace_num)}"',shell=True)
    
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
   

def test():
    start = t.time()
    run_simul_nth(2, 2,"city_graph/paris_4000m_radius.csv",1 , 10, "rand:1 attra:2", "trace", "res_gp" , "res_fm" )
    #sb.run(f"bash graph_treatment/batch_launch.sh 1 city_graph/paris_4000m_radius.csv 1 100",shell=True)
    #sb.run(f"bash graph_treatment/batch_launch.sh 1 city_graph/paris_4000m_radius.csv 1 100",shell=True)

    print(t.time()-start)
def main(): 
    """
    args: 
    nb_it nb_threads graph_gen_args args_simul_string 
    """
    print("parsing arguments")
    parser = args.ArgumentParser(prog="graph_parser",description="script retrieving a graph from \
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
    
    parser.add_argument('output_file', metavar='output_file', type=int, nargs=1, help="final output file name") 
    parser.add_argument('simul_tot', metavar='simul_tot', type=int, nargs=1, help="number of simuls per batch")
    parser.add_argument('nb_thread_max', metavar='max_thread', type=int, nargs=1, help="number of threads used per batch")
    parser.add_argument('coeff_walkers', metavar='walkers', type=int, nargs=1, help="coeff of walkers per batch")
    parser.add_argument('nb_iterations', metavar='nb_iterations', type=int, nargs=1, help="nb iterations of the simul")
    parser.add_argument('simul_opt', metavar='simul parameters', type=str, nargs=1, help="string to specify options of the simul")
    #doesn't parse the fucking options of the simulation 
    #which is kinda fucking stupid
    opt = parser.parse_args()
    (latt, long, rad, step, path)= (opt.lattitude[0] ,  opt.longitude[0], opt.radius[0], opt.step[0], opt.path[0])
    print("generating graph")
    gg.gen_graph(latt, long, rad, step , path)
    
    start_time = t.time()
    (nb_thread_max, tot, coeff_wk, nb_it, sim_opt) = (opt.nb_thread_max[0], opt.simul_tot[0], opt.coeff_walkers[0], opt.nb_iterations[0], opt.simul_opt[0])
    output_file= opt.output_file[0
                                 ]
    print("starting simulations, please do not remove files created by the simulation before it is done running")
    
    run_simul_nth(tot, nb_thread_max , path , coeff_wk, nb_it,  sim_opt, "trace", "res_group_data", "res_walker_path")
    ms.mean_results("res_group_data", output_file)
    ms.clean_results("res_group_data")
    
    print("simulation finished in "+ str(t.time()-start_time) +" seconds\nResults are stored in res_mean")
    return 0


if __name__=='__main__':
    main()