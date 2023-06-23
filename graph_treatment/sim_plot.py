#an actual clean script to plot stuff 
import numpy as np 
import matplotlib.pyplot as plt
import argparse as arg
import os
from cycler import cycler

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
            plt.figure(1)
            plt.plot(dt.T[0], color=c, label=name)
            plt.figure(2)
            plt.plot(dt.T[1], color=c, label=name)
            plt.figure(3)
            plt.plot(dt.T[2], color=c, label=name)
            plt.figure(4)
            plt.plot(dt.T[3], color=c, label=name)
            plt.figure(5)
            plt.plot(dt.T[4], color=c, label=name)
            plt.figure(6)
            plt.plot(dt.T[5], color=c, label=name)
            plt.figure(7)
            plt.plot(dt.T[6], color=c, label=name)
    plt.figure(1)
    plt.legend()
    plt.figure(2)
    plt.legend()
    plt.figure(3)
    plt.legend()
    plt.figure(4)
    plt.legend()
    plt.figure(5)
    plt.legend()
    plt.figure(6)
    plt.legend()
    plt.figure(7)
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
    