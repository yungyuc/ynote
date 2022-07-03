#!/usr/bin/env python3

import inspect

module_specific = "module data"

def main():
    function_local = "local data"

    f = inspect.currentframe()

    # Show the keys in the builtins namespace
    print(sorted(f.f_builtins.keys()))

    # Commonly imported modules are global
    assert "inspect" in f.f_globals
    # Commonly defined functions are global
    assert "main" in f.f_globals
    # The "global" variables are actually module-level
    assert "module_specific" in f.f_globals
    assert "module data" == f.f_globals["module_specific"]

    # The local variables are in the local namespace
    assert "function_local" in f.f_locals
    assert "local data" == f.f_locals["function_local"]

    # Local symbol is not in globals
    assert "function_local" not in f.f_globals
    # "Global" variables are not in local namespace
    assert "module_specific" not in f.f_locals

    # f_builtins is exactly the __builtins__ available at the frame
    assert f.f_builtins == __builtins__.__dict__
    assert f.f_builtins is __builtins__.__dict__
    # f_globals is exactly the globals() available at the frame
    assert f.f_globals == globals()
    assert f.f_globals is globals()
    # f_locals is exactly the locals() available at the frame
    assert f.f_locals == locals()
    assert f.f_locals is locals()

    del f

main()

# [end example]

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
