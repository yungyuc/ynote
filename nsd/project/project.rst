=================
Practical Project
=================

.. toctree::
  :hidden:

  project_template
  project_simple_array

The course requires to develop a hybrid software system of C++11 (modern C++)
and Python for a numerical, scientific, or engineering problem.  Everyone needs
to write a proposal, develop the code, and present the project to the class at
the end of the course.

The software needs to be open-source and hosted on `github.com
<https://github.com/>`__.  It should be executable through command line.
Building the software should use a single command.

A proposal needs to be written before developing the project.  It serves as a
plan for the code development.  Please follow :doc:`project_template` and make
it work like a specification, which is used to discuss what you want to do and
how you will do it.  You may also reference a sample project proposal:
:doc:`project_simple_array`.

With your proposal, I can help you manage the development through discussions
(at which you should be pro-active).  A plan will not be 100% accurate and you
should modify it as you go.  Use pull requests to keep the proposal up-to-date.

You should write prototype code for your project with the proposal.  The
initial work will help you understand more about what to do.  It is difficult
to write a proposal without prototyping.

Assessment
==========

The project will be assessed based on the following guidelines: :ref:`good
engineering infrastructure <nsd-grading-soft>`, :ref:`correct implementation and
solution to the problem <nsd-grading-correct>`, :ref:`adequate architecture
<nsd-grading-arch>`, and :ref:`clear presentation <nsd-grading-present>`.

.. _nsd-grading-soft:

Software Engineering
++++++++++++++++++++

* Build system
* Testing
* Version control
* History quality [adv]_
* Issue tracking [adv]_
* Documentation [adv]_

.. _nsd-grading-correct:

Correctness
+++++++++++

* Existence of golden
* Quality of golden
* Sequence of development [adv]_

.. _nsd-grading-arch:

Software Architecture
+++++++++++++++++++++

* Modularity: SOLID, C++, and Python
* Use of wrapping
* API granularity and design for testing
* Performance, including runtime and memory consumption [adv]_
* Design, including iterative implementation [adv]_

.. _nsd-grading-present:

Presentation
++++++++++++

* Technical fluency
* Slide clarity
* Time control
* Appearance

.. _nsd-sample-topics:

Sample Topics
=============

Some possible topics are listed in what follows.  They are of real use cases
for the teaching code `modmesh <https://github.com/solvcon/modmesh>`__.  You may
use a topic derived from them, but also encouraged to come up with an original
one.

.. _nsd-project-conarr:

Contiguous Array
++++++++++++++++

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

.. _nsd-project-columnar:

Columnar Array
++++++++++++++

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

.. _nsd-project-graphpart:

Graph Partitioning
++++++++++++++++++

Numerical solution of partial differential equations (PDEs) depends on
discretization of space.  The entities describing the discretized space are
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

.. _nsd-project-rtree:

R-Tree Search Engine
++++++++++++++++++++

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

.. _nsd-project-voronoi:

Voronoi Diagram
+++++++++++++++

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

.. _nsd-project-meshconv:

Mesh Data Conversion
++++++++++++++++++++

Numerical solution of partial differential equations (PDEs) depends on
discretization of space.  The entities describing the discretized space are
called grid or mesh.  To successful carry out the numerical analysis, there are
3 types of codes: (i) a mesh generator creates the mesh, (ii) a solver computes
the solution, and (iii) a visualizer provides the analysis.  The mesh data play
a central role among the codes.

Different codes take different formats for the mesh data.  There needs to be
another code to convert the mesh and solution data for the corresponding codes.
The converting code should work as a standalone executable.  It will be good
if the code also works as a library to be plugged in other associated codes for
easier automation.

Examples of the numerical analytical codes:

1. `Plot3D <https://en.wikipedia.org/wiki/PLOT3D_file_format>`__ is a NASA code
   to visualize solution with structured mesh.  It's format is also used by many
   computing code as mesh input.
2. `SU2 <https://su2code.github.io>`__ is a solver for fluid dynamics.

.. _nsd-project-curve:

Parametric Description of Curved Geometry
+++++++++++++++++++++++++++++++++++++++++

To describe the smooth geometry of an object in space, Bezier curves are
usually used.  The spatial discretization may be applied on the objects for
numerical calculation.

The requirements of the Bezier code:

1. Computation mesh can be generated against the curved objects in two- or
   three-dimensional space.
2. The mesh can be associated with the curved geometry, preferably with serial
   (integer) identifiers.

.. _nsd-project-polybool:

Boolean Operations on Polygons
++++++++++++++++++++++++++++++

In Euclidean space we are interested in finding the Boolean, i.e., AND, OR,
NOT, XOR, of polygons.  The polygonal Boolean operations are useful when we
want to extract geometrical properties of the graphics.  In two-dimensional
space we deal with polygons.  In three-dimensional space it is polyhedra.

.. admonition:: References

  * `The boost.polygon library
    <https://www.boost.org/doc/libs/1_76_0/libs/polygon/doc/index.htm>`__

.. [adv] Advanced points.  It is recommended to address them after the basic
         points.

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2 tw=79:
