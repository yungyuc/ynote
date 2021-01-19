==============================
Numerical Software Development
==============================

`Numerical software development <https://yungyuc.github.io/nsd>`__ is a course
taught in NCTU, dept of CS, focusing on the art to build numerical software,
i.e., computer programs applying numerical methods for solving mathematical or
physical problems.

It introduces the hybrid architecture popular in the industry and shows how to
use Python and C++ for implementation.  I expect students to practice serious
software engineering and obtain fundamental skills for developing modern
numerical software systems.

The contents have two parts.  First, the recapitulation of how computers work:

1. Engineering practices
2. Python and numpy
3. C++ and computer architecture
4. Matrix operations
5. Cache optimization
6. Vector processing

Second, the practical implementation skills:

7. Memory management
8. Ownership and smart pointers
9. Modern C++
10. C++ and C for Python
11. Array code in C++
12. Advanced Python

What Is Numerical Software
==========================

Digital computer was originally invented to do mathematics.  The mission of the
first digital computer, Electronic Numerical Integrator and Computer (`ENIAC
<https://www.britannica.com/technology/ENIAC>`__, 1945), was to quickly obtain
artillery range tables.  Running at the electronic speed, the computer
performed a tremendous amount of calculation.

We use digital computers to crunch more and more numbers.  Computer code
follows the numerical methods, which are developed based on the mathematic
formulations.  Sometimes the mathematics has an associated physical problem.
But sometimes, it's just mathematics.  The applications are endless, but here
list some famous packages to give you an idea:

* Infrastructure: `NumPy <https://numpy.org>`__
* Data analytics: `Pandas <https://pandas.pydata.org>`__, `Arrow
  <https://arrow.apache.org>`__, `PyTables <https://www.pytables.org>`__
* Linear algebra: `BLAS <http://www.netlib.org/blas/>`__, `LAPACK
  <http://www.netlib.org/lapack/>`__
* Geometry: `boost.geometry
  <https://www.boost.org/doc/libs/1_72_0/libs/geometry/doc/html/index.html>`__
* Visualization: `VTK <https://vtk.org>`__, `Matplotlib
  <https://matplotlib.org>`__
* Machine learning: `PyTorch <https://pytorch.org>`__

Despite the versatility, numerical software shares common traits:

* Not visually pleasant, oftentimes no graphical user interface
* Knowledge-intensive, unintuitive to code
* Computation-intensive, often incorporating parallelism, distributed
  computing, and special hardware

Numerical software is developed to solve problems in science and engineering.
It always has an application domain attached, and cannot be handled solely in
computer science.  Of course, since it is computer software, it cannot exist
without computer science.  Naturally it is cross-discipline and demands
knowledge and skills in two or more fields from the practitioners.

Why Develop Numerical Software
==============================

Numerical software is developed to solve problems that are either impracticable
or unmarketable without it.

For the impracticable problems, numerical software simply enables the solution
so that we can study them.  Problems in the fields of fluid dynamics and
astrophysics are usually of this kind.  For the unmarketable problems, the
software will significantly reduce the cost to solution.  Machine learning,
visualization, communication, etc., are problems of this kind.

Like developing any software, the true driver must be identified so that the
system can be properly specified.  After that, there is a pattern in developing
numerical software:

1. Observation
2. Generalize to a theory in math
3. Obtain analytical solutions for simple setup
4. Get stuck with **complex** setup
5. Numerical analysis comes to rescue
6. ... **a lot of code development** ...
7. Release a software package

Hybrid Architecture
===================

Computing is about commanding the computers to perform calculations to yield
the results that we want to see.  We delegate work to computers as much as
possible, but keep the highest possible system performance.

Numerical software usually uses a hybrid architecture to achieve this.  The
system is composed of a fast, low-level computing engine and an easy-to-use,
high-level scripting layer.  It is usually developed as a platform, working
like a library that provides data structures and helpers for problem solving.
The users will use a scripting engine it provides to build applications.
Assembly is allowed in the low-level computing engine to utilize every drop of
hardware: multi-core, multi-threading, cache, vector processing, etc.

