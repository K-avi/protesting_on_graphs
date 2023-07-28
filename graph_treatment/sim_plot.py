#an actual clean script to plot stuff 
import numpy as np 
import matplotlib.pyplot as plt
import argparse as arg
import os


def plot_list(simul_files, index=False):
    """
    creates figures for 
    the list of files
    passed as arg
    """
    
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange','cyan',"lightblue","darkgreen"]) 
    
    plt.figure(1)
    plt.title("evolution of number of groups")
    plt.figure(2)
    plt.title("evolution of mean spread of groups")
    plt.figure(3)
    plt.title("evolution of number of walkers per group")

    plt.figure(5) 
    plt.title("evolution of the number of 'groupless walkers'")
    plt.figure(6)
    plt.title("evolution of the mean density of the groups")
    plt.figure(7)
    plt.title("evolution of the number of occupied nodes")
    plt.figure(8)
    plt.title("evolution of the number of groups / number of nodes in the graphs in permille ")
    
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
        for i,j in enumerate(range(1,9)): 
            if not j == 4 :       
                plt.figure(j)
                plt.plot(dt.T[i], color=c, label=cusname)
    
    for i in range(1,9):
        if not i == 4 :
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
    
