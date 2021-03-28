==================
Cache Optimization
==================

.. contents:: Contents in the chapter
  :local:
  :depth: 1

Memory Hierarchy
================

.. contents:: Contents in the section
  :local:
  :depth: 1

In the simplest model of computers, we write programs that process data in a
linearly-addressed memory.  In reality, to achieve good runtime performance,
the memory is hierarchical.  The programs we write still treat it like a
linearly-addressed space, but by understanding the underneath hierarchical
structure, we may get the most performance from the system.

When it comes to performance, we may be tempted to make all memory as fast as
possible.  It is impractical because fast memory is very difficult to
manufacture.  The economically feasible approach is to keep the most frequently
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
  or hundreds of MBs.  See the following example specification of some devices:

  * `Intel Xeon Platinum 9282
    <https://en.wikichip.org/wiki/intel/xeon_platinum/9282>`__

    * 56 cores
    * 77MB L3 cache memory
  * `AMD EPYC 7H12
    <https://www.amd.com/en/products/cpu/amd-epyc-7h12>`__

    * 64 cores
    * 256MB L3 cache memory

3. Main memory

  Main memory is away from the CPU chip package.  It usually uses less
  expensive dynamic random access memory (DRAM).  The circuit takes more time
  to access the data but the lower cost allows much larger space.

  Data in the main memory are lost when the system is powered off.

  Mainstream PC uses DDR (double data rate) 4 SDRAM (synchronous dynamic
  random-access memory) with DIMM (dual in-line memory module) and its
  variants, e.g. RDIMM (registered DIMM) and LRDIMM (load reduced DIMM).
  Depending on the bandwidth of the CPU memory controller, the data throughput
  may be around 60GB/s or higher.

  A powerful server may have up to 6TB of main memory:

  * `24 RDIMM/LRDIMM slots
    <https://www.supermicro.com/en/products/ultra>`__

  * `256GB RDIMM/LRDIMM module
    <https://www.samsung.com/semiconductor/dram/module/>`__

4. Storage

  Data in the storage cannot be directly accessed by CPU instructions.  They
  need to be loaded to main memory and then the instructions can touch them.
  The loading and saving operations are considered input and output (I/O).
  Data in the storage are persisted when the system is powered off.

  The storage is usually called the "disks", because it used to be hard-disk
  drives (HDD).  In a recent system the storage changes to use the solid-state
  drives (SSD), which use the flash memory instead of hard disk.

  Example specification of some devices:

  * `Seagate Burracuda 510 SSD
    <https://www.seagate.com/internal-hard-drives/ssd/barracuda-ssd/>`__

    * sequential read 3.45 GB/s
    * write 3.2 GB/s
  * `Samsung PM1735 SSD
    <https://www.samsung.com/semiconductor/ssd/enterprise-ssd/>`__

    * sequential read 8 GB/s
    * write 3.8 GB/s

Here is a table (excerpt from Figure 6.23 in CS:APP (3/e)) [1]_ showing the
latency of each of the memory, measured in CPU cycle (less than 1 ns):

.. table:: Latency at each level of hierarchical memory
  :name: nsd-memory-latency
  :align: center
  :width: 80%

  ================ ==================
   Type             Latency (cycles)
  ================ ==================
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

.. contents:: Contents in the section
  :local:
  :depth: 1

There are 3 ways to implement caches:

1. Direct-map
2. Set-associative
3. Fully-associative

The direct-map caches are the simplest one and I will use it to show the basic
concepts of caches.

If the accessed byte is already in the cache, it's a hit, and CPU directly gets
the byte from the cache.  If the byte is not in the cache, it's a miss, and the
memory controller goes to the main memory to fetch the byte into cache, before
CPU gets it.

According to the :ref:`table above <nsd-memory-latency>`, a cache miss is
expensive.  When CPU can get data from cache, the latency is around a couple of
cycles.  When there is a miss, additional hundreds of cycles are required to
get the data.

There are two kinds of misses.  A cold miss happens when the requested byte is
not in the cache.  The second kind is conflict miss, and happens when multiple
cacheable data claim the same cache block.  One set pops out the other, and vice
versa, and each access is a miss.

Assume we have a main memory of 64 bytes (6-bit address is sufficient), and a
cache of 8 bytes (use 3 bits for addressing).  The following example
demonstrates how a cache works:

