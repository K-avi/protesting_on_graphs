import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("data.csv")
plt.plot(*data.T, "k.")
plt.xlabel("N° lines")
plt.ylabel("temps")
plt.show()
