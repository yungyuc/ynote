==================
Cache Optimization
==================

Memory Hierarchy
================

In the simplest model of computers, we write programs that process data in a
linearly-addressed memory.  In reality, to achieve good runtime performance,
the memory is hierarchical.  The programs we write still treat it like a
linearly-addressed space, but by understanding the underneath hierarchical
structure, we may get the most performance from the system.

When it comes to performance, we may be tempted to make all memory as fast as
possible.  It is impractical because fast memory is very difficult to
manufacture.  The economocally feasible approach is to keep the most frequently
used data in the fastest memory, and the infrequently used data in slow and
inexpensive memory.  When the work with the data is finished, they are removed
from the fast memory, and other data are loaded to it.

Memory comes in many kinds in the hierarchy.  I put them in 4 categories:

1. CPU register file

  The registers reside in the CPU circuits.  Instructions (machine code or
  assembly) can directly operate them and the electronic signals flow through
  the CPU circuits.  There is no delay in access time to registers.  They are
  the fastest memory.

2. CPU cache

  CPU cache memory works as a 'buffer' between the registers and the main
  memory.  It usually uses fast and expensive static random access memory
  (SRAM).  It is called static because the circuit keeps in one of the two
  stable states and access doesn't change the state.  The circuit takes more
  transistors than slower types of memory.

  The cache memory may be part of the CPU circuit or outside it.  A CPU usually
  has multiple levels of cache.  It can be a couple of MBs, or as large as tens
  or hundreds of MBs:

  * Intel Xeon Platinum 9282 has 77MB L3 (56 cores):
    https://en.wikichip.org/wiki/intel/xeon_platinum/9282
  * AMD EPYC 7H12 has 256MB L3 (64 cores):
    https://www.amd.com/en/products/cpu/amd-epyc-7h12

3. Main memory

  Main memory is away from the CPU chip package.  It usually uses less
  expensive dynamic random access memory (DRAM).  The circuit takes more time
  to access the data but the lower cost allows much larger space.

  Data in the main memory are lost when the system is powered off.

  Mainstream PC uses DDR (double data rate) 4 SDRAM (synchronous dynamic
  random-access memory) with DIMM (dual in-line memory module) and its
  variants, e.g. RDIMM (registered DIMM) and LRDIMM (load reduced DIMM).
  Depending on the CPU memory controller bandwidth, the data throughput may be
  around 60GB/s or higher.

  A powerful server may have up to 6TB of main memory:

  * 24 RDIMM/LRDIMM slots: https://www.supermicro.com/en/products/ultra
  * 256GB RDIMM/LRDIMM module:
    https://www.samsung.com/semiconductor/dram/module/

4. Storage

   Data on the storage cannot be directly accessed by CPU instructions.  They
   need to be loaded to main memory and then the instructions can touch them.
   The loading and saving operations are considered input and output (I/O).
   Data in the storage are presisted when the system is powered off.

   The storage is usually called the "disks", because it used to be hard-disk
   drives (HDD).  In a recent system the storage changes to use the solid-state
   drives (SSD), which use the flash memory instead of hard disk.

   * Seagate Burracuda 510 SSD sequential read 3.45 GB/s, write 3.2 GB/s:
     https://www.seagate.com/internal-hard-drives/ssd/barracuda-ssd/
   * Samsung PM1735 SSD sequential read 8 GB/s, write 3.8 GB/s:
     https://www.samsung.com/semiconductor/ssd/enterprise-ssd/

Here is a table (excerpt from Figure 6.23 in CS:APP (3/e)) [1]_ showing the
latency of each of the memory, measured in CPU cycle (less than 1 ns):

.. table::
  :align: center

  ================ ==================
   Type             Latency (cycles)
  ---------------- ------------------
   CPU registers    0
   L1 cache         4
   L2 cache         10
   L3 cache         50
   Main memory      200
   Storage (disk)   100,000
  ================ ==================

Nothing is fast without cache.

How Cache Works
===============

There are 3 ways to implement caches: (i) direct-map caches, (ii)
set-associative caches, and (iii) fully-associative caches.  The direct-map
caches are the simplest one and I will use it to show the basic concepts of
caches.

If the accessed byte is already in the cache, it's a hit, and CPU directly gets
the byte from the cache.  If the byte is not in the cache, it's a miss, and the
memory controller goes to the main memory to fetch the byte into cache, before
CPU gets it.

