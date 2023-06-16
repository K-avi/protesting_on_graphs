#this file will be used to plot the different elements 
import numpy as np 
import matplotlib.pyplot as plt


def load_trace(path):
    dt= np.loadtxt(path)
    test =dt.T[0]
    print(test)
    plt.plot(test,"k.")
    plt.xlabel("iteration")
    plt.ylabel("nb gp")
    plt.show()

if __name__=='__main__':
    load_trace("res_mean1")