.. table::
  :align: center

  ========== ============== ============= ================= =============
   Access #   Decimal addr   Binary addr   Hit or miss       Cache block
  ========== ============== ============= ================= =============
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

.. contents:: Contents in the section
  :local:
  :depth: 1

A cache block usually contains more than one byte or word.  In x86, the block
(line) size is 64 bytes.  When loading data from main memory to cache, it's
done block by block, rather than byte by byte.

I will be using an example of "skip access" to demonstrate that with cache,
doing more things doesn't take more time (it uses :ref:`a simple timing helper
class <nsd-cache-StopWatch>`).

This is an example for skip access.  We allocate a memory buffer of :math:`128
\times 1024 \times 1024 \times 4` bytes (1 GB):

.. code-block:: cpp

  constexpr const size_t nelem = 128 * 1024 * 1024;
  int * arr = new int[nelem];

Then we measure the time to perform the same operations on the data with
different "skips".  The full example code can be found in
:ref:`01_skip_access.cpp <nsd-cache-example-01-skip-access>`.

No Skip (Access All Elements)
+++++++++++++++++++++++++++++

First is the time spent in the sequential access of all elements, i.e., skip of 1:

.. code-block:: cpp

  // Sequential; accessing all data every 4 bytes.
  for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (size_t i=0; i<nelem; ++i) { arr[i] *= 3; }
  elapsed = sw.lap();
  std::cout << "Sequential takes: " << elapsed << " sec" << std::endl;

.. code-block:: none

  Sequential takes: 0.0909938 sec

Skip 2 -- 16 Elements
+++++++++++++++++++++

Without knowing the effect of cache, we might intuitively think that skipping
more elements results in shorter runtime.

It's not wrong, but not exactly the case when the skipped data are still in a
cache line.  If we only skip 2, it's only slightly faster than accessing all:

.. code-block:: cpp

  // Skipping 2; accessing 4 bytes every 8 bytes.
  for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (size_t i=0; i<nelem; i+=2) { arr[i] *= 3; }
  elapsed = sw.lap();
  std::cout << "Skipping 2 takes: " << elapsed << " sec" << std::endl;

.. code-block:: none

  Skipping 2 takes: 0.0858447 sec

And skipping 4 -- 16 elements takes roughly the same time in the experiment:

.. code-block:: cpp

  // Skipping 4; accessing 4 bytes every 16 bytes.
  for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (size_t i=0; i<nelem; i+=4) { arr[i] *= 3; }
  elapsed = sw.lap();
  std::cout << "Skipping 4 takes: " << elapsed << " sec" << std::endl;

  // Skipping 8; accessing 4 bytes every 32 bytes.
  for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (size_t i=0; i<nelem; i+=8) { arr[i] *= 3; }
  elapsed = sw.lap();
  std::cout << "Skipping 8 takes: " << elapsed << " sec" << std::endl;

  // Skipping 16; accessing 4 bytes every 64 bytes.
  for (size_t i=0; i<nelem; ++i) { arr[i] = i; }
  sw.lap();
  for (size_t i=0; i<nelem; i+=16) { arr[i] *= 3; }
  elapsed = sw.lap();
  std::cout << "Skipping 16 takes: " << elapsed << " sec" << std::endl;

.. code-block:: none

  Skipping 4 takes: 0.075287 sec
  Skipping 8 takes: 0.0734199 sec
  Skipping 16 takes: 0.0762235 sec

Skip 32 -- 1024 Elements
++++++++++++++++++++++++

The runtime will significantly slow down after the skip number is larger than
16.  See the :ref:`following table <nsd-cache-skip>`.

.. list-table:: Runtime comparison for different number of access
  :name: nsd-cache-skip
  :header-rows: 1
  :align: center

  * - Element skip
    - Byte skip
    - access / all elements
    - Runtime (seconds)
  * - Sequential
    - 4
    - 128M
    - 0.0909938
  * - Skip elements
    -
    -
    -
  * - 2
    - 8
    - 64M
    - 0.0858447
  * - 4
    - 16
    - 32M
    - 0.075287
  * - 8
    - 32
    - 16M
    - 0.0734199
  * - 16
    - 64
    - 8M
    - 0.0762235
  * - Skip larger than cache line
    -
    -
    -
  * - 32
    - 128
    - 4M
    - 0.0581277
  * - 64
    - 256
    - 2M
    - 0.0449813
  * - 128
    - 512
    - 1M
    - 0.0307075
  * - 256
    - 1024
    - 512k
    - 0.0125121
  * - 512
    - 2048
    - 256k
    - 0.00623866
  * - 1024
    - 4092
    - 128k
    - 0.00230463

Locality
========

.. contents:: Contents in the section
  :local:
  :depth: 1

While coding we usually don't have a lot of time to do detailed cache analysis.
Instead, we keep in mind that the code runs faster when it's more compact by
using the concept of locality.  There are two kinds of locality:

Temporal
  Temporal locality means that a fixed address is reused in the near future.

Spatial
  Spatial locality means that the addresses close to the current address is
  reused in the near future.

The better locality, of either kind, improves the performance.  And the cache
hierarchy is why locality works.

To take advantage of locality, programmers analyze by using "strides".  A
stride is the number of indexes to elements to slide when accessing the data in
arrays.  The most basic striding is sequential access, or the 1-stride.
Similarly, we may have n-strides.  The larger the stride is, the worse the
locality.

Recall that x86 uses 64-byte cache blocks, and a double-precision floating
point takes 8 bytes.

Data Layout
===========

.. contents:: Contents in the section
  :local:
  :depth: 1

To demonstrate how the data layout (majoring) affects runtime, we use an
example of populating a matrix of :math:`1024 \times 1024 \times 64 = 67108864`
integer elements.  The matrix is populated along the two axes.  First we
iterate over the last index (row-majoring):

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

Then iterate over the first index (column-majoring):

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

To get the benchmark results correct, before the first benchmarked population,
we should access everywhere in the buffer to make sure the memory is allocated:

.. code-block:: cpp

  // Pre-populate to cancel the effect of overcommit or delayed allocation.
  for (size_t i=0; i<nelem; ++i) { buffer[i] = nelem-i; }

The full example code can be found in :ref:`02_locality.cpp
<nsd-cache-example-02-locality>`.  The benchmark results are:

.. list-table:: Runtime comparison for majoring
  :name: nsd-cache-majoring
  :header-rows: 1
  :align: center

  * - # rows
    - # columns
    - sec as flat
    - sec by last
    - sec by first
    - speed ratio
  * - 1024*1024*64
    - 1
    - 0.075598
    - 0.138065
    - 0.0613524
    - 0.444374
  * - 1024*1024*32
    - 2
    - 0.0840775
    - 0.0971987
    - 0.134753
    - 1.38637
  * - 1024*1024*16
    - 4
    - 0.0890497
    - 0.0765863
    - 0.242477
    - 3.16606
  * - 1024*1024*8
    - 8
    - 0.0852944
    - 0.0892297
    - 0.481189
    - 5.3927
  * - 1024*1024*4
    - **16**
    - 0.0859778
    - 0.0951305
    - 0.626653
    - 6.58731
  * - 1024*1024*2
    - 32
    - 0.0960501
    - 0.0732025
    - 0.787803
    - 10.762
  * - 1024*1024
    - 64
    - 0.081576
    - 0.095843
    - 0.89465
    - 9.33454
  * - 1024*512
    - 128
    - 0.0805293
    - 0.0841207
    - 0.883303
    - 10.5004
  * - 1024*256
    - 256
    - 0.0876071
    - 0.0827943
    - 0.899938
    - 10.8696
  * - 1024*128
    - 512
    - 0.0812722
    - 0.0807163
    - 0.816387
    - 10.1143
  * - 1024*64
    - 1024
    - 0.0882161
    - 0.0807104
    - 0.821201
    - 10.1747
  * - 1024*32
    - 1024*2
    - 0.0900379
    - 0.0750308
    - 0.586014
    - 7.81031
  * - 1024*16
    - 1024*4
    - 0.0865169
    - 0.0932342
    - 0.558974
    - 5.99537
  * - 1024*8
    - 1024*8
    - 0.0772652
    - 0.0819846
    - 0.589144
    - 7.18603

While writing programs, it's much easier to know the stride than analyzing the
cache behavior.  The latter, in many scenarios, is prohibitively difficult.

Since we know the cache line is 64 byte wide, we expect the cache performance
may significantly reduce when the stride is around that value (16 int
elements).  As shown in the above benchmark.

Array Majoring in Numpy
+++++++++++++++++++++++

We can also use numpy to show how the data layout impacts the runtime by using
matrix-vector multiplication as an example.  Use a :math:`10000\times10000`
matrix:

.. code-block:: python

  dim = 10000
  float_rmajor = np.arange(dim*dim, dtype='float64').reshape((dim,dim))
  float_cmajor = float_rmajor.T.copy().T
  vec = np.arange(dim, dtype='float64')

As a reference, the time spent in the bootstrapping is:

.. code-block:: none

  CPU times: user 1.17 s, sys: 388 ms, total: 1.56 s
  Wall time: 1.56 s

Use :py:func:`numpy:numpy.dot` for the matrix-vector multiplication with the
row-majored matrix:

.. code-block:: python

  res_float_rmajor = np.dot(float_rmajor, vec)

The time spent is:

.. code-block:: none

  CPU times: user 64.2 ms, sys: 1.26 ms, total: 65.5 ms
  Wall time: 64.1 ms

Then do the multiplication with the column-majored matrix:

.. code-block:: python

  res_float_cmajor = np.dot(float_cmajor, vec)

The time is:

.. code-block:: none

  CPU times: user 138 ms, sys: 1.47 ms, total: 139 ms
  Wall time: 138 ms

The column-majoring is twice as slow as the row-majoring.

Integer Matrix-Vector Multiplication
++++++++++++++++++++++++++++++++++++

Let's also see the same matrix-vector multiplication for integers.  The
:math:`10000\times10000` is set up in the same way as the floating-point:

.. code-block:: python

  dim = 10000
  int_rmajor = np.arange(dim*dim, dtype='int64').reshape((dim,dim))
  int_cmajor = int_rmajor.T.copy().T
  vec = np.arange(dim, dtype='int64')

As a reference, the time spent in the bootstrapping is the same as that for
floating-point:

.. code-block:: none

  CPU times: user 1.13 s, sys: 390 ms, total: 1.52 s
  Wall time: 1.52 s

Also use :py:func:`numpy:numpy.dot` for the matrix-vector multiplication with the
row-majored matrix:

.. code-block:: python

  res_int_rmajor = np.dot(int_rmajor, vec)

It is not too slow:

.. code-block:: none

  CPU times: user 81.6 ms, sys: 1.09 ms, total: 82.7 ms
  Wall time: 81.4 ms

The perform the multiplication with the column-majored matrix:

.. code-block:: python

  res_int_cmajor = np.dot(int_cmajor, vec)

The performance difference of integer arrays is much larger than floating-point
arrays:

.. code-block:: none

  CPU times: user 815 ms, sys: 2.01 ms, total: 817 ms
  Wall time: 816 ms

It is 10 times slower.  Note that ``double`` and ``int64`` both take 8 bytes.
The reason is that there is not optimized helpers in LAPACK / MKL / vecLib for
the column-majored multiplication.

For the same reason, the floating-point multiplication is slightly faster than
the integer.

Tiling
======

This is a naive implementation of matrix-matrix multiplication:

.. code-block:: cpp

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
we should expect bad performance.

Matrix-matrix multiplication is one of the most important problems for
numerical calculation, and there are many techniques available for making it
fast.  Most if not all of them are about hiding the memory access latency.
Tiling is a basic technique that delivers impressive speed-up by reordering the
calculation and making it cache-friendly.  The technique is shown in the
example code :ref:`03_matrix_matrix.cpp <nsd-cache-example-03-matrix-matrix>`.

The benchmark results are:

.. list-table:: Performance for matrix-matrix multiplication
  :name: nsd-cache-matrix-tiling
  :header-rows: 1
  :align: center

  * - Flavor
    - Time (s)
    - Gflops
    - Ratio
  * - Naive
    - 3.1151
    - 0.344689 (baseline)
    - 1
  * - MKL
    - 0.0489621
    - 21.9301
    - **63.6229**
  * - Tile 32*32 bytes
    - 1.15064
    - 0.933171
    - 2.70729
  * - Tile 64*64 bytes
    - 0.452658
    - 2.37208
    - **6.88180**
  * - Tile 128*128 bytes
    - 0.748723
    - 1.4341
    - 4.16056
  * - Tile 256*256 bytes
    - 0.833207
    - 1.28869
    - 3.73870
  * - Tile 512*512 bytes
    - 0.764348
    - 1.40478
    - 4.07540
  * - Tile 1024*1024 bytes
    - 0.934039
    - 1.14957
    - 3.33509

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
