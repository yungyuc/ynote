#!/usr/bin/env python3

# [begin example]
import numpy as np
from matplotlib import pyplot as plt
import libst

res = 32
xcrd = np.arange(res+1) / res * 2 * np.pi

time_stop = 2*np.pi

grid = libst.Grid(xcrd)
cfl_max = 1.0
dx = (grid.xmax - grid.xmin) / grid.ncelm
dt_max = dx * cfl_max
nstep = int(np.ceil(time_stop / dt_max))
dt = time_stop / nstep
cfl = dt / dx

svr = libst.InviscidBurgersSolver(grid=grid, time_increment=dt)

# Initialize
svr.set_so0(0, np.sin(svr.xctr()))
svr.set_so1(0, np.cos(svr.xctr()))

plt.figure(figsize=(15,10))
plt.xlim((0, 2))
plt.xlabel('$x$ $(\pi)$')
plt.ylabel('$u$')
plt.grid()
plt.plot(svr.xctr() / np.pi, svr.get_so0(0).ndarray, '-', label='begin')

svr.setup_march()
svr.march_alpha2(50)
plt.plot(svr.xctr() / np.pi, svr.get_so0(0).ndarray, '-', label='end')

plt.legend()
# [end example]

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