A general description of the architecture is like the following layers, from
high-level to low-level:

* External result

  * This is presented in a non-technical way to people outside the
    problem-solving team.  They can be stakeholders for business or general
    public.  The result has to be generated in some way, which may or may not
    be included in the numerical software we make.

* Problem presentation: physics, math, or equations

  * Users use the software or associated tools to present the technical result.

* Scripting or configuration

  * Users follow the example scripts to configure the problems to solve.
    Configuration files may also be used.

* Library interface

  * This defines the application programming interface (API) for the numerical
    software.  Scripts should not touch anything below this layer.

* Library structure

  * This is where we architect the software.  Good book-keeping code is here to
    separate the interface and the computing kernel.  Data structures are
    designed at this layer to make sure no time is wasted in copying or
    converting data.

* Computing kernel

  * This is the place the does the heavy-lifting, and where we do most of the
    optimization.

Pattern 1: Research Code
++++++++++++++++++++++++

For a research code, the boundary between external result, problem
presentation, and scripting, and that between library interface, library
structure, and computing kernel, may be less clear.  The architecture is
usually like:

* Problem presentation: high-level description, physics, and scripting / code
  configuration
* Library implementation

But sometimes if we don't pay attention to architecture, there may be no
boundary between anything.

Pattern 2: Full-Fledged Application
+++++++++++++++++++++++++++++++++++

For a commercial grade package, each of the layers will include more
sub-layers.  It is a challenge to prevent those layers or sub-layers from
interweaving.  From users' point of view, the sophistication appears in the
problem presentation and the scripting layers.  Developers, on the other hand,
take care of everything below problem presentation, so that users can focus on
problem solving.

Pattern 3: Scripting for Modularization
+++++++++++++++++++++++++++++++++++++++

At this point, it should be clear that the scripting layer is the key glue in
the system architecture.  The high-level users, who use the code for problem
solving, wouldn't want to spend time in the low-level implementation.  Instead,
they will specify the performance of the API exposed in the scripting layer.
The performance may be about the quality of result and runtime (including
memory).

The scripting layer can separate the programming work between the high-level
problem presentation and the low-level library implementation.  A scripting
language is usually dynamically typed, while for speed, the low-level
implementation language uses static typing system.  In the dynamic scripting
language, unit-testing is required for robustness.  In a statically typed
language like C++, the compiler and static analyzers are very good at detecting
errors before runtime.  But the great job done by the compiler makes it clumsy
to use C++ to quickly write highly flexible code for problem presentation.

It is tempting to invent one programming language to rule them all.  That
approach needs to convince both the high-level problem solvers and the
low-level implementers to give up the tools they are familiar with.  The new
language will also need to provide two distinct styles for both use cases.  It
will be quite challenging, and before anyone succeeds with the one-language
approach, we still need to live with a world of hybrid systems.

Numerical Software = C++ + Python
=================================

The key to a successful numerical software system is make it uncompromisingly
fast and extremely flexible.  It should be flexible enough so that users, i.e.,
scientists and engineers, can easily write lengthy programs to control
everything.  It should be noted that, although the users program in the system,
they by no means know about computer science.

Not all programming languages can meet the expectation.  To this point, the
most suitable scripting language is Python, and the most suitable low-level
language may be C++.  C++ can be controversial, but considering the support it
received from the industry, it's probably difficult to find another language of
higher acceptance.  Our purpose here is to introduce the skills for developing
numerical software, not to analyze programming languages.  We will focus on C++
and Python.

More Reasons to Use Python
++++++++++++++++++++++++++

* Python provides a better way to describe the physical or mathematical
  problem.

* Python can easily build an even higher-level application, using GUI,
  scripting, or both.

* Is there alternative for C++?  No.  For Python?  Yes.  But Python is the
  easiest choice for its versatility and simplicity.

* A numerical software developer sees through the abstraction stack:

  * The highest-level application is presented as a Python script.
  * The Python script drives the number-crunching C++ library.
  * C++ is the syntactic sugar for the machine code.

Contents
========

To be added.

.. toctree::
  :maxdepth: 1
