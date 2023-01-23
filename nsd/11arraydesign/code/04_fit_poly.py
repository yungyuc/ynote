#!/usr/bin/env python3

import numpy as np
from matplotlib import pyplot as plt

import time

class Timer:
    def __enter__(self):
        self._t0 = time.time()
    def __exit__(self, *args, **kw):
        self._t1 = time.time()
        print("Wall time: {:g} s".format(self._t1 - self._t0))

print("prep")
# [begin prep pycon]
with Timer():
    # np.unique returns a sorted array.
    xdata = np.unique(np.random.sample(1000000) * 1000)
    ydata = np.random.sample(len(xdata)) * 1000
# [end prep pycon]

# [begin fit_poly pycon]
import data_prep

plt.rc('figure', figsize=(12, 8))

def plot_poly_fitted(i):
    slct = (xdata>=i)&(xdata<(i+1))
    sub_x = xdata[slct]
    sub_y = ydata[slct]
    poly = data_prep.fit_poly(sub_x, sub_y, 3)
    print(poly)
    poly = np.poly1d(poly)
    xp = np.linspace(sub_x.min(), sub_x.max(), 100)
    plt.plot(sub_x, sub_y, '.', xp, poly(xp), '-')

plot_poly_fitted(10)
# [end fit_poly pycon]

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

print("lumped")
# [begin lumped pycon]
with Timer():
    # Do the calculation for the 1000 groups of points.
    polygroup = np.empty((1000, 3), dtype='float64')
    for i in range(1000):
        # Use numpy to build the point group.
        slct = (xdata>=i)&(xdata<(i+1))
        sub_x = xdata[slct]
        sub_y = ydata[slct]
        polygroup[i,:] = data_prep.fit_poly(sub_x, sub_y, 2)
# [end lumped pycon]

print("point build")
# [begin point build pycon]
with Timer():
    # Using numpy to build the point groups takes a lot of time.
    data_groups = []
    for i in range(1000):
        slct = (xdata>=i)&(xdata<(i+1))
        data_groups.append((xdata[slct], ydata[slct]))
# [end point build pycon]

print("fitting")
# [begin fitting pycon]
with Timer():
    # Fitting helper runtime is much less than building the point groups.
    polygroup = np.empty((1000, 3), dtype='float64')
    for it, (sub_x, sub_y) in enumerate(data_groups):
        polygroup[it,:] = data_prep.fit_poly(sub_x, sub_y, 2)
# [end fitting pycon]

print("batch")
# [begin batch pycon]
with Timer():
    rbatch = data_prep.fit_polys(xdata, ydata, 2)
# [end batch pycon]

print(rbatch.shape)
# Verify batch.
assert len(rbatch) == len(polygroup)
for i in range(1000):
    assert (rbatch[i] == polygroup[i]).all()

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
