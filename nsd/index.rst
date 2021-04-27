==============================
Numerical Software Development
==============================

.. admonition:: Work in Progress

  This note is being adapted from `the lecture note of numerical software
  development <https://yungyuc.github.io/nsd>`__.

This is a note introducing the popular hybrid architecture that uses Python and
C++ to develop numerical software systems.  It has two parts.  The :ref:`first
<nsd-first-part>` is the recapitulation of how computers work.  The
:ref:`second <nsd-second-part>` is the practical implementation skill.

.. toctree::
  :caption: Overview
  :maxdepth: 1

  00intro
  schedule/schedule

.. _nsd-first-part:

.. toctree::
  :caption: Computer Architecture
  :maxdepth: 1

  01engineering/engineering
  02numpy/numpy
  03cpp/cpp
  04matrix/matrix
  05cache/cache
  06simd/simd

.. _nsd-second-part:

.. toctree::
  :caption: Implementation Skill
  :maxdepth: 1

  07mem/mem
  08smart/smart
  09moderncpp/moderncpp
  10cpppy/cpppy
  11arraydesign/arraydesign
  12advpy/advpy

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2 tw=79:
