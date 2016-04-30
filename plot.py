import numpy as np
import matplotlib.pyplot as plt
import ipdb; ist = ipdb.set_trace

hist = np.loadtxt('hist.dat')

dr = 0.1
rmax = 25
NFRAMES = 40
N = 10000
L = 24.6402
BOXV = L*L*L
NPAIRS = N*(N-1)
pair_rho = NPAIRS/BOXV
r = np.arange(dr,rmax+dr,dr)
r0 = np.insert(r,0,0)
BINVOL = (4.0/3.0) * np.pi * (r0[1:]**(3.0) - r0[:-1]**(3.0))

gr  = hist/(NFRAMES*BINVOL*pair_rho)
plt.plot(r,gr)
plt.show()