According to the previous table, a cache miss is expensive.  When CPU can get
data from cache, the latency is around a couple of cycles.  When there is a
miss, additional hundreds of cycles are required to get the data.

There are two kinds of misses.  A cold miss happens when the requested byte is
not in the cache.  The second kind is conflict miss, and happens when multiple
cachable data claim the same cache block.  One set pops out the other, and vice
versa, and each access is a miss.

Assume we have a main memory of 64 bytes (6-bit address is sufficient), and a
cache of 8 bytes (use 3 bits for addressing).  The following example
demonstrates how a cache works:

.. table::
  :align: center

  ========== ============== ============= ================= =============
   Access #   Decimal addr   Binary addr   Hit or miss       Cache block
  ---------- -------------- ------------- ----------------- -------------
   1          22             010 110       miss (cold)       110
   2          26             011 010       miss (cold)       010
   3          22             010 110       hit               110
   4          26             011 010       hit               010
   5          16             010 000       miss (cold)       000
   6          18             010 010       miss (cold)       010
   7          26             011 010       miss (conflict)   010
   8          18             010 010       miss (conflict)   010
  ========== ============== ============= ================= =============

This is a direct-map cache.  To reduce conflict misses, we may use multi-way
set associativity.  2-, 4-, or 8-way set associative cache is commonly used.
Full associativity is too expensive in circuit implementation.

Cache Block (Line) Size Determines Speed
========================================

A cache block usually contains more than one byte or word.  In x86, the block
(line) size is 64 bytes.  When loading data from main memory to cache, it's
done block by block, rather than byte by byte.

I will be using an example of "skip access" to demonstrate that with cache,
doing more things doesn't take more time.

Before the example, I am showing the C++(11)-based timer used in the
experiments:

.. code-block:: cpp
  :linenos:

  #pragma once

  #include <chrono>

  class StopWatch
  {

  private:

      using clock_type = std::chrono::high_resolution_clock;
      using time_type = std::chrono::time_point<clock_type>;

  public:

      /// A singleton.
      static StopWatch & me()
      {
          static StopWatch instance;
          return instance;
      }

      StopWatch() : m_start(clock_type::now()), m_stop(m_start) {}

      StopWatch(StopWatch const & ) = default;
      StopWatch(StopWatch       &&) = default;
      StopWatch & operator=(StopWatch const & ) = default;
      StopWatch & operator=(StopWatch       &&) = default;
      ~StopWatch() = default;

      /**
       * Return seconds between laps.
       */
      double lap()
      {
          m_start = m_stop;
          m_stop = clock_type::now();
          return std::chrono::duration<double>(m_stop - m_start).count();
      }

      /**
       * Return seconds between end and start.
       */
      double duration() const { return std::chrono::duration<double>(m_stop - m_start).count(); }

      /**
       * Return resolution in second.
       */
      static constexpr double resolution()
      {
          return double(clock_type::period::num) / double(clock_type::period::den);
      }

  private:

      time_type m_start;
      time_type m_stop;

  }; /* end struct StopWatch */

Benchmark by Skip Access
++++++++++++++++++++++++

This is an example for skip access.  We allocate a memory buffer of :math:`128
\times 1024 \times 1024` bytes (128 MB), and operate the contents with
different "skips".  Skip 1 means accessing every element.  Skip 2 accesses half
of the elements, and so on.

