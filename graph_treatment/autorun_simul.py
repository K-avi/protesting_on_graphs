# this files contains the function to automate the run of simulations
import data_analysis as dt
import graph_generator as gg
import load_trace as lt
import subprocess as sb
import numpy as np
import time as t
import argparse as args


def gen_data_groups(t_curnum, adj, mobility_mean):
    """
    np.array[1D], scipy.sparse.csgraph , np.array[2D] ->
    array[4] nb_group, spread_group , group size, nb lonely wk
    
    generates the data related to group for
    one iteration
    """
    Nitt = t_curnum.shape[0]
    ret = np.zeros((Nitt, 5))
    ret[:, 3] = mobility_mean
    for itt, cur in enumerate(t_curnum):
        nadj = lt.merge_wknum_adj_mat(cur, adj)
        nb_gp, labels = dt.get_adj_group(cur, nadj)

        ret[itt, 0] = nb_gp
        ret[itt, 1] = dt.spread_gp(nb_gp, labels)
        ret[itt, 2] = dt.size_gp(cur, nb_gp, labels)
        ret[itt, 4] = cur[labels == -1].sum()
    return ret


def run_simul_once(
    nb_threads, path_graph,
    coeff_wk, nb_it,
    sim_opt, trace_name,
    trace_num, result_file
):
    """
    bunch of simul args -> simul_file
    
    the main function to run a simulation
    starts it w the little shell script ,
    reads from the trace , write in result files
    """

    # start simulation
    sb.run(
        f'bash batch_launch.sh "{nb_threads}" "{path_graph}" "{coeff_wk}" "{nb_it}" "{sim_opt}" "{trace_name}{str(trace_num)}"',
        shell=True,
    )

    # loads the trace
    for i in range(nb_threads):
        tr_comp_name = f"{trace_name}{trace_num}{i}"

        (t_curnum, t_flux, t_wkpos, adj) = lt.load_trace(tr_comp_name, nb_it)
        del t_flux
      
        lt.clean_trace(tr_comp_name)
        # append the results of analysis functions to a file used to generate mean results of the
        # simulation

        mobility_mean = dt.stat_mobility(t_wkpos, t_curnum.shape[1])

        group_data_mat = gen_data_groups(t_curnum, adj, mobility_mean)
        del (t_curnum, adj)
        del t_wkpos
        
        np.savetxt(f"{result_file}{i}", group_data_mat)


def run_simul_nth(
    num, nb_threads, path_graph, coeff_wk,
    nb_it, simul_opt, trace_name, result_file
):
    """
    runs a bunch of simul w given parameters
    and calculates their mean result
    """
    N = num // nb_threads
    n = num % nb_threads

    for i in range(N):
        run_simul_once(
            nb_threads, path_graph,
            coeff_wk, nb_it,
            simul_opt, trace_name,
            i, f"{result_file}_{i * nb_threads}",
        )
    run_simul_once(
        n, path_graph,
        coeff_wk, nb_it,
        simul_opt, trace_name,
        N, f"{result_file}_{N * nb_threads}",
    )
    print("simulation are done running; starting data analysis")



def main():
    """
    """
    print("parsing arguments")
    parser = args.ArgumentParser(
        prog="autorun sim",
        description="script retrieving a graph from \
        osm, running a walking on graph simul on it and then doing dt analysis on the simul",
    )
    parser.add_argument(
        "lattitude", metavar="latt", type=float, nargs=1,
        help="floating point number corresponding \
        to the lattitude of a geographical point",
    )
    parser.add_argument(
        "longitude", metavar="long", type=float, nargs=1,
        help="floating point number corresponding \
        to the longitude of a geographical point",
    )
    parser.add_argument(
        "radius", metavar="radius", type=int, nargs=1,
        help="floating point number corresponding \
        to the radius (in meters) of a circle around \
        a geograpgical point used to generate a graph",
    )
    parser.add_argument(
        "step", metavar="step", type=int, nargs=1,
        help="the step used when discretising the \
        graph",
    )
    parser.add_argument(
        "path", metavar="path", type=str, nargs=1,
        help="the path of the file where the csv rep \
        of the graph will be stored",
    )

    parser.add_argument(
        "output_file", metavar="output_file", type=str,
        nargs=1, help="final output file name",
    )
    parser.add_argument(
        "simul_tot", metavar="simul_tot", type=int,
        nargs=1, help="number of simuls per batch",
    )
    parser.add_argument(
        "nb_thread_max", metavar="max_thread", type=int,
        nargs=1, help="number of threads used per batch",
    )
    parser.add_argument(
        "coeff_walkers", metavar="walkers", type=int,
        nargs=1, help="coeff of walkers per batch",
    )
    parser.add_argument(
        "nb_iterations", metavar="nb_iterations", type=int,
        nargs=1, help="nb iterations of the simul",
    )
    parser.add_argument(
        "simul_opt", metavar="simul parameters", type=str,
        nargs=1, help="string to specify options of the simul",
    )
    # doesn't parse the fucking options of the simulation
    # which is kinda fucking stupid
    opt = parser.parse_args()
    (latt, long, rad, step, path) = (
        opt.lattitude[0], opt.longitude[0],
        opt.radius[0], opt.step[0], opt.path[0],
    )
    
    print("generating graph")
    gg.gen_graph(latt, long, rad, step, path)
    
    start_time = t.time()
    (nb_thread_max, tot, coeff_wk, nb_it, sim_opt) = (
        opt.nb_thread_max[0], opt.simul_tot[0],
        opt.coeff_walkers[0], opt.nb_iterations[0],
        opt.simul_opt[0],
    )
    output_file = opt.output_file[0]
    print(
        "starting simulations, please do not remove files created by the simulation before it is done running",
        tot, nb_thread_max, path, coeff_wk, nb_it, sim_opt, output_file,
    )

    run_simul_nth(
        tot, nb_thread_max, path, coeff_wk,
        nb_it, sim_opt, "trace", output_file,
    )

    dt.mean_results(output_file, "res_mean")
    dt.clean_results(output_file)

    print("run finished in "+ str(t.time() - start_time))
    return 0


if __name__ == "__main__":
    main()
