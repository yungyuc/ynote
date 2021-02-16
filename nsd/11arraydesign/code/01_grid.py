#!/usr/bin/env python3

# [begin example]
import numpy as np
from matplotlib import pyplot as plt

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

def show_grid(size):
    fig, ax = plt.subplots(figsize=(size,size))
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.set_xticks(x, minor=True)
    ax.set_yticks(x, minor=True)
    ax.set_xlabel('$x$')
    ax.set_ylabel('$y$')
    ax.grid(True, which='minor')

nx, x, uoriginal = make_grid()
show_grid(10)
# [end example]

plt.rc('figure', figsize=(8, 8))

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
