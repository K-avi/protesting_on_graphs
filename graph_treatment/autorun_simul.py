#this files contains the function to automate simulations 
import data_analysis as dta
import graph_generator as gg
import load_trace as lt


def run_simul_once():
    """
    """
    
def rune_simul_nth(num):
    """
    """
    for i in range(num):
        run_simul_once()

def main(): 
    """
    args: 
        nb_it ggargs argstring (simul args)
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

    args = parser.parse_args()
    (latt, long, rad, step, path)= (args.lattitude[0] ,  args.longitude[0], args.radius[0], args.step[0], args.path[0])

    gg.gen_graph(latt, long, rad, step , path)
    return 0


if __name__=='__main__':
    main()