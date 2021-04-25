===============================
Example Code: Memory Management
===============================

.. literalinclude:: code/cmem.c
  :name: nsd-mem-example-cmem
  :caption:
    Example code for C memory management (:download:`cmem.c <code/cmem.c>`).
  :language: c
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-mem-example-cmem-build
  :caption: 
    Build :ref:`cmem.c <nsd-mem-example-cmem>`.

  $ gcc cmem.c -o cmem -std=c11 -O3 -g

.. literalinclude:: code/cppmem.cpp
  :name: nsd-mem-example-cppmem
  :caption:
    Example code for C++ memory management (:download:`cppmem.cpp
    <code/cppmem.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-mem-example-cppmem-build
  :caption: 
    Build :ref:`cppmem.cpp <nsd-mem-example-cppmem>`.

  $ g++ cppmem.cpp -o cppmem -std=c++17 -O3 -g

.. literalinclude:: code/alloc.cpp
  :name: nsd-mem-example-alloc
  :caption:
    A simple C++ STL allocator that keeps track of bytes allocated
    (:download:`alloc.cpp <code/alloc.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. literalinclude:: code/icount.cpp
  :name: nsd-mem-example-icount
  :caption:
    Template to help keep track of the number of living instances
    (:download:`icount.cpp <code/icount.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. add 10 blank lines to avoid messing up with the vim modeline.











.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
