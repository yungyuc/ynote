#!/usr/bin/env python3

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

nx, x, uoriginal = make_grid()

import time
import numpy as np

class Timer:
    def __enter__(self):
        self._t0 = time.time()
    def __exit__(self, *args, **kw):
        self._t1 = time.time()
        print("Wall time: {:g} s".format(self._t1 - self._t0))

# [begin example]
def solve_python_loop():
    u = uoriginal.copy()  # Input from outer scope
    un = u.copy()  # Create the buffer for the next time step
    converged = False
    step = 0
    # Outer loop.
    while not converged:
        step += 1
        # Inner loops. One for x and the other for y.
        for it in range(1, nx-1):
            for jt in range(1, nx-1):
                un[it,jt] = (u[it+1,jt] + u[it-1,jt] + u[it,jt+1] + u[it,jt-1]) / 4
        norm = np.abs(un-u).max()
        u[...] = un[...]
        converged = True if norm < 1.e-5 else False
    return u, step, norm
# [end example]

with Timer():
    u, step, norm = solve_python_loop()

print(step)

# Run the Python solver
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
