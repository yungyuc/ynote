#!/usr/bin/env python3

import traceback

def f1():
    traceback.print_stack()

def f2():
    f1()

def f3():
    f2()

# [end example]

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
