"""
Take the "main" code out into a real function main().  For short code it's
optional.  For long code it helps keep the code organized.
"""


# Python wants the following list to include all names of the objects that will
# be considered public from outside this module.
__all__ = [
    'count_line',
    'main',
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


def main():
    if len(sys.argv) < 2:
        sys.stdout.write('missing file name\n')
    elif len(sys.argv) > 2:
        sys.stdout.write('only one argument is allowed\n')
    else:
        count_line(sys.argv[1])


# This tests whether the file is evaluated as a script.
if __name__ == '__main__':
    main()

# vim: set ff=unix ft=python et sw=4 ts=4 sts=4:
