#!/usr/bin/env python3

"""
This script counts the number of line of a text file.  This is intended to be
run as a script and not suitable for being imported.
"""


import sys
import os.path


if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))

# vim: set ff=unix ft=python et sw=4 ts=4 sts=4:
