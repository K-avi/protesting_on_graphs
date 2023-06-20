#this file will be used to plot the different elements 
#highly contextual script don't use it 
import numpy as np 
import matplotlib.pyplot as plt


plt.figure(1)

dt_align = np.loadtxt("res_100_paris_align_var")
dt_attra = np.loadtxt("res_100_paris_attra_var")
dt_attco = np.loadtxt("res_100_paris_attra_coeff_var")
dt_noisy_align = np.loadtxt("res_100_paris_noisy_align_var")
dt_noisy_attra = np.loadtxt("res_100_paris_noisy_attra_var")

plt.plot(dt_align.T[0], color='r', label='align')
plt.plot(dt_attra.T[0], color='y', label='attra')
plt.plot(dt_attco.T[0], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[0], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[0], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of number of groups")



plt.figure(2)

plt.plot(dt_align.T[1], color='r', label='align')
plt.plot(dt_attra.T[1], color='y', label='attra')
plt.plot(dt_attco.T[1], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[1], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[1], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of mean spread of groups")


plt.figure(3)

plt.plot(dt_align.T[2], color='r', label='align')
plt.plot(dt_attra.T[2], color='y', label='attra')
plt.plot(dt_attco.T[2], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[2], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[2], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of number of walkers per group")


plt.figure(4)

plt.plot(dt_align.T[3], color='r', label='align')
plt.plot(dt_attra.T[3], color='y', label='attra')
plt.plot(dt_attco.T[3], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[3], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[3], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of the mean of different nodes visited by each walker")




plt.figure(5)

plt.plot(dt_align.T[4], color='r', label='align')
plt.plot(dt_attra.T[4], color='y', label='attra')
plt.plot(dt_attco.T[4], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[4], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[4], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of the number of 'groupless walkers'")

plt.show()
