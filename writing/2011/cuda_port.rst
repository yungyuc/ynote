===============
Porting to CUDA
===============

.. contents::

Note on 2011/02/12.

A porting of the multi-dimensional, second-order CESE solver to CUDA  has been
finished in **less than 2 weeks, from Feb. 1, 2011 to Feb. 11, 2011**.  The
man-hours are estimated to be 80.  It is the first systematic effort for
porting the C-based CESE implementation of SOLVCON to CUDA.

The porting started from `changeset 8cda91c301bf (445)
<https://bitbucket.org/yungyuc/solvcon/changeset/8cda91c301bf>`_ and ended at
`changeset eaf35e678ead (532)
<https://bitbucket.org/yungyuc/solvcon/changeset/eaf35e678ead>`_.  The porting
used **88 changesets**.  The total change to the code base amounts to **8867
lines**, which is measured with::

  $ hg diff -g -r 8cda91c301bf:eaf35e678ead | wc -l

The porting ended with **2 products**.  The first is a CUDA-enabled CESE base
solver, `solvcon.kerpak.cuse
<https://bitbucket.org/yungyuc/solvcon/src/eaf35e678ead/solvcon/kerpak/cuse.py>`_,
and the second is a gas-dynamics solver of the Euler equations,
`solvcon.kerpak.gasdyn
<https://bitbucket.org/yungyuc/solvcon/src/eaf35e678ead/solvcon/kerpak/gasdyn.py>`_.
``gasdyn`` is developed based on ``cuse``.

There are **2 important by-products**.  The first is the one-file module
`solvcon.scuda
<https://bitbucket.org/yungyuc/solvcon/src/eaf35e678ead/solvcon/scuda.py>`_ for
wrapping CUDA with ctypes.  The second is the revised `CUDA SCons tool
<https://bitbucket.org/yungyuc/solvcon/src/eaf35e678ead/site_scons/site_tools/cuda.py>`_.
Both can be just copied out and used in a standalone fashion.

The porting hasn't involved any memory optimization.  The porting was based on
compute capability 2.0 (``sm_20``) for convenience.  The 3 Fermi (Tesla M2050)
nodes installed in `CFD lab <http://cfd.eng.ohio-state.edu/>`_) support CC 2.0.
It should be easy to further port to lower CC.

Porting Process
===============

The process to develop ``cuse`` involves 4 steps: (i) making ctypes-based CUDA
wrapper, (ii) porting old ``euler`` to ``cueuler`` with CUDA, (iii)
reorganizing ``cueuler`` to a new hierarchy of ``cuse`` and ``gasdyn``, and
(iv) incorporating ``cuse`` and ``gasdyn`` to ``solvcon.kerpak`` name space.

Step 1 includes 10 changesets from `changeset 8cda91c301bf (445)
<https://bitbucket.org/yungyuc/solvcon/changeset/8cda91c301bf>`_ to `changeset
0a02bb811eba (464)
<https://bitbucket.org/yungyuc/solvcon/changeset/0a02bb811eba>`_.  Experimental
code is written in ``sandbox/cuda``, and then move to ``solvcon.scuda`` module.
The wrapper was facilitated by fingolfin, a piloting CUDA implementation of the
two-dimensional CESE method based on old-version SOLVCON developed by `David
Bilyeu <http://cfd.eng.ohio-state.edu/people_davidb.html>`_ in the summer of
2009.  Although fingolfin provided CUDA-related know-how, no code was taken
from the old implementation to the new porting effort.

Step 2 includes 39 changesets from changeset `d20a4a9eac33 (456)
<https://bitbucket.org/yungyuc/solvcon/changeset/d20a4a9eac33>`_ to `changeset
efb10610385d (494)
<https://bitbucket.org/yungyuc/solvcon/changeset/efb10610385d>`_.  The
experimental CUDA Euler solver ``cueuler`` is put inside ``sandbox/cuda``.
Step 2 took most of the time.

First, a working module was copyied from ``solvcon.kerpak.euler`` as the
starting point.  Necessary C files were ported to CUDA by changing function
signatures, facilitated by using C macros.

Second, operation "phases" were migrated one by one from CPU to GPU.  It
started with ``calc_solt()``, then ``calc_soln()`` and ``calc_dsoln``.  To
facilitate the one-by-one migration, for each migrated phase, the data are (i)
allocated both on CPU and GPU but defined on CPU, (ii) uploaded from CPU to
GPU, (iii) processed on GPU, and (iv) finally downloaded from GPU to CPU.  The
overhead of data transfer would be minimized in Step 3.

Third, phases of boundary-condition treatments were migrated from CPU to GPU,
by following the same upload-process-download model.  After all operations were
migrated to GPU, unnecessary data transfer were removed.

Step 3 includes 36 changesets from `changeset 8a2e0106d4e2 (495)
<https://bitbucket.org/yungyuc/solvcon/changeset/8a2e0106d4e2>`_ to `changeset
e4aa39bbb317 (529)
<https://bitbucket.org/yungyuc/solvcon/changeset/e4aa39bbb317>`_.  The
experimental ``cueuler`` module was decomposed into ``solvcon.kerpak.cuse`` and
``gasdyn``.  It should be noted that ``gasdyn`` in this step was still in
directory ``sandbox/cuda``, because I didn't want to mess up ``solvcon.kerpak``
name space.  The process of reorganization was similar to step 2.  The phases
were ported again by using the upload-process-download model.  Unwanted data
transfer were removed after the code was proven to be correct.

Step 4 includes 3 changesets from `changeset 4cd3ca7c6545 (530)
<https://bitbucket.org/yungyuc/solvcon/changeset/4cd3ca7c6545>`_ to `changeset
eaf35e678ead (532)
<https://bitbucket.org/yungyuc/solvcon/changeset/eaf35e678ead>`_.  ``gasdyn``
module was incorporated into ``solvcon.kerpak`` name space, and a corresponding
example was created in ``examples/gasdyn/blnt``.

Planned Work
============

Tasks left to be done include: (i) eliminating data transfer per half-time-step
of ``cfl``, ``ocfl``, ``soln``, and ``dsoln``, (ii) porting to lower CC, e.g.,
``sm_13``, (iii) porting to OSC glenn with cluster support by selective upload
and download, (iv) porting the linear equations solver ``lincese`` to
``lincuse``, and (v) porting the velocity-stress equations solver ``elaslin``
to ``vslin``.  A meaningful benchmark can only be performed after task 1, 2,
and 3 are finished.

Asynchronous data transfer could be a candidate task, but with data transfer
eliminated, it is not really needed.  As a record, making use of pinned
(page-locked) memory can start with the following code::

  import ctypes as ct
  import numpy as np
  libc = ct.cdll.LoadLibrary('libc.so.6')
  malloc = libc.malloc
  malloc.restype = np.ctypeslib.ndpointer(dtype='int8', shape=(8,))
  ret = malloc(8)
  print ret, ret.ctypes._as_parameter_

Another note: use the following to turn on CUDA on Tesla without start X::

  $ modprobe nvidia
  $ mknod -m 666 /dev/nvidia0 c 195 0
  $ mknod -m 666 /dev/nvidiactl c 195 255
