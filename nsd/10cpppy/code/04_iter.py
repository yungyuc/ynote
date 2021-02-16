#!/usr/bin/env python3

# [begin example]
import numpy as np
from matplotlib import pyplot as plt
import libst

grid = libst.Grid(0, 4*2*np.pi, 4*64)
cfl = 1
dx = (grid.xmax - grid.xmin) / grid.ncelm
dt = dx * cfl
svr = libst.LinearScalarSolver(grid=grid, time_increment=dt)

# Initialize
# NOTE: 'selms' returns a template instance of SolverElementIterator
for e in svr.selms(odd_plane=False):
    if e.xctr < 2*np.pi or e.xctr > 2*2*np.pi:
        v = 0
        dv = 0
    else:
        v = np.sin(e.xctr)
        dv = np.cos(e.xctr)
    e.set_so0(0, v)
    e.set_so1(0, dv)

# Plot it
plt.figure(figsize=(15,10))
plt.xlim((0, 8))
plt.xlabel('$x$ $(\pi)$')
plt.ylabel('$u$')
plt.grid()
plt.plot(svr.xctr() / np.pi, svr.get_so0(0).ndarray, '-')
# [end example]

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
