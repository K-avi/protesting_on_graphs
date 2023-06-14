#this files contains the function to automate simulations 
import data_analysis as dt
import graph_generator as gg
import load_trace as lt
import subprocess as sb
import numpy as np
import time as t


def gen_data_groups( t_curnum, graph_dict):
    """
    curnum matrix , gr - finish testing dt analysisaph_dict -> array of tuples (nb_group, spread_group , group size)
    generates the data related to group for 
    one iteration
    """
    ret= np.array([])
    for i in t_curnum:
        graph_dict=lt.merge_wknum_row__dictgraph(i,graph_dict)
        group_array=dt.get_adj_group(graph_dict)
   
        ret= np.append(ret , np.array([dt.count_groups(group_array) , dt.spreading_groups(group_array), \
                    dt.get_mean_group_size(group_array)]))
     
    return ret
        


def run_simul_once(nb_threads, path_graph, coeff_wk, nb_it , sim_opt ,trace_name, result_file_gp, result_file_mean):
    """
    string, simul_file_name -> simul_file
    the main function to run a simulation 
    starts it w the little shell script , 
    reads from the trace , write in result files
    """
    
    #start simulation 
    sb.run(f"bash graph_treatment/batch_launch.sh {nb_threads} {path_graph} {coeff_wk} {nb_it} {sim_opt} {trace_name}",shell=True)
    
    #loads the trace
    (t_curnum, t_flux ,t_wkpos, dict_graph)= lt.load_trace(trace_name, nb_it)
    lt.clean_trace(trace_name)
    #append the results of analysis functions to a file used to generate mean results of the 
    #simulation
    mean_wkmvmt= dt.get_mean_nodes_visited(t_wkpos)
    group_data_mat=gen_data_groups(t_curnum, dict_graph)
    with open(result_file_mean, "a") as file_wk:

        file_wk.write(str(mean_wkmvmt))
    file_wk.close
    np.savetxt(result_file_gp, group_data_mat)
    
def run_simul_nth(num,nb_threads,path_graph , coeff_wk, nb_it, simul_opt ,trace_name, result_file_gp, result_file_mean):
    """
    runs a simulation num times
    on nb_threads with 
    """
    i=num- (num%nb_threads)
    while i-nb_threads > 0:
        run_simul_once(nb_threads, path_graph , coeff_wk , nb_it, simul_opt, trace_name, f"{result_file_gp}_{i}", f"{result_file_mean}_{i}")
        i-=nb_threads
    run_simul_once(num%nb_threads, path_graph , coeff_wk , nb_it, simul_opt, trace_name, f"{result_file_gp}_{i}", f"{result_file_mean}_{i}")
        


def test():
    start = t.time()
    run_simul_nth(1, 1,"city_graph/paris_4000m_radius.csv",1 , 10, "rand:1", "trace", "res_gp" , "res_fm" )
    #sb.run(f"bash graph_treatment/batch_launch.sh 1 city_graph/paris_4000m_radius.csv 1 100",shell=True)
    #sb.run(f"bash graph_treatment/batch_launch.sh 1 city_graph/paris_4000m_radius.csv 1 100",shell=True)

    print(t.time()-start)
def main(): 
    """
    args: 
    nb_it nb_threads graph_gen_args args_simul_string 
    """
    parser = arg.ArgumentParser(prog="graph_parser",description="script retrieving a graph from \
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
    
    parser.add_argument('batches', metavar='batches', type=int, nargs=1, help="number of threads used per batch")
    parser.add_argument('simul_tot', metavar='simul_tot', type=int, nargs=1, help="number of simuls per batch")
    parser.add_argument('nb_walkers', metavar='walkers', type=int, nargs=1, help="number of simul running per batch")
    parser.add_argument('nb_iterations', metavar='walkers', type=int, nargs=1, help="number of simul running per batch")
    #doesn't parse the fucking options of the simulation 
    #which is kinda fucking stupid
    args = parser.parse_args()
    (latt, long, rad, step, path)= (args.lattitude[0] ,  args.longitude[0], args.radius[0], args.step[0], args.path[0])
    gg.gen_graph(latt, long, rad, step , path)
    
    (batches, tot, coeff_wk, nb_it) = (args.batches[0], args.simul_tot[0], args.nb_walkers[0], args.nb_iterations[0])
    
    run_simul_nth(batches, tot , path , coeff_wk, nb_it, "", "trace", "res_group_data", "res_walker_path")
    return 0


if __name__=='__main__':
    test()