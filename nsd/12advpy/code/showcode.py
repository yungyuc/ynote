#!/usr/bin/env python3

import inspect

def main():
    f = inspect.currentframe()

    # Get the code object
    c = f.f_code
    
    # List all fields in the code object
    codekeys = sorted(k for k in dir(c) if not k.startswith("__"))
    print("all fields in the code object:")
    print("  {}".format(codekeys))
    print("value in the fields:")
    for k in codekeys:
        print("  {}:".format(k), getattr(c, k))

    # Show the function name and argument count
    print("name and argcount:", c.co_name, c.co_argcount)
    # Show file name and line number
    print("filename, line number:", c.co_filename, c.co_firstlineno)

main()

# [end example]

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
