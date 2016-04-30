import numpy as np
import matplotlib.pyplot as plt
import ipdb; ist = ipdb.set_trace

hist = np.loadtxt('corr.out')

plt.plot(hist[:,0],hist[:,3])
plt.show()

