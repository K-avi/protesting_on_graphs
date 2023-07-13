#I'll use this script to retrieve sopme infos from the db 
#the function that will realyl be usefull is the ... fn 
import numpy as np 
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import os

def plot_efficiency(eff_arr, marker_arr =  [ i for i in "^,v.*dPsX"]):
    
    
    fig, ax = plt.subplots(1, 2, figsize=(10, 10), layout="constrained")
    
    color_patches = []
    marker_patches = [mlines.Line2D([], [], color='k', marker='^', markersize=10, linestyle='' ,label='rand'),
                      mlines.Line2D([], [], color='k', marker=',', markersize=10, linestyle='' ,label='attra'),
                      mlines.Line2D([], [], color='k', marker='v', markersize=10, linestyle='' ,label='attco'),
                      mlines.Line2D([], [], color='k', marker='.', markersize=10, linestyle='' ,label='align'),
                      mlines.Line2D([], [], color='k', marker='*', markersize=10, linestyle='' ,label='propu'),
                      mlines.Line2D([], [], color='k', marker='d', markersize=10, linestyle='' ,label='no majority'),
                     ]
    
    
    for j,i in enumerate(eff_arr): 
        
        color_patches.append(mpatches.Patch(color=i[1], label=i[2]))
      

        ax[0].scatter([ j[0] for j in i[0][0]], [j[1] for j in i[0][0]], color=i[1], marker = marker_arr[0])
        
        for p in range(1, len(i[0])):
            ax[1].scatter([ j[0] for j in i[0][p]], [j[1] for j in i[0][p]], color=i[1], marker = marker_arr[p%len(marker_arr)])
    
    
    ax[1].set_title("scatterplot of efficiency without randomness")
    ax[1].set_xlabel("mean density of groups")
    ax[1].set_ylabel("flocking")
    
    
    ax[0].set_title("scatterplot of efficiency with randomness")
    ax[0].set_xlabel("mean density of groups")
    ax[0].set_ylabel("flocking")
    
    
    for i in [0,1]:
        ax[i].grid("on")
        xlab = ax[i].xaxis.get_label()
        ylab = ax[i].yaxis.get_label()
        xlab.set_style("italic")
        xlab.set_size(10)
        ylab.set_style("italic")
        ylab.set_size(10)
    plt.legend(handles= color_patches + marker_patches)
    plt.show()

def get_efficiency(base_path_array, 
                   color_sim=["b","g","c","m","y","purple","cyan","grey"]): 
    """
    
    str , list -> 
    creates points for every 
    simulation in a base. 
    the x coordinate is the "gathering" 
    of the sim defined by 
    it's mean nb of walkers on occupied nodes. 
    it's y coordinate is it's 
    flocking 
    """
    ret = []
    
    for cpt, base_path in enumerate(base_path_array): 
        
        index = []
        with open(base_path+"/index_base.csv","r") as fi: 
            index = fi.readlines()
        points = [[],[],[],[],[],[]]
        
        for fname in os.listdir(base_path+"/base"):
            
            if "fluxmean" in fname :
                    continue 
            
            with open(base_path+"/base/"+fname,"r" ) as fres: 
                
               
                num = int(fname.split("_")[0])
                s = fres.readlines()[-1]
                
                point = [ float([i for i in s.split(" ")][5]), np.loadtxt(base_path+"/base/"+fname+"fluxmean") ]
            
                li = index[num].split(",")[1:5]
                coeffs = [ float(n.split(":")[1]) for n in li]
                
                if coeffs[0] > 0 : #rand
                    points[0].append(point)
                elif coeffs[1] > 5 : #attra/attco 
                    if "attra" in index[num]: 
                        points[1].append(point)
                    else : 
                        points[2].append(point)
                elif coeffs[2] > 5: 
                        points[3].append(point)
                elif coeffs[3] > 5 : 
                        points[4].append(point)
                else : 
                    points[5].append(point)

        ret.append( [[np.array(i) for i in points],color_sim[cpt%len(color_sim)],base_path])
    plot_efficiency(ret)
    return ret


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
    
    a = get_efficiency(["paris", "paris_slow_ride"])
    

if __name__=='__main__': 
    main()
