#I'll use this script to retrieve sopme infos from the db 
#the function that will realyl be usefull is the ... fn 
import numpy as np 
import matplotlib.pyplot as plt
import os

def get_efficiency(base_path, color_sim_filters=[]): 
    """
    
    str , list -> numpy[2D array]
    creates points for every 
    simulation in a base. 
    the x coordinate is the "gathering" 
    of the sim defined by 
    it's mean nb of walkers on occupied nodes. 
    it's y coordinate is it's 
    flocking 
    """
    ret = [[],[],[]]
    for i in os.listdir(base_path):
        if "fluxmean" in i :
            continue 
        
        with open(base_path+"/"+i,"r" ) as f: 
            s = f.readlines()[-1]
          
            ret[0].append( float([i for i in s.split(" ")][5]))
            ret[1].append( np.loadtxt(base_path+"/"+i+"fluxmean"))
            ret[2].append(int(i.split("_")[0]))
    
    ret[0], ret[1] = np.array(ret[0]), np.array(ret[1])
    return ret

def plot_efficiency(efficiency_arr):
    
    plt.scatter(efficiency_arr[0],efficiency_arr[1])
    plt.title("scatterplot of group density over mean flux")
    plt.show()

def filter_efficiency(efficiency_arr):

    for i,j in enumerate(efficiency_arr[0]):
        if j>=20 and efficiency_arr[1][i]>=0.7:
            print(j,efficiency_arr[1][i],efficiency_arr[2][i])
            
            
def add_gp_permille(file , nb_nodes):
    """
    adds the permille of 
    the nb of groups over 
    the nb of nodes in a graph 
    to a res file
    """
    with open(file,'r') as f: 
        dt = np.loadtxt(f)
        ndt = np.array( [np.append( i,(i[0]/nb_nodes)*1000) for i in dt])
    np.savetxt(file, ndt)
    
    
def update_db_permille(path, nb_nodes): 
    """
    adds the permille of 
    the nb of groups over 
    the nb of nodes to every res
    file at path
    """
    for i in os.listdir(path): 
        if "_res" in i and "_resfluxmean" not in i:
            add_gp_permille(path+"/"+i, nb_nodes)

def main(): 
    """
    """
    
    a = get_efficiency("base")
    filter_efficiency(a)
    plot_efficiency(a)

if __name__=='__main__': 
    main()
