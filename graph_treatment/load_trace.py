#file containing the functions to load the traces of a simulation
import  numpy as np
import sys

def load_trace_elem( path, nb_elem, nb_it ):
    """
    reads the content of the binary file stored at path 
    as a matrix of nb_elem*nb_int 32 bytes integers 
    and stores it in a numpy matrix
    """
    dt_arr= np.fromfile(path, dtype=np.int32)
    return dt_arr.reshape( nb_it, nb_elem)


def load_trace(trace_name):
    """
    loads the formatted trace files corresponding to the trace name 
    given as argument from the current directory 
    and returns it as a tuple of numpy matrixes. 
    the first element of the tuple is a nb_iteration * nb_nodes matrix
    containing the evolution of the number of walkers at each node during the simulation 
    the second one is a nb_iteration * nb_lines matrix containing the evolution 
    of the flux stored at each line during the simulation
    the last one is a  nb_iteration * nb_walkers matrix 
    containing the evolution of the position of each walker during the simulation
    """
    nb_it = int(trace_name.split("_")[4])
    
    t_curnum = load_trace_elem( trace_name+"_curnum",  int(trace_name.split("_")[1]), nb_it)
    t_flux = load_trace_elem( trace_name+"_flux", int(trace_name.split("_")[2]), nb_it)
    t_wkpos = load_trace_elem( trace_name+"_wkpos", int(trace_name.split("_")[3]), nb_it)
    
    return (t_curnum, t_flux ,t_wkpos)
    
def main():
    """
    loads the trace files from the current directory 
    and stores em into variables. 
    
    placeholder main relying on the load_trace function 
    which is the relevant one tbh
    """
    if(len(sys.argv) == 0 ):
        print(f"usage: {sys.argv[0]} trace_name\n")
        exit(1)
    
    trace_name = sys.argv[1]
    (curnum, flux, wkpos)= load_trace(trace_name)
    print(f"curnum:\n{curnum}\nflux:\n{flux}\nwkpos:\n{wkpos}")
    return 0


if __name__=='__main__':
    main()
    