.. code-block:: cpp
  :linenos:

  constexpr const size_t nelem = 128 * 1024 * 1024;
  int * arr = new int[nelem];

  // Sequential.
  for (int i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (int i=0; i<nelem; ++i) { arr[i] *= 3; }
  elapsed = sw.lap();

  // Skipping 2.
  for (int i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (int i=0; i<nelem; i+=2) { arr[i] *= 3; }
  elapsed = sw.lap();

  // ... 4, 8, 16, ... 1024.

Without knowing the effect of cache, we might intuitively think that the more
skip the shorter the runtime.  It's not wrong, but not completely correct.  In
the experiment, we see that the skipping has an effect of runtime only after
certain value.

.. admonition:: Execution Results

  :download:`code/01_skip_access.cpp`

  .. code-block:: console
    :caption: Build ``01_skip_access.cpp``

    $ g++ 01_skip_access.cpp -o 01_skip_access -std=c++17 -O3 -g -m64

  .. code-block:: console
    :caption: Run ``01_skip_access``
    :linenos:

    $ ./01_skip_access
    Sequential takes: 0.0909938 sec

    Skipping 2 takes: 0.0858447 sec
    Skipping 4 takes: 0.075287 sec
    Skipping 8 takes: 0.0734199 sec
    Skipping 16 takes: 0.0762235 sec

    Skipping 32 takes: 0.0581277 sec
    Skipping 64 takes: 0.0449813 sec
    Skipping 128 takes: 0.0307075 sec
    Skipping 256 takes: 0.0125121 sec
    Skipping 512 takes: 0.00623866 sec
    Skipping 1024 takes: 0.00230463 sec

Locality
========

While coding we usually don't have a lot of time to do detailed cache analysis.
Instead, we keep in mind that the code runs faster when it's more compact.
This is the concept of locality.

There are two kinds of locality: temporal and spatial.  Temporal locality means
that a fixed address is reused in the near future.  Spatial locality means that
the addresses close to the current address is reused in the near future.  The
better locality, of either kind, improves the performance.  And the cache
hierarchy is why locality works.

To take advantage of locality, programmers analyze by using "strides".  A
stride is the number of indexes to elements to slide when accessing the data in
arrays.  The most basic striding is sequential access, or the 1-stride.
Similarly, we may have n-strides.  The larger the stride is, the worse the
locality.

Recall that x86 uses 64-byte cache blocks, and a double-precision floating
point takes 8 bytes.

Matrix Population in C++
========================

To demonstrate the data layout, i.e., majoring or striding, affects runtime, we
use an example of populating :math:`1024 \times 1024 \times 64` integer elements as a
matrix.  The following shapes are benchmarked (total number of elements remains
unchanged):

* :math:`(1024\times1024\times64) \times 1`, i.e., one-dimension
* :math:`(1024\times1024\times32) \times 2`
* :math:`(1024\times1024\times16) \times 4`
* :math:`(1024\times1024\times8) \times 8`
* :math:`(1024\times1024\times4) \times 16`
* :math:`(1024\times1024\times2) \times 32`
* :math:`(1024\times1024) \times 64`
* :math:`(1024\times512) \times 128`
* :math:`(1024\times256) \times 256`
* :math:`(1024\times128) \times 512`
* :math:`(1024\times64) \times 1024`
* :math:`(1024\times32) \times (1024\times2)`
* :math:`(1024\times16) \times (1024\times4)`
* :math:`(1024\times8) \times (1024\times8)`

We populate the matrices along two axes.  First we iterate over the last index
(row):

.. code-block:: cpp
  :linenos:

  // Populate by last index.
  for (size_t i=0; i<nrow; ++i) // the i-th row
  {
      for (size_t j=0; j<ncol; ++j) // the j-th column
      {
          buffer[i*ncol + j] = i*ncol + j;
      }
  }

Then iterate over the first index (column):

.. code-block:: cpp
  :linenos:

  // Populate by first index.
  for (size_t j=0; j<ncol; ++j) // the j-th column
  {
      for (size_t i=0; i<nrow; ++i) // the i-th row
      {
          buffer[i*ncol + j] = i*ncol + j;
      }
  }

The speed varies.  To get the benchmark results correct, before the first
benchmarked population, we should access everywhere in the buffer to make sure
the memory is allocated:

.. code-block:: cpp

  // Prepopulation to cancel the effect of overcommit or delayed allocation.
  for (size_t i=0; i<nelem; ++i) { buffer[i] = nelem-i; }


While writing programs, it's much easier to know the stride than analyzing the
cache behavior.  The latter, in many scenarios, is prohibitively difficult.

Since we know the cache line is 64 byte wide, we expect the cache performance
may significantly reduce when the stride is around that value (16 int
elements).  As shown in the above benchmark.

.. admonition:: Execution Results

  :download:`code/02_locality.cpp`

  .. code-block:: console
    :caption: Build ``02_locality.cpp``

    $ g++ 02_locality.cpp -o 02_locality -std=c++17 -O3 -g -m64

  .. code-block:: console
    :caption: Run ``02_locality``
    :linenos:

    $ ./02_locality
    # of elements: 67108864 = 67108864 x 1
    populate double flatly takes: 0.075598 sec
    populate double along last axis takes: 0.138065 sec
    populate double along first axis takes: 0.0613524 sec
    ratio: 0.444374

    # of elements: 67108864 = 33554432 x 2
    populate double flatly takes: 0.0840775 sec
    populate double along last axis takes: 0.0971987 sec
    populate double along first axis takes: 0.134753 sec
    ratio: 1.38637

    # of elements: 67108864 = 16777216 x 4
    populate double flatly takes: 0.0890497 sec
    populate double along last axis takes: 0.0765863 sec
    populate double along first axis takes: 0.242477 sec
    ratio: 3.16606

    # of elements: 67108864 = 8388608 x 8
    populate double flatly takes: 0.0852944 sec
    populate double along last axis takes: 0.0892297 sec
    populate double along first axis takes: 0.481189 sec
    ratio: 5.3927

    # of elements: 67108864 = 4194304 x 16
    populate double flatly takes: 0.0859778 sec
    populate double along last axis takes: 0.0951305 sec
    populate double along first axis takes: 0.626653 sec
    ratio: 6.58731

    # of elements: 67108864 = 2097152 x 32
    populate double flatly takes: 0.0960501 sec
    populate double along last axis takes: 0.0732025 sec
    populate double along first axis takes: 0.787803 sec
    ratio: 10.762

    # of elements: 67108864 = 1048576 x 64
    populate double flatly takes: 0.081576 sec
    populate double along last axis takes: 0.095843 sec
    populate double along first axis takes: 0.89465 sec
    ratio: 9.33454

    # of elements: 67108864 = 524288 x 128
    populate double flatly takes: 0.0805293 sec
    populate double along last axis takes: 0.0841207 sec
    populate double along first axis takes: 0.883303 sec
    ratio: 10.5004

    # of elements: 67108864 = 262144 x 256
    populate double flatly takes: 0.0876071 sec
    populate double along last axis takes: 0.0827943 sec
    populate double along first axis takes: 0.899938 sec
    ratio: 10.8696

    # of elements: 67108864 = 131072 x 512
    populate double flatly takes: 0.0812722 sec
    populate double along last axis takes: 0.0807163 sec
    populate double along first axis takes: 0.816387 sec
    ratio: 10.1143

    # of elements: 67108864 = 65536 x 1024
    populate double flatly takes: 0.0882161 sec
    populate double along last axis takes: 0.0807104 sec
    populate double along first axis takes: 0.821201 sec
    ratio: 10.1747

    # of elements: 67108864 = 32768 x 2048
    populate double flatly takes: 0.0900379 sec
    populate double along last axis takes: 0.0750308 sec
    populate double along first axis takes: 0.586014 sec
    ratio: 7.81031

    # of elements: 67108864 = 16384 x 4096
    populate double flatly takes: 0.0865169 sec
    populate double along last axis takes: 0.0932342 sec
    populate double along first axis takes: 0.558974 sec
    ratio: 5.99537

    # of elements: 67108864 = 8192 x 8192
    populate double flatly takes: 0.0772652 sec
    populate double along last axis takes: 0.0819846 sec
    populate double along first axis takes: 0.589144 sec
    ratio: 7.18603

Array Majoring in Numpy
=======================

Array majoring is directly related to locality.  The difference in the
performance of matrix-vector multiplication is show for row- and
column-majoring arrays.

.. code-block:: python

  %%time
  dim = 10000
  float_rmajor = np.arange(dim*dim, dtype='float64').reshape((dim,dim))
  float_cmajor = float_rmajor.T.copy().T
  vec = np.arange(dim, dtype='float64')

.. code-block:: console

  CPU times: user 1.17 s, sys: 388 ms, total: 1.56 s
  Wall time: 1.56 s

.. code-block:: python

  %%time
  res_float_rmajor = np.dot(float_rmajor, vec)

.. code-block:: console

  CPU times: user 64.2 ms, sys: 1.26 ms, total: 65.5 ms
  Wall time: 64.1 ms

.. code-block:: python

  %%time
  res_float_cmajor = np.dot(float_cmajor, vec)

.. code-block:: console

  CPU times: user 138 ms, sys: 1.47 ms, total: 139 ms
  Wall time: 138 ms

Integer Matrix-Vector Multiplication
++++++++++++++++++++++++++++++++++++

.. code-block:: python

  %%time
  dim = 10000
  int_rmajor = np.arange(dim*dim, dtype='int64').reshape((dim,dim))
  int_cmajor = int_rmajor.T.copy().T
  vec = np.arange(dim, dtype='int64')

.. code-block:: console

  CPU times: user 1.13 s, sys: 390 ms, total: 1.52 s
  Wall time: 1.52 s

.. code-block:: python

  %%time
  res_int_rmajor = np.dot(int_rmajor, vec)

.. code-block:: console

  CPU times: user 81.6 ms, sys: 1.09 ms, total: 82.7 ms
  Wall time: 81.4 ms

.. code-block:: python

  %%time
  res_int_cmajor = np.dot(int_cmajor, vec)

.. code-block:: console

  CPU times: user 815 ms, sys: 2.01 ms, total: 817 ms
  Wall time: 816 ms

The performance difference of integer arrays is much larger than floating-point
arrays.  Note that ``double`` and ``int64`` both take 8 bytes.  Reason: LAPACK
/ MKL / vecLib.

For the same reason, the floating-point multiplication is slightly faster than
the integer.

Tiling
======

This is a naive implementation of matrix-matrix multiplication:

.. code-block:: cpp
  :linenos:

  for (size_t i=0; i<mat1.nrow(); ++i)
  {
      for (size_t k=0; k<mat2.ncol(); ++k)
      {
          double v = 0;
          for (size_t j=0; j<mat1.ncol(); ++j)
          {
              v += mat1(i,j) * mat2(j,k);
          }
          ret(i,k) = v;
      }
  }

The matrices are row-major.  The stride for the second matrix is ``ncol2``, so
it doesn't have good locality.  This naive implementation is clear, but the
performance is bad.

Matrix-matrix multiplication is one of the most important problems for
numerical calculation, and there are many techniques available for making it
fast.  Most if not all of them are about hiding the memory access latency.
Tiling is a basic technique that delivers impressive speed-up by reordering the
calculation and making it cache-friendly.

.. admonition:: Execution Results

  :download:`code/03_matrix_matrix.cpp`

  .. code-block:: console
    :caption: Build ``03_matrix_matrix.cpp``

    $ g++ 03_matrix_matrix.cpp -o 03_matrix_matrix -std=c++17 -O3 -g -m64  -I/opt/intel/mkl/include /opt/intel/mkl/lib/libmkl_intel_lp64.a /opt/intel/mkl/lib/libmkl_sequential.a /opt/intel/mkl/lib/libmkl_core.a -lpthread -lm -ldl

  .. code-block:: console
    :caption: Run ``03_matrix_matrix``
    :linenos:

    $ ./03_matrix_matrix
    Timing mkl: 0.0489621 second, 1.07374 Gflo, 21.9301 Gflops
    Timing indirect: 3.1151 second, 1.07374 Gflo, 0.344689 Gflops
    Timing direct: 3.04632 second, 1.07374 Gflo, 0.352472 Gflops
    Timing tiled 32: 1.15064 second, 1.07374 Gflo, 0.933171 Gflops
    Timing tiled 64: 0.452658 second, 1.07374 Gflo, 2.37208 Gflops
    Timing tiled 128: 0.748723 second, 1.07374 Gflo, 1.4341 Gflops
    Timing tiled 256: 0.833207 second, 1.07374 Gflo, 1.28869 Gflops
    Timing tiled 512: 0.764348 second, 1.07374 Gflo, 1.40478 Gflops
    Timing tiled 1024: 0.934039 second, 1.07374 Gflo, 1.14957 Gflops

Exercises
=========

1. Consult the data sheet of one x86 CPU and one Arm CPU.  Make a table for the
   line size of each of the cache levels, and compare the difference between
   the two CPUs.
2. Write a program that uses tiling to speed up matrix-matrix multiplication,
   and do not require the matrix dimension to be multiples of the tile size.

References
==========

.. [1] Computer Systems: A Programmer's Perspective, Chapter 6 The Memory
  Hierarchy, Randal E. Bryant and David R. O'Hallaron:
  https://csapp.cs.cmu.edu/

.. [2] Gallery of Processor Cache Effects:
  http://igoro.com/archive/gallery-of-processor-cache-effects/

.. [3] Lecture Notes of Applications of Parallel Computers by David Bindel:
  https://www.cs.cornell.edu/~bindel/class/cs5220-s10/slides/lec03.pdf

.. [4] https://en.wikichip.org/wiki/WikiChip

.. [5] https://www.uops.info/

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
