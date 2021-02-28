"""
The line-counting code is made as a module, so that when it is imported as a
module the code doesn't get executed immediately.
"""


# Python wants the following list to include all names of the objects that will
# be considered public from outside this module.
__all__ = [
    'count_line',
]


import sys
import os.path


def count_line(fname):
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))


# This tests whether the file is evaluated as a script.
if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.stdout.write('missing file name\n')
    elif len(sys.argv) > 2:
        sys.stdout.write('only one argument is allowed\n')
    else:
        count_line(sys.argv[1])

# vim: set ff=unix fenc=utf8 ft=python et sw=4 ts=4 sts=4 tw=79:
