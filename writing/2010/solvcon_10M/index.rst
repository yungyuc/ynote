==============================
A Note for SOLVCON Going Large
==============================

.. contents::

On 2010/9/18, Sat., `SOLVCON
<http://cfd.eng.ohio-state.edu/~yungyuc/solvcon.html>`_/`SOLVCESE
<http://cfd.eng.ohio-state.edu/~yungyuc/solvcese.html>`_ finished its first run
of 10M+ (precisely speaking, **16,110,000**) cells on the glenn cluster at
`Ohio Supercomputer Center (OSC) <http://www.osc.edu/>`_.  The simulation
utilized 64 8-core Opteron nodes, i.e., 512 cores in total, to calculate a
`three-dimensional supersonic jet in cross flow problem
<http://cfd.eng.ohio-state.edu/old/research/injection/injection030103.htm>`_
with Euler solver.

Flow Field
==========

The 16M run is rather difficult to be visualized (the resulting binary legacy
VTK file is 3.3GB per output).  The following figures depict the streak lines
coming from the jet inlet.

  .. image:: sjcfstd_30mm.jet.png
     :width: 400px
     :alt: Overview of the streak lines from the jet inlet.

  .. image:: sjcfstd_30mm.jet.rear.png
     :width: 400px
     :alt: Rear view of the streak lines from the jet inlet.

Comparing the 16M jet streak lines with the 3M streak lines plotting as
following:

  .. image:: sjcfz.jet.overview.png
     :width: 400px
     :alt: Overview of the streak lines from the jet inlet.

  .. image:: sjcfz.jet.rear.png
     :width: 400px
     :alt: Rear view of the streak lines from the jet inlet.

It is clear that higher resolution gives better detail.

Status: Preliminary Benchmark Results
=====================================

Issues involving **large datasets** surfaced while running the "not-small"
calculation.  Some facts about the current state:

  * The Gambit Neutral file (in ASCII format) for the 16M mesh is
    several GB.  It took SOLVCON **more than an hour** to convert
    (read/parse/convert/calculate/compress/write) the neutral file to SOLVCON's
    blk file (1.1GB).  While converting, SOLVCON uses about 10GB of memory.

  * Domain decomposition for the 16M mesh took 15 minutes (including
    partitioning and splitting).

  * Transferring/distributing the initialized solver objects to 64 nodes took 4
    minutes.

  * The head process consumed 2GB memory per 1M cells (30GB memory for the 16M
    cells run).

  * Each of the output VTK files is 3.3GB.  The size prohibits it to be loaded
    on a laptop (my ThinkPad T400 has only 4GB of memory).

  * For the previous 3M cell run, SOLVCON has successfully utilized 512 4-core
    Opteron on glenn cluster at OSC.

Issues Raised in Solving Large-Scale Problems
=============================================

In general, SOLVCON uses **too much memory**.  The memory-hogging behavior is
resulted from the rapid implementation of SOLVCON, but not intrinsic in the
algorithms nor the programming language (Python).  However, the huge memory
usage hinders calculation of larger problems.  The current issues in SOLVCON
can be itemized as:

  * Gambit Neutral file format is limiting.

    Some fields of the neutral file is fixed with 8 characters, such as the
    node list in the cell connectivity definition.  The limitation is two-fold.
    First, white spaces cannot be used as delimiter to parse the neutral file
    of 10M+ cells/nodes (indices could use full 8 characters to represent).
    Second, the format becomes ambiguous for mesh of 100M+ cells/nodes (indices
    could use full 9 characters to represent).

    The first problem can be and has been easily solved by rewriting the
    neutral file parser in SOLVCON.  The new parser uses less memory and does
    not rely on white space delimiter.  The second problem is very difficult if
    not impossible to be tackled.  In addition, the neutral file is of ASCII
    format, which is very inefficient in storage and takes more time to parse.
    
    These issues make Gambit Neutral file a less ideal intermediate format
    high-fidelity PDE calculation.  To achieve 1B cell simulation, SOLVCON
    should use a **more robust file format**, and/or **another mesh generator**
    in development.

  * Pre-processing uses too much memory and time.

    Before calculation can be performed on a cluster, the mesh needs to be (i)
    **loaded** as SOLVCON Block object, and (ii) **decomposed** for distributed
    Solver creation.  The current implementation of mesh conversion and domain
    decomposition puts everything in memory while working.  In addition, the
    pre-processing has to be performed before every execution of calculation,
    since there has not been a robust approach of serialization in SOLVCON.

  * Output for visualization is too time-consuming.

    SOLVCON currently outputs binary legacy VTK files.  Each legacy VTK file
    has to have mesh data, although the time-varying output needs only one copy
    of mesh data.  Further, there is no convenient way to select single/double
    precision for VTK output.  An approach to mitigate this issue is to use the
    new **XML-based VTK format**, which allows parallel output and more
    flexible meta-data.

  * Execution control is not flexible and robust enough.

    Currently codes for SOLVCON mostly follow "in situ" processing model.  That
    is, all the desired data is calculated on the fly when the calculation is
    being executed.  There is not a flexible and robust mechanism of
    serialization available in SOLVCON.

  * Default simulation information reporting is not robust enough.

    This is rather a minor issue.  The default information reporting Hooks are
    not designed so well for long-running calculations.  Performance and
    meta-data are reported too infrequently.  Information is not written to log
    file either.

  * Multi-threading and message-passing is far from optimal.

    More benchmarks are needed.

Work Plan
=========

The objective of SOLVCON/SOLVCESE is to routinely calculate PDEs with a billion
of cells.  It is not trivial.  To achieve the specific objective, the current
goal is set to routinely calculate problems of 10M-50M cells with the following
capability:

  i.   Preparation (loading mesh, domain decomposition, Solver objects
       distribution, etc.) before each calculation consumes just **several
       minutes**.
  ii.  Visualization data output (VTK) should be parallelized and consume only
       the time for **several time steps**.
  iii. Execution can be flexibly controlled.
  iv.  Multi-threading and message-passing across network are optimized.

The following features should be added to SOLVCON/SOLVCESE:

  * Design an extensible strategy of high-performance serialization of     
    calculation data.
  * Based on the new serialization functionality, develop an approach to stage
    pre-processing operations to save time before each run of calculation.

    * A file/directory format for pre-partitioned Block object.
    * Remote initialization of Solver objects with pre-partitioned Block object.

  * Develop parallel Hook/Anchor for XML-based VTK files.

The following things in SOLVCON/SOLVCESE need to be improved:

  * Multi-threading: eliminate overhead.
  * Message-passing: characterize overhead.
  * Batch system management: more extensible.
  * Master process: memory usage to be less than 7GB.

An investigation of proper grid generating tools should be made.  Converters
for necessary data formats should be made into SOLVCON.
