#!/usr/bin/env python3

# [begin example]
import numpy as np
import matplotlib.pyplot as plt

poly = np.poly1d(np.array([5.35749, -2.04348, 12.5266, 0], dtype='float64'))
xin = np.array([1, 2, 3, 4], dtype='float64')
yin = np.array([18, 68, 168, 336], dtype='float64')
xp = np.linspace(1, 4, 100)

plt.rc('figure', figsize=(12, 8))
plt.plot(xin, yin, '.', xp, poly(xp), '-')
plt.xlim(0, 5)
plt.ylim(0, 400)
plt.xlabel('x')
plt.ylabel('y')
plt.grid()
# [end example]

import os
imagedir = os.path.join(os.path.dirname(__file__), '..', 'image')
imagebase = os.path.splitext(os.path.basename(__file__))[0] + '.png'
imagepath = os.path.join(imagedir, imagebase)
print('write to {}'.format(imagepath))
plt.savefig(imagepath, dpi=150)

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4 tw=79:
