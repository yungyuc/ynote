===================================================================
(A Sample Project Proposal) SimpleArray for Multi-Dimensional Field
===================================================================

(Modmesh) SimpleArray: a simple C++ library for multi-dimensional arrays that
provide seamless interface with numpy ndarray.  :download:`Download this file.
<project_simple_array.rst>`

Basic Information
=================

SimpleArray is part of modmesh, and the code is house in its repository:
https://github.com/solvcon/modmesh

Numerical analysis and high-performance computing (HPC) heavily rely on
multi-dimensional arrays, or contiguous memory buffers on which meta data
attach.  For the case of the modmesh library, it focuses on field calculations.
There are two major styles for storing the (multi-dimensional) field data in
arrays:

1. For an N-dimensional field, use an N-dimensional array.  Each of the axis in
   the field corresponds to a distinct axis in the array.
2. Store the coordinates in an M x N arrays, where M is the number of
   coordinates and N is the number of dimensions.  Use lookup tables (LUT) to
   keep track of the connectivities among the coordinates as well as the
   multi-dimensional field.

Problem to Solve
================

The primary problem SimpleArray tries to solve is to share memory buffers
between Python (numpy) and C++.

Multi-dimensional arrays are high-speed data structure optimized for reading
and writing but slow for resizing.  Programmers try to minimize array copying
because it is time-consuming.  Numpy provides a fast and easy-to-use
implementation of the multi-dimensional array, which is called
:py:class:`numpy:numpy.ndarray` (N-dimensional array).  However, using ndarray
must include the whole (C)Python and numpy runtime, which depends on the Python
memory management system.

Because modmesh focuses on field calculations, SimpleArray must make them fast
and easy.  Field can be scalar or vector, and SimpleArray should provide
fundamental helpers for both.  The array may also be used as building blocks
for the internal buffers of other constructs like trees optimized for reading.

Prospective Users
=================

The field calculations by the modmesh library are for computational mechanics.
Numerical methods like the space-time conservation element and solution element
method, finite volume method, finite element method, etc., may take advantage
of the array features.

System Architecture
===================

Modmesh SimpleArray will be developed as a *simple* counterpart in C++ and does
not depend on Python.  It can share the memory buffer with numpy, or use the
memory buffer managed by numpy ndarray.  Users are enabled to take an ndarray
to C++ without data copy, and use C++ to write fast code operating the memory
buffer, and have the results available in Python.

.. note::

  By *simple*, SimpleArray does not try to implement all features of ndarray.

A class template :cpp:class:`!SimpleArray` is holding a smart pointer to an
untyped memory buffer as well as an object for the array meta data, including
shape, stride, etc.  The untyped memory buffer is managed by a class
:cpp:class:`ConcreteBuffer`.

In C++, because :cpp:class:`!SimpleArray` is a class template, there is not
runtime overhead in determining the element types.

In Python, the C++ class template :cpp:class:`!SimpleArray` will have multiple
instantiations for each of the fundamental types supported.

It is not sure how to supported composition types (``struct``).  At the moment,
it is left unspecified.

API Description
===============

Both :cpp:class:`!SimpleArray` and :cpp:class:`!ConcreteBuffer` are under the
namespace ``modmesh``.  They will have constructors defined, including copy and
movement.

Multi-dimensional element accessors will be provided in C++ and Python.  The
C++ element accessors will be implemented for no overheads, but the Python
counterparts will not try to optimize.  For high-speed access of the memory
buffers from Python, SimpleArray will provide an ndarray wrapper.

SimpleArray can be constructed from C++ and Python.  When it is constructed
from Python, element type names will be part of the (Python) class.

Engineering Infrastructure
==========================

The existing unit-testing framework in the modmesh system will be used.  Unit
tests will be provided in Python.  Public API will be wrapped to Python for
testing.  Github Action is configured for running the tests regularly and for
each check-in and pull request.  See https://github.com/solvcon/modmesh/actions
for the CI history.

All new features developed will have corresponding unit tests.

Correctness will be verified by using numpy: computation carried out in C++
should produce exactly identical results to that in numpy.

Schedule
========

Week 1 (mm/dd):
  Review the existing unit tests and CI runs.  Start the presentation skeleton.
  Contents will be added to the presentation along with the code development.

Week 2 (mm/dd):
  Create a universal Python constructor (or factory function) for the different
  fundamental element types.

Week 3 (mm/dd):
  Make automatic conversion between ndarray in Python and the SimpleArray
  template in C++.

Week 4 (mm/dd):
  Refactoring week (and accommodation of overflown work).

Week 5 (mm/dd):
  Make a centralized memory manager for the arrays and the underneath buffers.

Week 6 (mm/dd):
  Develop a simple scoped-based profiling system that hooks into the pybind11
  wrapping code for modmesh and SimpleArray.

Week 7 (mm/dd):
  Refactoring week (and accommodation of overflown work).

Week 8 (mm/dd):
  Overflown work.  If everything goes well, prototype a 2D unstructured mesh by
  porting from the old SOLVCON code base.

References
==========

* SOLVCON (part of it is a predecessor of modmesh):
  https://github.com/solvcon/solvcon
