#!/usr/bin/env python3

import sys
sys.path.insert(0, 'modmesh-master')

# [begin pycon]
import modmesh as mm

gd = mm.StaticGrid1d(1000000)
for it in range(100):
    gd.fill(0)
print(mm.time_registry.report())
# [end pycon]

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
