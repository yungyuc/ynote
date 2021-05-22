#!/usr/bin/env python3

from matplotlib import pyplot as plt

# [begin example]
import numpy as np

def make_grid():
    # Number of the grid points.
    nx = 51
    # Produce the x coordinates.
    x = np.linspace(0, 1, nx)
    # Create the two arrays for x and y coordinates of the grid.
    gx, gy = np.meshgrid(x, x)
    # Create the field and zero-initialize it.
    u = np.zeros_like(gx)
    # Set the boundary condition.
    u[0,:] = 0
    u[-1,:] = 1 * np.sin(np.linspace(0,np.pi,nx))
    u[:,0] = 0
    u[:,-1] = 0
    # Return all values.
    return nx, x, u

nx, x, uoriginal = make_grid()
# [end example]

def show_grid(size):
    fig, ax = plt.subplots(figsize=(size,size))
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.set_xticks(x, minor=True)
    ax.set_yticks(x, minor=True)
    ax.set_xlabel('$x$')
    ax.set_ylabel('$y$')
    ax.grid(True, which='major', linewidth=2)
    ax.grid(True, which='minor', linewidth=1)

show_grid(10)

plt.rc('figure', figsize=(8, 8))

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
