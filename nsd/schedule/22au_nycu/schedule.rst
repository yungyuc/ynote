=============================
NSD Schedule 2022 Autumn NYCU
=============================

.. begin schedule contents

`Link to NYCU course system.
<https://timetable.nycu.edu.tw/?r=main/crsoutline&Acy=111&Sem=1&CrsNo=535704>`__

The course introduces the art of building numerical software, i.e., computer
programs applying numerical methods for solving mathematical, scientific, or
engineering problems.  We will be using Python, C++ and other tools (e.g., bash,
git, make, etc.) to learn the modern development processes.  This is a practical
course for making software.  The grade composition is: homework 30%, mid-term
exam 30%, and term project 40%.

Practical skills can only be acquired by practicing, and that is why the project
you are requested to define and implement takes 40%.  If you are not familiar
with self-learning, it will be difficult for you, so I would like to offer
additional help.  One of the help is the `sciwork community
<https://sciwork.dev>`__.  It is set up to help people who want to write
scientific code.  I understand that it may be difficult for you to initiate a
project alone.  You might not know exactly where to start or how to effectively
discuss.  The community is a place you can ask for help.  Please join the
discord server https://discord.gg/6MAkFrD and follow the rules.  (If you simply
want to make friends who care about numerical software, welcome too.)

The second thing I would like you to know is the practicing homework assignment
(number 0).  You are required to submit homework assignments using both GitHub
PR and E3.  I created the homework #0 to help you practice the use of GitHub,
and a demonstration is provided at https://github.com/yungyuc/nsdhw_22au/pull/1.
Should you have any questions, file an issue on GitHub, send an email to the
grader or me, or send a message in the private telegram chatroom.  (Preference
is in that order and do not send homework assignments to the sciwork community.)

Should you have any questions about the course, please send me :ref:`an email
<contact>` with the subject line starting with ``[nsd-course]``.  Email is a
great tool for serious discussions.  Please use more of it.

.. list-table::
  :header-rows: 1
  :align: center
  :width: 100%

  * - Week
    - Date
    - Subject
    - Comments
  * - 1
    - 9/12
    - :doc:`Introduction <../../00intro>`
    - hw #1
  * - 2
    - 9/19
    - Unit 1: :doc:`../../01engineering/engineering`
    - proposal open
  * - 3
    - 9/26
    - Unit 2: :doc:`../../02numpy/numpy`
    -
  * - 4
    - 10/3
    - Unit 3: :doc:`../../03cpp/cpp`
    - hw #2
  * - 5
    - 10/10
    - No meeting (national day)
    -
  * - 6
    - 10/17
    - Unit 4: :doc:`../../04matrix/matrix`
    -
  * - 7
    - 10/24
    - Unit 5: :doc:`../../05cache/cache`
    - hw #3
  * - 8
    - 10/31
    - Unit 6: :doc:`../../06simd/simd`
    - proposal due
  * - 9
    - 11/7
    - Mid-term examination
    -
  * - 10
    - 11/14
    - Unit 7: :doc:`../../07mem/mem`
    - hw #4
  * - 11
    - 11/21
    - Unit 8: :doc:`../../08smart/smart`
    -
  * - 12
    - 11/28
    - Unit 9: :doc:`../../09moderncpp/moderncpp`
    - hw #5
  * - 13
    - 12/5
    - Unit 10: :doc:`../../10cpppy/cpppy`
    -
  * - 14
    - 12/12
    - Unit 11: :doc:`../../11arraydesign/arraydesign`
    - hw #6
  * - 15
    - 12/19
    - Unit 12: :doc:`../../12advpy/advpy`
    -
  * - 16
    - 12/26
    - Project presentation
    -
  * - 17
    - 1/2
    - No meeting (new year)
    -
  * - 18
    - 1/9
    - To be planned
    -

.. _nsd-22au-project:

Term Project
============

The course requires students to develop a software system of a hybrid system of
C++11 (modern C++) and Python for a numerical, scientific, or engineering
problem.  Everyone needs to *individually* write a proposal, develop the code,
and present the project to the class at the end of the course.  The grading
guidelines are described in :doc:`../../project/project`.

The software needs to be open-source, hosted on `github.com
<https://github.com/>`__, and executable on `Ubuntu 20.04 LTS
<http://releases.ubuntu.com/20.04/>`__ on 64-bit x86 through command line.
Building the software should use a single command.

The project proposal should be submit through `the homework repository
<https://github.com/yungyuc/nsdhw_22au>`__.  Please follow
:doc:`project_template` and make it work like a specification, which is used to
discuss what you want to do and how you will do it.  You may also reference a
sample project proposal: :doc:`project_simple_array`.

With your proposal, I can help you manage the development through discussions
(at which you should be pro-active).  A plan will not be be 100% accurate and
you should modify it as you go.  Use pull requests to keep the proposal
up-to-date.

You should write prototype code for your project with the proposal.  The
initial work will help you understand more about what to do.  It is difficult
to write a proposal without prototyping.

Some possible topics are listed in what follows.  They are of real use cases
for a project `modmesh <https://github.com/solvcon/modmesh>`__.  You may use a
topic derived from them, but also encouraged to come up with an original one.

.. _nsd-22au-project-conarr:

Contiguous Array
================

Multi-dimensional arrays of fundamental types and struct are a building block
for numerical code.  It may be as simple as a pointer to a contiguous memory
buffer, or well-designed meta-data with the memory buffer.  While a mere
pointer works well with one-dimensional arrays, calculating the pointer offset
for multi-dimensional arrays makes the code for numerical calculation cryptic
and hard to maintain.  It is very helpful to wrap the multi-dimensional index
calculation in a library.

