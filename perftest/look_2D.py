#this file was written by https://github.com/Pacidus
import numpy as np
import matplotlib.pyplot as plt

w, e, t = np.log10(np.loadtxt("2D_data.csv").T)
bins = [13, 30]

bin_count = np.histogram2d(w, e, bins=bins)[0]
T, x, y = np.histogram2d(w, e, weights=t, bins=bins)
T /=bin_count

X, Y = np.meshgrid(x, y)

plt.pcolor(X.T, Y.T, T, cmap="RdYlBu")
plt.colorbar(label="time")
plt.xlabel("# Walker")
plt.ylabel("# Edges")
plt.legend()
plt.show()
