#an actual clean script to plot stuff 
import numpy as np 
import matplotlib.pyplot as plt
import argparse as arg
import os

def plot(simul_name):
    """
    creates figures for 
    the simul files in fname that 
    are in the current dir
    and plots the figures
    """  
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange'])
    
    
    plt.figure(1)
    plt.title("evolution of number of groups")
    plt.figure(2)
    plt.title("evolution of mean sprawl of groups")
    plt.figure(3)
    plt.title("evolution of number of walkers per group")
    plt.figure(4) 
    plt.title("evolution of the number of 'groupless walkers'")
    plt.figure(5)
    plt.title("evolution of the mean density of the groups")
    plt.figure(6)
    plt.title("evolution of the number of occupied nodes")
    
    for fname in os.listdir("."):
        if simul_name in fname:
            print(fname)
            dt = np.loadtxt(fname)
            c = next(color)
            name = fname.removeprefix(simul_name)
            for i,j in range(1,6),range(0,5):       
                plt.figure(i)
                plt.plot(dt.T[j], color=c, label=name)
    
    for i in range(1,7):
        plt.figure(i)
        plt.legend()
    plt.show()
    
def plot_list(simul_files, index=False):
    """
    creates figures for 
    the list of files
    passed as arg
    """
    
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange']) 
    
    plt.figure(1)
    plt.title("evolution of number of groups")
    plt.figure(2)
    plt.title("evolution of mean sprawl of groups")
    plt.figure(3)
    plt.title("evolution of number of walkers per group")
    plt.figure(4) 
    plt.title("evolution of the number of 'groupless walkers'")
    plt.figure(5)
    plt.title("evolution of the mean density of the groups")
    plt.figure(6)
    plt.title("evolution of the number of occupied nodes")
    
    for name in simul_files:
          
        dt = np.loadtxt(name)
        c = next(color)
        cusname = name 
        if(index):
            with open("index_base.csv", "r") as f: 
                for line in f:
                   
                    if( name.split("/")[1].split("_")[0]== line.split(",")[0]):
                        cusname = "".join([ i+" " for i in line.split(",")[1::] ])
                        break 
        for i,j in enumerate(range(1,7)):       
            plt.figure(j)
            plt.plot(dt.T[i], color=c, label=cusname)
    
    for i in range(1,7):
        plt.figure(i)
        plt.legend()
    plt.show()
    
def main():
    
    
    parser = arg.ArgumentParser(description='plots the simul name given')
    parser.add_argument('name', type=str, nargs="+", help='name of simul files to plot')
    args = parser.parse_args()
    
    plot_list(args.name, index=True) 
    return 0 

if __name__=='__main__':
    main()
    
