#this file was written by https://github.com/Pacidus
import numpy as np
import subprocess as sb

from time import time

N = 30
rmin = np.log10(100)
rmax = np.log10(5000)
while True:
    r = int(10**(rmin + (np.random.rand(1)[0]*(rmax-rmin))))
    print(r)

    sb.run(f"python ../city_graph/graph_treament.py 48.866 2.33 {r} 10 test.csv", shell=True)
    with open("test.csv") as NTM:
        fl = NTM.readline()

    fl = float(fl.split(",")[0])
    print(fl)
    tb = time()
    sb.run("../test", shell=True)
    tf = time()

    data = [fl, tf - tb]
    print(data[1])
    with open("data.csv", "a") as NTM:
        np.savetxt(NTM, [data])
