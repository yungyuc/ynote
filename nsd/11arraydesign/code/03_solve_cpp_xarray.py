#!/usr/bin/env python3

import numpy as np

def make_grid():
    nx = 51
    x = np.linspace(0, 1, nx)
    gx, gy = np.meshgrid(x, x)
    u = np.zeros_like(gx)
    u[0,:] = 0
    u[-1,:] = 1 * np.sin(np.linspace(0,np.pi,nx))
    u[:,0] = 0
    u[:,-1] = 0
    return nx, x, u

nx, x, uoriginal = make_grid()

# [begin example]
import solve_cpp_xarray
# [end example]

import time

class Timer:
    def __enter__(self):
        self._t0 = time.time()
    def __exit__(self, *args, **kw):
        self._t1 = time.time()
        print("Wall time: {:g} s".format(self._t1 - self._t0))

# [begin pycon]
with Timer():
    u, step, norm = solve_cpp_xarray.solve_cpp(uoriginal)
# [end pycon]

from matplotlib import pyplot as plt

def show_result(u, step, norm, size=7):
    print("step", step, "norm", norm)
    fig, ax = plt.subplots(figsize=(size,size))
    cs = ax.contour(x, x, u.T)
    ax.clabel(cs, inline=1, fontsize=10)

    ax.set_xticks(np.linspace(0,1,6))
    ax.set_yticks(np.linspace(0,1,6))
    ax.set_xlabel('$x$')
    ax.set_ylabel('$y$')

    ax.grid(True, which='minor')

show_result(u, step, norm)

plt.rc('figure', figsize=(8, 8))

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
