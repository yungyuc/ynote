==============================
Numerical Software Development
==============================

.. admonition:: Work in Progress

  This note is being adapted from `the lecture note of numerical software
  development <https://yungyuc.github.io/nsd>`__.

This is a note introducing the popular hybrid architecture that uses Python and
C++ to develop numerical software systems.  It has two parts.  The :ref:`first
<nsd-first-part>` is the recapitulation of how computers work.  The
:ref:`second <nsd-second-part>` is the practical implementation skill.  An
:doc:`overview <00intro>` is provided before the details.

.. toctree::
  :maxdepth: 1
  :hidden:

  00intro

.. _nsd-first-part:

.. toctree::
  :caption: Computer Architecture
  :maxdepth: 1

  01engineering/index
  02numpy/index
  03cpp/index
  04matrix/index
  05cache/index
  06simd/index

.. _nsd-second-part:

.. toctree::
  :caption: Implementation Skill
  :maxdepth: 1

  07mem/index
  08smart/index
  09moderncpp/index
  10cpppy/index
  11arraydesign/index
  12advpy/index

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2 tw=79:
