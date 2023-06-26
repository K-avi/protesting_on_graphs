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
    plt.title("evolution of mean spread of groups")
    plt.figure(3)
    plt.title("evolution of number of walkers per group")
    plt.figure(4)
    plt.title("evolution of the mean of different nodes visited by each walker")
    plt.figure(5) 
    plt.title("evolution of the number of 'groupless walkers'")
    plt.figure(6)
    plt.title("evolution of the mean density of the groups")
    plt.figure(7)
    plt.title("evolution of the number of occupied nodes")
    
    for fname in os.listdir("."):
        if simul_name in fname:
            print(fname)
            dt = np.loadtxt(fname)
            c = next(color)
            name = fname.removeprefix(simul_name)
            for i,j in range(1,7),range(0,6):       
                plt.figure(i)
                plt.plot(dt.T[j], color=c, label=name)
    
    for i in range(1,7):
        plt.figure(i)
        plt.legend()
    plt.show()
    
def plot_list(simul_files):
    """
    creates figures for 
    the list of files
    passed as arg
    """
    
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange']) 
    
    plt.figure(1)
    plt.title("evolution of number of groups")
    plt.figure(2)
    plt.title("evolution of mean spread of groups")
    plt.figure(3)
    plt.title("evolution of number of walkers per group")
    plt.figure(4)
    plt.title("evolution of the mean of different nodes visited by each walker") 
    plt.figure(5) 
    plt.title("evolution of the number of 'groupless walkers'")
    plt.figure(6)
    plt.title("evolution of the mean density of the groups")
    plt.figure(7)
    plt.title("evolution of the number of occupied nodes")
    
    for name in simul_files:
             
        dt = np.loadtxt(name)
        c = next(color)
        for i,j in range(1,7),range(0,6):       
            plt.figure(i)
            plt.plot(dt.T[j], color=c, label=name)
    
    for i in range(1,7):
        plt.figure(i)
        plt.legend()
    plt.show()
    
def main():
    
    
    parser = arg.ArgumentParser(description='plots the simul name given')
    parser.add_argument('name', type=str,
                    help='name of simul to plot')
    args = parser.parse_args()
    
    plot(args.name) 
    return 0 

if __name__=='__main__':
    main()
    