A handy multi-dimensional array library should provide the following features:

1. No more runtime overhead than the calculation of the pointer offset.
2. Allow safe sharing of the memory buffer to other library and language in
   the same process.  This feature is the so-called zero-copy.  Sharing the
   buffer with other process using OS-provided shared memory should not be
   forbidden.
3. Support both fundamental types as well as composite types (struct).

.. _nsd-22au-project-columnar:

Columnar Array
==============

There are generally two ways to implement arrays of composite types.  One is to
pack the composite data and use an array for them, i.e., the so-called array of
struct (AoS):

.. code-block:: cpp

  struct Data
  {
      int m_field1;
      double m_field2;
  };

  SimpleArray<Data> data_array;

The other is to organize arrays of fundamental types, i.e., the so-called
struct of arrays (SoA) or the columnar arrays:

.. code-block:: cpp

  struct StructOfArray
  {
      SimpleArray<int32_t> m_field1;
      SimpleArray<double> m_field2;
  };

In the columnar array, if the fields are considered as the "rows" in a
two-dimensional array, the data organization is like the "column-major" format.
This is why we use the term "columnar" to describe this kinds of data
structure.  The columnar array (SoA) may provide better cache locality than
AoS, especially when there are many fields.  For example, if there are 8 fields
of double-precision floating point, each "row" will totally occupy a cache line
of 64 bytes.

.. note::

  The columnar array is usually two-dimensional and works like a table.

The requirements of the columnar array library:

1. A single class template can create the columnar array.
2. Automatic generate a row-accessor.  The row-accessor works as a handle (or
   cursor) over all rows in the array.

.. admonition:: References

  * `Arrow Columnar Format
    <https://arrow.apache.org/docs/format/Columnar.html>`__

.. _nsd-22au-project-graphpart:

Graph Partitioning
==================

Numerical solution of partial differential equations (PDEs) depends on
discretization of space.  The entities describing the discretized space is
called grid or mesh.  The mesh can be broadly categorized into structured and
unstructured mesh.  The latter is more flexible than the former.

The unstructured mesh allows free connectivity, which enables flexible
distribution of data for parallel computing.  The connectivity between mesh
elements can be represented as a graph, and the graph is used for partitioning.
The graph-partitioning problem is useful to minimizing the communication
between sub-mesh.

The graph partitioning code should support:

1. Extract a graph from a two- or three-dimensional unstructured mesh of mixed
   elements.
2. Find the sub-graphs whose edges across each other are minimized.
3. Use the sub-graphs to decompose the original mesh into inter-connected sub
   meshes.

.. admonition:: References

  * `METIS <http://glaros.dtc.umn.edu/gkhome/views/metis>`__
  * `SCOTCH <https://www.labri.fr/perso/pelegrin/scotch/>`__

.. _nsd-22au-project-rtree:

R-Tree Search Engine
====================

R-tree is an index to speed up searches in space.  It is usually referred to as
a spatial index or just a tree.  In one-dimensional space, a common search tree
may be used because it may use a single key for search. In multiple-dimensional
space, there are intrinsically multiple keys, so the search tree needs to
accommodate the dimensionality.  Data structures of the similar purpose include
k-d tree, quad-tree, etc.

The requirements of an implementation of the R-Tree search engine are:

1. It works in two- or three-dimensional space and may index point, line,
   surface, or volume.
2. Allow dynamic update of elements.
3. Allow access elements using a serial (integer) identifier.
4. Support ranged search of the geometrical entities.

.. admonition:: References

  * `R-tree implementation in boost.geometry
    <https://www.boost.org/doc/libs/1_77_0/libs/geometry/doc/html/index.html>`__

.. _nsd-22au-project-voronoi:

Voronoi Diagram
===============

The Voronoi diagram is a decomposition of a region that any point in a
sub-region is closest to the site of the sub-region.  A classical example is to
determine the service areas of each branch of a reseller chain.  Our interest
of this problem is to discretize space for mesh generation.  It can be used to
create triangular mesh in the Delaunay triangulation.

The requirements of the Voronoi diagram code are:

1. Given geometrical entities in two- or three-dimensional space, find the
   Voronoi diagram.
2. The data structure allows accessing the geometrical entities and the Voronoi
   diagram using a serial (integer) number.  The index access implies the
   entities and the Voronoi diagram are associated with each other.
3. Fast searching for nearby entities is supported with a spatial index.

.. _nsd-22au-project-curve:

Parametric Description of Curved Geometry
=========================================

To describe the smooth geometry of an object in space, Bezier curves are
usually used.  The spatial discretization may be applied on the objects for
numerical calculation.

The requirements of the Bezier code:

1. Computation mesh can be generated against the curved objects in two- or
   three-dimensional space.
2. The mesh can be associated with the curved geometry, preferably with serial
   (integer) identifiers.

.. _nsd-22au-project-polybool:

Boolean Operations on Polygons
==============================

In Euclidean space we are interested in finding the Boolean, i.e., AND, OR,
NOT, XOR, of polygons.  The polygonal Boolean operations are useful when we
want to extract geometrical properties of the graphics.  In two-dimensional
space we deal with polygons.  In three-dimensional space it is polyhedra.

.. admonition:: References

  * `The boost.polygon library
    <https://www.boost.org/doc/libs/1_76_0/libs/polygon/doc/index.htm>`__


.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2 tw=79:
