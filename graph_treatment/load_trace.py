import  numpy as np

def load_trace_elem( path, nb_elem, nb_it ):
    """
    reads the content of the binary file stored at path 
    as a matrix of nb_elem*nb_int 32 bytes integers 
    and stores it in a numpy matrix
    """
    dt_arr= np.fromfile(path, dtype=np.int32)
    return dt_arr.reshape(nb_elem, nb_it)

def main():
    """
    not done
    """
    return 0


if __name__=='__main__':
    main()
    