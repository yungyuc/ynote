=============================
NSD Schedule 2021 Autumn NYCU
=============================

.. begin schedule contents

`Link to NYCU course system.
<https://timetable.nycu.edu.tw/?r=main/crsoutline&Acy=110&Sem=1&CrsNo=5296>`__

.. list-table::
  :header-rows: 1
  :align: center
  :width: 100%

  * - Week
    - Date
    - Subject
    - Homework
  * - 1
    - 9/13
    - :doc:`Introduction <../00intro>` and
      Unit 1: :doc:`../01engineering/engineering`
    - assignment #1
  * - 2
    - 9/20
    - No meeting (mid-autumn holiday)
    -
  * - 3
    - 9/27
    - Unit 2: :doc:`../02numpy/numpy`
    - project proposal start
  * - 4
    - 10/4
    - Unit 3: :doc:`../03cpp/cpp`
    - assignment #2
  * - 5
    - 10/11
    - No meeting (national day)
    -
  * - 6
    - 10/18
    - Unit 4: :doc:`../04matrix/matrix`
    - assignment #3
  * - 7
    - 10/25
    - Unit 5: :doc:`../05cache/cache`
    - project proposal due
  * - 8
    - 11/1
    - Unit 6: :doc:`../06simd/simd`
    -
  * - 9
    - 11/8
    - Mid-term examination
    -
  * - 10
    - 11/15
    - Unit 7: :doc:`../07mem/mem`
    - assignment #4
  * - 11
    - 11/22
    - Unit 8: :doc:`../08smart/smart`
    -
  * - 12
    - 11/29
    - Unit 9: :doc:`../09moderncpp/moderncpp`
    - assignment #5
  * - 13
    - 12/6
    - Unit 10: :doc:`../10cpppy/cpppy`
    -
  * - 14
    - 12/13
    - Unit 11: :doc:`../11arraydesign/arraydesign`
    - assignment #6
  * - 15
    - 12/20
    - Unit 12: :doc:`../12advpy/advpy`
    -
  * - 16
    - 12/27
    - Project presentation 1
    -
  * - 17
    - 1/3
    - Project presentation 2
    -
  * - 18
    - 1/10
    - Project presentation 3
    -

.. _nsd-21au-project:

Term Project
============

The course requires students to develop a software projects of a hybrid system
of C++11 (modern C++) and Python for a numerical, scientific, or engineering
problem.  Students need to submit a proposal for the project.  A template for
the proposal can be found at :doc:`nsd_21au_nycu_project_template`.

There are some possible directions listed below.  All of them are useful in a
teaching code `modmesh <https://github.com/solvcon/modmesh>`__.  Students are
free to derive a topic from them, or come up with one by themselves.

.. _nsd-21au-project-conarr:

Contiguous Array
================

N-dimensional arrays of fundamental types and struct are a building block for
numerical code.  It may be as simple as a pointer to a contiguous memory
buffer, or well-designed meta-data with the memory buffer.

In `modmesh <https://github.com/solvcon/modmesh>`__, there is a class template
:cpp:class:`!SimpleArray` implementing an N-dimensional array of contiguous
memory.  It is still in an early stage of development and may use a lot of
enhancements.

.. _nsd-21au-project-columnar:

Columnar Array
==============

The Apache Arrow project provides `a clear definition to the columnar data
<https://arrow.apache.org/docs/format/Columnar.html>`__.  Columnar data are an
application of contiguous buffer, and provide a way to store flexible data
format while providing high performance.

Because it is based on contiguous buffer, it is slow in insertion and resizing.
But on the other hand, it provides constant-time random access and is friendly
to cache optimization and SIMD (vector processing).

.. _nsd-21au-project-graphpart:

Graph Partitioning
==================

Numerical solution of partial differential equations (PDEs) depends on
discretization of space.  The entities describing the discretized space is
called mesh or grid.  The mesh can be broadly categorized into structured and
unstructured mesh.  The latter is more flexible than the former.

The unstructured mesh allows free connectivity, which enables flexible
distribution of data for parallel computing.  The connectivity between mesh
elements can be represented as a graph for partitioning.  The
graph-partitioning problem is useful to minimizing the communication between
sub-mesh.  There have been codes developed for this, e.g., `METIS
<http://glaros.dtc.umn.edu/gkhome/views/metis>`__, and `SCOTCH
<https://www.labri.fr/perso/pelegrin/scotch/>`__.

.. _nsd-21au-project-polybool:

Boolean Operations on Polygons
==============================

In Euclidean space we are interested in finding the Boolean, i.e., AND, OR,
NOT, XOR, of polygons.  The polygonal Boolean operations are useful when we
want to extract geometrical properties of the graphics.  In two-dimensional
space we deal with polygons.  In three-dimensional space it is polyhedra.  The
Boolean operations are most useful in the two-dimensional space.

.. _nsd-21au-project-rtree:

R-Tree Search Engine
====================

R-tree is an index to speed up searches in space.  It is usually referred to as
a spatial index or just a tree.  Data structures of the similar purpose include
k-d tree, quadtree, etc.  There is `an R-tree implementation in boost
<https://www.boost.org/doc/libs/1_77_0/libs/geometry/doc/html/index.html>`__.

.. _nsd-21au-project-voronoi:

Voronoi Diagram
===============

The Voronoi diagram is a decomposition of a region that any point in a
sub-region is closest to the site of the sub-region.  This problem is useful
when we are interested in distance to some sites in an Euclidean space.  For
example, we can use the Voronoi diagram to estimate the service areas of each
branch of a reseller chain.

The Voronoi diagram will also be used to create triangular mesh in the Delaunay
triangulation.

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2 tw=79:
