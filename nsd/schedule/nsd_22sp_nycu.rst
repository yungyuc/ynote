=============================
NSD Schedule 2022 Spring NYCU
=============================

.. begin schedule contents

.. `Link to NYCU course system.
.. <https://timetable.nycu.edu.tw/?r=main/crsoutline&Acy=110&Sem=1&CrsNo=5296>`__

.. list-table::
  :header-rows: 1
  :align: center
  :width: 100%

  * - Week
    - Date
    - Subject
    - Comments
  * - 1
    - 2/14
    - :doc:`Introduction <../00intro>` and
      Unit 1: :doc:`../01engineering/engineering`
    - hw #1
  * - 2
    - 2/21
    - Unit 2: :doc:`../02numpy/numpy`
    - proposal open
  * - 3
    - 2/28
    - No meeting (228 memorial)
    -
  * - 4
    - 3/7
    - Unit 3: :doc:`../03cpp/cpp`
    - hw #2
  * - 5
    - 3/14
    - Unit 4: :doc:`../04matrix/matrix`
    -
  * - 6
    - 3/21
    - Unit 5: :doc:`../05cache/cache`
    - hw #3
  * - 7
    - 3/28
    - Unit 6: :doc:`../06simd/simd`
    -
  * - 8
    - 4/4
    - No meeting (children's day)
    - proposal due
  * - 9
    - 4/11
    - Mid-term examination
    -
  * - 10
    - 4/18
    - Unit 7: :doc:`../07mem/mem`
    - hw #4
  * - 11
    - 4/25
    - Unit 8: :doc:`../08smart/smart`
    -
  * - 12
    - 5/2
    - Unit 9: :doc:`../09moderncpp/moderncpp`
    - hw #5
  * - 13
    - 5/9
    - Unit 10: :doc:`../10cpppy/cpppy`
    -
  * - 14
    - 5/16
    - Unit 11: :doc:`../11arraydesign/arraydesign`
    - hw #6
  * - 15
    - 5/23
    - Unit 12: :doc:`../12advpy/advpy`
    -
  * - 16
    - 5/30
    - Project presentation
    -
  * - 17
    - 6/6
    - To be planned
    -
  * - 18
    - 6/13
    - To be planned
    -

.. _nsd-22sp-project:

Term Project
============

The course requires students to develop a software projects of a hybrid system
of C++11 (modern C++) and Python for a numerical, scientific, or engineering
problem.  Everyone needs to *individually* write a proposal, develop the code,
and present the project to the class at the end of the course.  The grading
guidelines are described in :doc:`../project/project`.

The software needs to be open-source, hosted on `github.com
<https://github.com/>`__, and executable on `Ubuntu 20.04 LTS
<http://releases.ubuntu.com/20.04/>`__ on 64-bit x86 through command line.
Building the software should use a single command.

The project proposal should be submit through `the homework repository
<https://github.com/yungyuc/nsdhw_22sp>`__ on time.  A template can be found at
:doc:`nsd_22sp_nycu_project_template`.  The following contents should be
included:

1. Basic information (including the GitHub repository)
2. Problem to solve
3. Perspective users
4. System architecture
5. API description
6. Engineering infrastructure
7. Schedule

The proposal works like a specification, of which the purpose is to enable
discussions that cannot be done with programming language.  For example, source
code is not suitable for describing software architecture.  In `The
Architecture of Open Source Applications
<https://aosabook.org/en/index.html>`__, you can see the many different ways
that the developers use to present architecture.  It is usually effective to
use diagrams and natural language to do it.

With your proposal, I can help you manage the development through discussions
(at which you should be pro-active).  A plan will not be be 100% accurate and
you should modify it as you go.  Use pull requests to keep the proposal
up-to-date.

You should write prototype code for your project while writing the proposal.
The initial work will help you understand more about what to do.  It is
difficult, if not impossible, to write a proposal without prototyping.

There are some possible directions listed in this page.  All of them are useful
in a code name `modmesh <https://github.com/solvcon/modmesh>`__.  Students are
free to derive a subject from them, or come up with one by themselves.

.. _nsd-22sp-project-conarr:

Contiguous Array
================

N-dimensional arrays of fundamental types and struct are a building block for
numerical code.  It may be as simple as a pointer to a contiguous memory
buffer, or well-designed meta-data with the memory buffer.

In `modmesh <https://github.com/solvcon/modmesh>`__, there is a class template
:cpp:class:`!SimpleArray` implementing an N-dimensional array of contiguous
memory.  It is still in an early stage of development and may use a lot of
enhancements.

A sample project proposal in this direction can be found in
:doc:`nsd_22sp_nycu_project_simple_array`.

.. _nsd-22sp-project-columnar:

Columnar Array
==============

The Apache Arrow project provides `a clear definition to the columnar data
<https://arrow.apache.org/docs/format/Columnar.html>`__.  Columnar data are an
application of contiguous buffer, and provide a way to store flexible data
format while providing high performance.

Because it is based on contiguous buffer, it is slow in insertion and resizing.
But on the other hand, it provides constant-time random access and is friendly
to cache optimization and SIMD (vector processing).

.. _nsd-22sp-project-graphpart:

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

.. _nsd-22sp-project-polybool:

Boolean Operations on Polygons
==============================

In Euclidean space we are interested in finding the Boolean, i.e., AND, OR,
NOT, XOR, of polygons.  The polygonal Boolean operations are useful when we
want to extract geometrical properties of the graphics.  In two-dimensional
space we deal with polygons.  In three-dimensional space it is polyhedra.  The
Boolean operations are most useful in the two-dimensional space.

.. _nsd-22sp-project-rtree:

R-Tree Search Engine
====================

R-tree is an index to speed up searches in space.  It is usually referred to as
a spatial index or just a tree.  Data structures of the similar purpose include
k-d tree, quadtree, etc.  There is `an R-tree implementation in boost
<https://www.boost.org/doc/libs/1_77_0/libs/geometry/doc/html/index.html>`__.

.. _nsd-22sp-project-voronoi:

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
