#this file was written by https://github.com/Pacidus
import numpy as np
import subprocess as sb

from time import time

rmin = 2*np.log10(100)
rmax = 2*np.log10(5000)
nmin = np.log10(100)
nmax = np.log10(100000)
while True:
    r = int(np.sqrt(np.power(10, np.random.uniform(rmin, rmax))))
    N = int(np.power(10, np.random.uniform(nmin, nmax)))
    print(r, N)

    sb.run(f"python ../city_graph/graph_treament.py 48.866 2.33 {r} 10 test.csv", shell=True)
    with open("test.csv") as NTM:
        fl = NTM.readline()

    fl = float(fl.split(",")[0])
    print(fl)

    tb = time()
    sb.run(f"./wog0906 test.csv {N} 1000", shell=True)
    tf = time()

    data = [N, fl, tf - tb]
    print(data[2])
    with open("2D_data.csv", "a") as NTM:
        np.savetxt(NTM, [data])
