=======================
SOLVCON v0.1 and Beyond
=======================

.. contents::

Create date: 2011/8/11

SOLVCON v0.1 marks a milestone.  Before version 0.1, the development of SOLVCON
focused on the proof of concept of *using Python to construct a software
framework for a multi-physics, highly-parallelized system that solves for
conservation laws based on unstructured meshes*.  SOLVCON includes two distinct
physical models: One for compressible flows and the other for stress waves in
anisotropic elastic solids.  The code can deliver more than 0.3 million
elements/second on a Xeon or Opteron by using OpenMP, which is a reasonable
speed for time-accurate CFD solvers in production use.  The calculations can be
scaled up to use more than 1,000 CPU cores.  In addition to these results,
SOLVCON showed tremendous potentials by incorporating in situ visualization,
hybrid parallelism, and GPGPU computing (on clusters).

After summarizing the proof of concept by releasing version 0.1, I can now plan
for further development of SOLVCON.

The CESE Method
===============

The CESE method will still be employed as the default numerical method of
SOLVCON.  The current development of the CESE method implementation in SOLVCON
is directed to high-order schemes.  The high-order CESE method are being
implemented by David Bilyeu and me.  A prototype of the fourth-order Euler
solver has been worked out and is awaiting validation and consolidation.

For complex physical processes that involve more than 10 or 20 conservation
variables, the implementation of the CESE method should use compress sparse row
(CSR) or a similar format for the buffers of the Jacobian matrices, in order to
same memory and potentially to speed up.

Scalability and MPI
===================

Scalability of SOLVCON will be further improved by making the
distributed-memory parallel computing comply to the single-process
(thread-less), massively-parallelized, message-passing architecture.

The change will also facilitate simplification of the executional model.
Parallel calculations will be launched through standard MPI utilities only.
Currently, SOLVCON can be launched by MPI utilities or its own remote procedure
call (RPC) sub-system.  In the future the custom sub-system will be
discontinued.

The way for SOLVCON to invoke MPI will also change.  Currently, MPI libraries
are invoked through ``ctypes``.  This approach is cumbersome for deploying
SOLVCON onto various supercomputers/clusters.  Changing to use ``mpi4py`` will
solve the issue.  ``mpi4py`` will introduce an abstraction layer that deals
with the wrapping of MPI.

These changes will make the code base more compact and easier to be maintained.

Code for domain decomposition will be further optimized for speed and memory,
in order to efficiently use more than 100 million elements in routine
simulations along with in situ visualization.

Physical Models
===============

The immediate development task after the release of version 0.1 is to add
viscous terms in the compressible flow solver.  Based on the 2nd-order CESE
method, the treatment of the new *diffusive fluxes* will be added to the
implementation to complement the existing treatment of the *convective fluxes*.

Two more physical models will be incorporated into SOLVCON into the name space
``solvcon.kerpak``: (i) waves in viscoelastic media and (ii) liquid flows.

Other physical processes under planning include electromagnetic waves and
stress waves in piezoelectric solids.
