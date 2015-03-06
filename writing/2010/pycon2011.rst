===========================
PyCon 2011 SOLVCON Proposal
===========================

SOLVCON: A New Python-Based Software Framework for Massively Parallelized
Numerical Simulations

Description
===========

SOLVCON is the first Python-based software framework for high-fidelity
simulations of multi-physics conservation laws.  More than ninety percents of
the codes are done in Python.  Performance hot-spots are optimized by C and
glued by ctypes library.  SOLVCON is high-performance in nature and has been
able to utilized 512 4-core nodes at Ohio Supercomputer Center.

Abstract
========

In the coming decade, performance improvements of scientific computing will
mainly come from major changes in the computing hardware.  A well-organized
software structure is imperative to accommodate such changes.  Based on the
Python programming language, SOLVCON is designed as a software framework to
develop conservation-law solvers by segregating solving kernels from various
supportive functionalities.  Being the governing equations for the physical
world, conservation laws are applied everywhere in scientific and engineering
research.  Although everyone knows that the numerical algorithms and physical
models form the kernel of any conservation-laws solver, few if not none
simulation software can cleanly separates those core components from supportive
functionalities.  Because of the lack of organization, duplicated supportive
functionalities have to be reimplemented in many conventional simulation codes.
Code reuse was done in the unmaintainable copy-and-paste fashion.  The newly
developed SOLVCON framework provides a resolved structure to accommodate the
core components of conservation-law solver in segregated solving kernels.
Aided by the resolved structure, hybrid parallelism, which combines shared- and
distributed-memory parallelization, can also be implemented in an organized way
to achieve high-fidelity simulation of conservation laws.  Simulation codes by
using GPU clusters will also be accommodated by SOLVCON.  To date, SOLVCON has
utilized up to 512 4-core nodes at Ohio Supercomputer Center for high-fidelity
CFD simulations.

Supercomputing is undergoing the third revolution by the emerging GPU
computing.  GPU computing enables numerical analysts to reduce the time for
high-fidelity simulations using up to a billion of elements from months to
days.  In order to use GPU computing to accelerate such large-scale problems,
GPU nodes must be networked together to form a GPU cluster.  As such,
shared-memory and distributed-memory parallelization must be simultaneously
utilized to achieve the so-called hybrid parallelism.  Parallel computing is
difficult, and hybrid parallel computing is more difficult.  SOLVCON is
developed to resolve the unavoidably complicated programming efforts.  By using
Python to develop the fundamental software structure, GPU or multi-threaded
programming for shared-memory parallelization are locked in segregated solving
kernels.  Complex message-passing is implemented in SOLVCON and isolated from
solving-kernel developers.  The framework approach also enables pluggable
multi-physics through solving kernels.  Highly optimized C and GPU codes are
glued into SOLVCON without loss of performance by using the ctypes package.

SOLVCON has been applied to computation fluid dynamics and computational
mechanics.  More physical solvers are being developed for various propagating
wave problems, e.g., electromagnetic waves.  By using Python as the foundation
in SOLVCON, performance and extensibility are well balanced, and computational
research is being done in the most productive way.

Additional
==========

The new multi-physics software framework, SOLVCON, is being actively applied to
to perform the state-of-the-art scientific simulations by its author and his
coworkers.  The author has introduced the software in his local academic
community at the 6th Annual Dayton Engineering Science Symposium, 25 October
2010, Dayton, Ohio.  SOLVCON is planned to be open-sourced after the
presentation of an accepted paper for it at the 49th AIAA Aerospace Sciences
Meeting, 4-7, January, 2011.
