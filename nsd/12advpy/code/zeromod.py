#!/usr/bin/env python3

# [begin example]
import os
import importlib.abc
import importlib.machinery

class MyMetaPathFinder(importlib.abc.MetaPathFinder):
    def find_spec(self, fullname, path, target=None):
        if fullname == 'onemod':
            print('DEBUG: fullname: {} , path: {} , target: {}'.format(
                fullname, path, target))
            fpath = os.path.abspath('altdir/onemod.py')
            loader = importlib.machinery.SourceFileLoader('onemod', fpath)
            return importlib.machinery.ModuleSpec(fullname, loader, origin=fpath)
        else:
            return None
# [end example]

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
