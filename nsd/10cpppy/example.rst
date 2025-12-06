==================================
Example Code: C++ and C for Python
==================================

.. literalinclude:: code/01_linear.py
  :name: nsd-cpppy-example-linear
  :caption:
    Use :py:mod:`!libst` to solve a propagating linear wave
    (:download:`01_linear.py <code/01_linear.py>`).
  :language: python
  :linenos:
  :end-before: # vim: set

.. literalinclude:: code/pybind11_python_example/setup.py
  :name: nsd-cpppy-example-setup
  :caption:
    ``setup.py`` for pybind11 (:download:`setup.py
    <code/pybind11_python_example/setup.py>`).
  :language: python
  :linenos:
  :end-before: # vim: set

.. literalinclude:: code/pybind11_python_example/main.cpp
  :name: nsd-cpppy-example-setup-main
  :caption:
    Simple C++ code for using pybind11 (:download:`main.cpp
    <code/pybind11_python_example/main.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-cpppy-example-setup-build
  :caption:
    Build :ref:`setup.py <nsd-cpppy-example-setup>` with :ref:`main.cpp
    <nsd-cpppy-example-setup-main>`.

  $ python3 setup.py build_ext --inplace

.. literalinclude:: code/04_iter.py
  :name: nsd-cpppy-example-iter
  :caption:
    Show how the C++-to-Python adapter for iterator works
    (:download:`04_iter.py <code/04_iter.py>`).
  :language: python
  :linenos:
  :end-before: # vim: set

.. add 10 blank lines to avoid messing up with the vim modeline.











.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
