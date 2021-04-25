=================
Memory Management
=================

Numerical software tends to use as much memory as a workstation has.  The
memory has two major uses: (i) to hold the required huge amount of data, and
(ii) to gain speed.

Modern computers use a hierarchical memory system.  Registers locate in the
processor chip and are the fastest and scarcest memory.  There is no additional
cycle needed for the CPU to access the bits in registers.

Farther from the CPU, we have cache memory in multiple levels.  It takes 1 to
30 cycles to get data from cache memory to CPU, depending on the level.  Then
we reach the main memory.  Data in main memory takes 50-200 cycles of latency
before getting to CPU.

.. contents:: Contents in the chapter
  :local:
  :depth: 1

Register, Stack, Heap, and Memory Map
=====================================

All data in a computer program take space in memory.  Depending on the usage,
we will allocate them in different places.  The fundamental data types, the
numbers to be crunched, will eventually go into the register file.  Temporary
small objects are allocated on the stack.  Data to be shared among functions go
to dynamically allocated memory.  Depending on the size, the memory manager may
choose to use heap or memory map (mmap).

When talking about memory management, we usually mean dynamic memory
management.  Large chunks of static memory in an executable image shouldn't be
used.

C Dynamic Memory
================

.. contents:: Contents in the section
  :local:
  :depth: 1

The C programming language defines 5 API functions for manipulate dynamic
memory:

* Allocate without initializing

  .. code-block:: c

    void * malloc(size_t size);

* Allocate with initializing with zero

  .. code-block:: c

    void * calloc(size_t num, size_t size);

* Reallocate already allocated memory

  .. code-block:: c

    void * realloc(void * ptr, size_t new_size);

* Free allocated memory

  .. code-block:: c

    void free(void* ptr);

* Allocate memory at an address with the specified alignment

  .. code-block:: c

    void * aligned_alloc(size_t alignment, size_t size);

For convenience, we call a library or part of a library that implements the
dynamic memory management APIs a memory manager.  Although we should focus on
C++ code, it is crucial to know how a C memory manager works, because

1. C++ memory managers are implemented in C.
2. C memory management API can do what C++ cannot.
3. C memory managers sometimes are faster than C++.

With the following example code, I will show how the 5 API functions work, in
the order of :c:func:`!malloc`, :c:func:`!free`, :c:func:`!calloc`,
:c:func:`!realloc`, :c:func:`!aligned_alloc`.

The full code of the example for the C memory manager can be found in
:ref:`cmem.c <nsd-mem-example-cmem>`.  It contains 3 functions :c:func:`!main`,
:c:func:`!outer`, and :c:func:`!inner`.

.. _nsd-mem-example-cmem-main:

:c:func:`!main` function
++++++++++++++++++++++++

.. code-block:: c
  :linenos:

  int main(int argc, char ** argv)
  {
      printf("frame address of main: %p\n", __builtin_frame_address(0));

      outer();

      return 0;
  }

.. _nsd-mem-example-cmem-outer:

:c:func:`!outer` function
+++++++++++++++++++++++++

.. code-block:: c
  :linenos:

  void outer()
  {
      printf("frame address of outer: %p\n", __builtin_frame_address(0));

      int64_t * data = inner(); // Initialize the data.
      printf("data returned from inner: %p\n", data);

      for (size_t it = 0; it < 32; ++it)
      {
          if (data[it] != 200 + it)
          {
              printf("error\n");
          }
      }
      printf("=== malloc tested\n");

      // You must free the memory after you finish using it.  Otherwise it will
      // remain in the process unclaimed, results in the "memory leak".
      free(data);
      //free(data); // Double free results into error.
      printf("=== free tested\n");

      // The following two allocations result in the same zero-initialized array.
      //
      // The first one uses calloc.  If the OS returns the memory that is already
      // zero-initialized, calloc knows, and it doesn't need to redo the zero
      // initialization.
      data = (int64_t *) calloc(32, sizeof(int64_t));
      free(data);
      // The second one uses malloc and manual initialization.  The malloc call
      // does not provide any information about whether the memory is already
      // zero-initialized.
      data = (int64_t *) malloc(32 * sizeof(int64_t));
      // Even if the allocated memory was already zero-initialized by the OS, we
      // still need to do the initialization.
      for (size_t it = 0; it < 32; ++it) { data[it] = 0; }
      free(data);
      printf("=== calloc tested\n");

      // Reallocate the memory with smaller or larger size.
      data = (int64_t *) malloc((1UL << 20) * 2 * sizeof(int64_t));
      printf("address by malloc: %p\n", data);
      data = (int64_t *) realloc(data, (1UL << 20) * 1 * sizeof(int64_t));
      printf("address by realloc to smaller memory: %p\n", data);
      data = (int64_t *) realloc(data, (1UL << 20) * 4 * sizeof(int64_t));
      printf("address by realloc to larger memory: %p\n", data);
      free(data);
      printf("=== realloc tested\n");

      // Aligned allocation.
      int64_t * data1 = (int64_t *) malloc(sizeof(int64_t));
      printf("address by malloc: %p\n", data1);
      int64_t * data2 = (int64_t *) aligned_alloc(256, 256 * sizeof(int64_t));
      printf("address by aligned_alloc: %p\n", data2);
      free(data1);
      free(data2);
      printf("=== aligned_alloc tested\n");
  }

.. _nsd-mem-example-cmem-inner:

:c:func:`!inner` function
+++++++++++++++++++++++++

.. code-block:: c
  :linenos:

  int64_t * inner()
  {
      printf("frame address of inner: %p\n", __builtin_frame_address(0));

      // An array on the stack.  It is popped away when execution leaves this
      // function.  You cannot use the memory outside this function.
      int64_t data_stack[32];

      for (size_t it = 0; it < 32; ++it)
      {
          data_stack[it] = 100 + it;
      }
      printf("stack memory: %p\n", data_stack);

      // A dynamic array.
      int64_t * data_dynamic = (int64_t *) malloc(32 * sizeof(int64_t));

      for (size_t it = 0; it < 32; ++it)
      {
          data_dynamic[it] = 200 + it;
      }
      printf("dynamic memory: %p\n", data_dynamic);

      return data_dynamic;
  }

Execution results
+++++++++++++++++

See the change of local frame:

.. code-block:: none

  frame address of main: 0x7ffee17ea220
  frame address of outer: 0x7ffee17ea210
  frame address of inner: 0x7ffee17ea1e0

Stack address in the :ref:`inner function <nsd-mem-example-cmem-inner>`:

.. code-block:: none

  stack memory: 0x7ffee17ea0d0

Dynamic memory is far away from the stack:

.. code-block:: none

  dynamic memory: 0x7fedf5c05ab0

The allocated dynamic memory is returned to the :ref:`outer function
<nsd-mem-example-cmem-outer>`:

.. code-block:: none

  data returned from inner: 0x7fedf5c05ab0

Showing :c:func:`!malloc`, :c:func:`!free`, and :c:func:`!calloc` work:

.. code-block:: none

  === malloc tested
  === free tested
  === calloc tested

Results of running :c:func:`!realloc` (on macos):

.. code-block:: none

  address by malloc: 0x7fedf6800000
  address by realloc to smaller memory: 0x7fedf6800000
  address by realloc to larger memory: 0x7fedf6800000
  === realloc tested

.. note::

  :c:func:`!realloc` may return a different address.  It depends on the
  implementation of the standard C library, and usually varies with operation
  systems.  For example, on Ubuntu Linux 20.04 LTS, the results are:

  .. code-block:: none

    address by malloc: 0x7f27fd790010
    address by realloc to smaller memory: 0x7f27fd790010
    address by realloc to larger memory: 0x7f27fb78f010
    === realloc tested

Use :c:func:`!aligned_alloc` to allocate memory at 256 (0x100) bytes:

.. code-block:: none

  address by malloc: 0x7fedf5c05ab0
  address by aligned_alloc: 0x7fedf6009800
  === aligned_alloc tested

.. note::

  macos does not provide :c:func:`!aligned_alloc`, but provide
  :c:func:`!posix_memalign`.  We can make a simple wrapper like:

  .. code-block:: c

    #ifdef __APPLE__
    // Macos hasn't implemented the C11 aligned_alloc as of the time 2019/8.
    void * aligned_alloc(size_t alignment, size_t size)
    {
        void * ptr;
        posix_memalign(&ptr, alignment, size);
        return ptr;
    }
    #endif

.. note::

  On Ubuntu Linux 20.04 LTS, the results are:

  .. code-block:: none

    address by malloc: 0x55abd8f127c0
    address by aligned_alloc: 0x55abd8f12800
    === aligned_alloc tested

Never access freed memory
+++++++++++++++++++++++++

In the :ref:`outer function`, after freeing the memory for ``data``:

.. code-block:: c

  // You must free the memory after you finish using it.  Otherwise it will
  // remain in the process unclaimed, results in the "memory leak".
  free(data);
  //free(data); // Double free results into error.
  printf("=== free tested\n");

if it is later accessed, we will not get correct behavior:

.. code-block:: c

  // You may not use the memory that is already freed.  The results is
  // undefined.
  for (size_t it = 0; it < 32; ++it)
  {
      if (data[it] != 200 + it)
      {
          printf("error\n");
      }
  }

C++ Dynamic Memory
==================

.. contents:: Contents in the section
  :local:
  :depth: 1

Objects in C++ have 4 storage durations:

1. static
2. thread
3. automatic
4. dynamic

The first 3 of them, static, thread, and automatic storage durations, are
distinguished by the declarations.  The last one, dynamic storage duration, is
managed by ``operator new/delete`` and our focus in memory management.

There are 3 frequent use cases of the ``new/delete`` expression:

1. Single object allocation.
2. Array allocation.
3. Placement new.

Precisely speaking, only the first two cases are fully related to memory
management.  The third use case doesn't directly allocate or deallocate memory,
but allows to use the ``new/delete`` expression for constructing objects on an
already-allocated block of memory.

The full code of the example for the C++ memory manager can be found in
:ref:`cppmem.cpp <nsd-mem-example-cppmem>`.  There are 3 test functions:
:cpp:func:`!scalar_form`, :cpp:func:`!array_form`, and :cpp:func:`!placement`.
For the test functions, a dummy class is defined:

.. code-block:: cpp
  :linenos:

  /*
   * A dummy class taking 8k bytes.
   */
  struct Block
  {
      Block()
      {
          std::cout << "Block (" << this << ") constructed" << std::endl;
      }
      ~Block()
      {
          std::cout << "Block (" << this << ") destructed" << std::endl;
      }
      int64_t buffer[1024];
  };

.. _nsd-mem-example-cppmem-scalar:

Standard scalar allocation and deallocation
+++++++++++++++++++++++++++++++++++++++++++

The example code for scalar new and delete is:

.. code-block:: cpp
  :linenos:

  void scalar_form()
  {
      std::cout
          << "frame address of scalar_form: " << __builtin_frame_address(0)
          << std::endl;

      // Doing this place 8k bytes on stack.
      Block block_stack;
      for (size_t it = 0; it < 1024; ++it)
      {
          block_stack.buffer[it] = 1000 + it;
      }
      std::cout << "object on stack: " << &block_stack << std::endl;
      std::cout
          << "address difference: "
          << reinterpret_cast<std::size_t>(__builtin_frame_address(0))
           - reinterpret_cast<std::size_t>(&block_stack)
          << ", sizeof(Block): " << sizeof(Block)
          << std::endl;

      // Use the new expression.  Note that this "new" is an expression.  It
      // calls the operator ("::operator new"), but not the operator itself.
      Block * block_dynamic = new Block;
      std::cout << "object on dynamic memory: " << block_dynamic << std::endl;

      for (size_t it = 0; it < 1024; ++it)
      {
          block_dynamic->buffer[it] = 2000 + it;
      }
      std::cout << "=== new tested" << std::endl;

      // The delete expression that destruct and deallocate the memory of the
      // dynamic block object.  Similarly, the expression calls ::operator delete
      // for block_dynamic.
      delete block_dynamic;
      std::cout << "=== delete tested" << std::endl;
  }

The execution results are:

.. code-block:: none

  frame address of scalar_form: 0x7ffee70ab210
  Block (0x7ffee70a91f0) constructed
  object on stack: 0x7ffee70a91f0
  address difference: 8224, sizeof(Block): 8192
  Block (0x7ffea6809800) constructed
  object on dynamic memory: 0x7ffea6809800
  === new tested
  Block (0x7ffea6809800) destructed
  === delete tested
  Block (0x7ffee70a91f0) destructed

.. _nsd-mem-example-cppmem-array:

Array allocation and deallocation
+++++++++++++++++++++++++++++++++

The example code for array new and delete is:

.. code-block:: cpp
  :linenos:

  void array_form()
  {
      // An array on the stack.  It is popped away when execution leaves this
      // function.  You cannot use the memory outside this function.
      int64_t data_stack[32];

      for (size_t it = 0; it < 32; ++it)
      {
          data_stack[it] = 100 + it;
      }
      std::cout << "stack array memory: " << data_stack << std::endl;

      // A dynamic array.
      int64_t * data_dynamic = new int64_t[32];

      for (size_t it = 0; it < 32; ++it)
      {
          data_dynamic[it] = 200 + it;
      }
      std::cout << "dynamic array memory: " << data_dynamic << std::endl;
      std::cout << "=== new[] tested" << std::endl;

      delete[] data_dynamic;
      std::cout << "=== delete[] tested" << std::endl;
  }

The execution results are:

.. code-block:: none

  stack array memory: 0x7ffee70ab0f0
  dynamic array memory: 0x7ffea6405ab0
  === new[] tested
  === delete[] tested

.. _nsd-mem-example-cppmem-placement:

Placement new
+++++++++++++

The example code for placement new is:

.. code-block:: cpp
  :linenos:

  void placement()
  {
      char * buffer = new char[sizeof(Block)];

      Block * block = new (buffer) Block;
      for (size_t it = 0; it < 1024; ++it)
      {
          block->buffer[it] = it;
      }
      std::cout << "=== placement new tested" << std::endl;

      // Instead of deleting the pointer block, call explicit the destructor and
      // delete the original buffer.
      block->~Block();
      delete[] buffer;
  }

The execution results are:

.. code-block:: none

  Block (0x7ffea6809800) constructed
  === placement new tested
  Block (0x7ffea6809800) destructed

.. note::

  Do not use ``operator delete`` with an object constructed using placement
  new:

  .. code-block:: cpp

    // This induces undefined behavior.  Don't do this.
    delete block;

  It causes double free (tested on macos):

  .. code-block:: none

    cppmem(34359,0x1167b5e00) malloc: *** error for object 0x7f89e5009800: pointer being freed was not allocated
    cppmem(34359,0x1167b5e00) malloc: *** set a breakpoint in malloc_error_break to debug

  The reason is that the memory buffer is managed separately:

  .. code-block:: cpp

    // Instead of deleting the pointer block, call explicit the destructor and
    // delete the original buffer.
    block->~Block();
    delete[] buffer;

STL Allocator
=============

.. contents:: Contents in the section
  :local:
  :depth: 1

STL uses another set of template API for allocating the memory for most of its
container.  By default, the STL containers use ``std::allocator`` class
template for memory allocation.  We are allowed to provide custom allocators to
the containers.

We will use an example to show how a STL allocator works with ``std::vector``.
The example counts the number of bytes allocated by the container.  The full
code can be found in :ref:`alloc.cpp <nsd-mem-example-alloc>`.  It has three
parts: (i) the byte counter, (ii) the STL allocator, and (iii) the test code.

.. _nsd-mem-example-alloc-byte:

Byte counter
++++++++++++

.. code-block:: cpp
  :linenos:

  struct ByteCounterImpl
  {

      std::atomic_size_t allocated = 0;
      std::atomic_size_t deallocated = 0;
      std::atomic_size_t refcount = 0;

  }; /* end struct ByteCounterImpl */

  /**
   * One instance of this counter is shared among a set of allocators.
   *
   * The counter keeps track of the bytes allocated and deallocated, and report
   * those two numbers in addition to bytes that remain allocated.
   */
  class ByteCounter
  {

  public:

      ByteCounter()
        : m_impl(new ByteCounterImpl)
      { incref(); }

      ByteCounter(ByteCounter const & other)
        : m_impl(other.m_impl)
      { incref(); }

      ByteCounter & operator=(ByteCounter const & other)
      {
          if (&other != this)
          {
              decref();
              m_impl = other.m_impl;
              incref();
          }

          return *this;
      }

      ByteCounter(ByteCounter && other)
        : m_impl(other.m_impl)
      { other.decref(); }

      ByteCounter & operator=(ByteCounter && other)
      {
          if (&other != this)
          {
              decref();
              m_impl = other.m_impl;
          }

          return *this;
      }

      ~ByteCounter() { decref(); }

      void swap(ByteCounter & other)
      {
          std::swap(m_impl, other.m_impl);
      }

      void increase(std::size_t amount)
      {
          m_impl->allocated += amount;
      }

      void decrease(std::size_t amount)
      {
          m_impl->deallocated += amount;
      }

      std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
      std::size_t allocated() const { return m_impl->allocated; }
      std::size_t deallocated() const { return m_impl->deallocated; }
      /* This is for debugging. */
      std::size_t refcount() const { return m_impl->refcount; }

  private:

      void incref() { ++m_impl->refcount; }

      void decref()
      {
          if (nullptr == m_impl)
          {
              // Do nothing.
          }
          else if (1 == m_impl->refcount)
          {
              delete m_impl;
              m_impl = nullptr;
          }
          else
          {
              --m_impl->refcount;
          }
      }

      ByteCounterImpl * m_impl;

  }; /* end class ByteCounter */

.. _nsd-mem-example-alloc-alloc:

Simple allocator
++++++++++++++++

.. code-block:: cpp
  :linenos:

  /**
   * Very simple allocator that counts the number of bytes allocated through it.
   *
   * It's made to demonstrate the STL allocator and only works in this example.
   * A lot of modification is needed to use it in a real application.
   */
  template <class T>
  struct MyAllocator
  {

      using value_type = T;

      // Just use the default constructor of ByteCounter for the data member
      // "counter".
      MyAllocator() = default;

      template <class U> constexpr
      MyAllocator(const MyAllocator<U> & other) noexcept
      {
          counter = other.counter;
      }

      T * allocate(std::size_t n)
      {
          if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
          {
              throw std::bad_alloc();
          }
          const std::size_t bytes = n*sizeof(T);
          T * p = static_cast<T *>(std::malloc(bytes));
          if (p)
          {
              counter.increase(bytes);
              return p;
          }
          else
          {
              throw std::bad_alloc();
          }
      }

      void deallocate(T* p, std::size_t n) noexcept
      {
          std::free(p);

          const std::size_t bytes = n*sizeof(T);
          counter.decrease(bytes);
      }

      ByteCounter counter;

  }; /* end struct MyAllocator */

  template <class T, class U>
  bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
  {
      return a.counter == b.counter;
  }

  template <class T, class U>
  bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
  {
      return !(a == b);
  }

.. _nsd-mem-example-alloc-vector:

Bytes allocated by ``std::vector``
++++++++++++++++++++++++++++++++++

Now this shows the execution results for the example of STL allocator.  To
begin, create the allocator object:

.. code-block:: cpp

  MyAllocator<size_t> alloc;

Create an empty ``std::vector``:

.. code-block:: cpp

  std::vector<size_t, MyAllocator<size_t>> vec1(alloc);
  std::cout << alloc << std::endl;

Nothing is allocated, as expected:

.. code-block:: none

  allocator: bytes = 0 allocated = 0 deallocated = 0

Then populate 1024 elements to the vector:

.. code-block:: cpp

  for (size_t it=0; it<1024; ++it)
  {
      vec1.push_back(it);
  }
  std::cout << alloc << std::endl;

8192 bytes remain in the container:

.. code-block:: none

  allocator: bytes = 8192 allocated = 16376 deallocated = 8184

The total number of bytes allocated is almost twice the remaining bytes, and
there are bytes deallocated.  It is the overhead incurred by
``std::vector::push_back``.

Use ``std::swap`` to get rid of contents in ``vec1``:

.. code-block:: cpp

  std::vector<size_t, MyAllocator<size_t>>(alloc).swap(vec1);
  std::cout << alloc << std::endl;

No bytes remain:

.. code-block:: none

  allocator: bytes = 0 allocated = 16376 deallocated = 16376

Create another ``std::vector``, named ``vec2``, and ask for 1024 elements on
construction:

.. code-block:: cpp

  std::vector<size_t, MyAllocator<size_t>> vec2(1024, alloc);
  std::cout << alloc << std::endl;

This time the bytes of deallocation do not increase.  The second construction
does not have the overhead incurred by ``push_back`` (which is not used):

.. code-block:: none

  allocator: bytes = 8192 allocated = 24568 deallocated = 16376

Test to see how the move semantics works by using the third object ``vec3``:

.. code-block:: cpp

  std::vector<size_t, MyAllocator<size_t>> vec3(std::move(vec2));
  std::cout << alloc << std::endl;

No memory allocation or deallocation happen, as expected:

.. code-block:: none

  allocator: bytes = 8192 allocated = 24568 deallocated = 16376

In the end, discard the contents of the third container:

.. code-block:: cpp

  std::vector<size_t, MyAllocator<size_t>>(alloc).swap(vec3);
  std::cout << alloc << std::endl;

All bytes are freed:

.. code-block:: none

  allocator: bytes = 0 allocated = 24568 deallocated = 24568

Instance Counter
================

In some cases, we want to know how many instances are created of certain
classes.  One quick way is to add an instance counter for the specific class.
The number of instances is available at any given time point.  The full source
code of the counter can be found in :ref:`icount.cpp <nsd-mem-example-icount>`.

Counter Template
++++++++++++++++

This is a very simple counter implementation that only works in limited
scenarios, e.g., single-threaded environment.  But it's sufficient as an
example.

.. code-block:: cpp
  :linenos:

  template <class T>
  class InstanceCounter
  {

  public:

      InstanceCounter() { ++m_constructed; }
      InstanceCounter(InstanceCounter const & other) { ++m_copied; }
      ~InstanceCounter() { ++m_destructed; }

      static std::size_t active()
      {
          return m_constructed + m_copied - m_destructed;
      }
      static std::size_t constructed() { return m_constructed; }
      static std::size_t copied() { return m_copied; }
      static std::size_t destructed() { return m_destructed; }

  private:

      static std::atomic_size_t m_constructed;
      static std::atomic_size_t m_copied;
      static std::atomic_size_t m_destructed;

  }; /* end class InstanceCounter */

  // Compiler will make sure these static variables are defined only once.
  template <class T> std::atomic_size_t InstanceCounter<T>::m_constructed = 0;
  template <class T> std::atomic_size_t InstanceCounter<T>::m_copied = 0;
  template <class T> std::atomic_size_t InstanceCounter<T>::m_destructed = 0;

Use the Counter
+++++++++++++++

To show the use of the counter, make two classes:

.. code-block:: cpp
  :linenos:

  struct Data
    : public InstanceCounter<Data>
  {

      std::size_t buffer[1024];

  }; /* end struct Data */

  struct Data2
    : public InstanceCounter<Data2>
  {

      Data2() = default;
      Data2(Data2 const & other)
  #if 0
      // Don't forget to call the base class copy constructor.  The implicit copy
      // constructor calls it for you.  But when you have custom copy
      // constructor, if you do not specify the base constructor, the default
      // constructor in the base class is used.
        : InstanceCounter<Data2>(other)
  #endif
      {
          std::copy_n(other.buffer, 1024, buffer);
      }
      Data2 & operator=(Data2 const & other)
      {
          std::copy_n(other.buffer, 1024, buffer);
          return *this;
      }

      std::size_t buffer[1024];

  }; /* end struct Data */

Count at Construction
+++++++++++++++++++++

Now we can run a test program.  Both ``Data`` and ``Data2`` will be
instantiated.  First it's ``Data``:

.. code-block:: cpp

  // Data.
  Data * data = new Data();
  report<Data> ("Data  (default construction)  ");

  Data * data_copied = new Data(*data);
  report<Data> ("Data  (copy construction)     ");

  std::vector<Data> dvec(64);
  report<Data> ("Data  (construction in vector)");

The results are:

.. code-block:: none

  Data  (default construction)   instance: active = 1 constructed = 1 copied = 0 destructed = 0
  Data  (copy construction)      instance: active = 2 constructed = 1 copied = 1 destructed = 0
  Data  (construction in vector) instance: active = 66 constructed = 65 copied = 1 destructed = 0

Then it's ``Data2``:

.. code-block:: cpp

  // Data2.
  Data2 * data2 = new Data2();
  report<Data2>("Data2 (default construction)  ");

  Data2 * data2_copied = new Data2(*data2);
  report<Data2>("Data2 (copy construction)     ");

  std::vector<Data2> d2vec(64);
  report<Data2>("Data2 (construction in vector)");

The results are slightly different:

.. code-block:: none

  Data2 (default construction)   instance: active = 1 constructed = 1 copied = 0 destructed = 0
  Data2 (copy construction)      instance: active = 2 constructed = 2 copied = 0 destructed = 0
  Data2 (construction in vector) instance: active = 66 constructed = 66 copied = 0 destructed = 0

``InstanceCounter<Data2>`` does not work correctly for copy construction!  We
have documented the reason in the code:

.. code-block:: cpp

  #if 0
      // Don't forget to call the base class copy constructor.  The implicit copy
      // constructor calls it for you.  But when you have custom copy
      // constructor, if you do not specify the base constructor, the default
      // constructor in the base class is used.
        : InstanceCounter<Data2>(other)
  #endif

C++ programmers need to be familiar with the behaviors of construction.

Count at Destruction
++++++++++++++++++++

Test the destruction:

.. code-block:: cpp

  // Data.
  std::vector<Data>().swap(dvec);
  report<Data>("Data ");
  delete data;
  report<Data>("Data ");
  delete data_copied;
  report<Data>("Data ");

  // Data2.
  std::vector<Data2>().swap(d2vec);
  report<Data2>("Data2");
  delete data2;
  report<Data2>("Data2");
  delete data2_copied;
  report<Data2>("Data2");

We made no mistakes in the destructor so the number will be correct with
destruction of both classes:

.. code-block:: none

  Data  instance: active = 2 constructed = 65 copied = 1 destructed = 64
  Data  instance: active = 1 constructed = 65 copied = 1 destructed = 65
  Data  instance: active = 0 constructed = 65 copied = 1 destructed = 66
  Data2 instance: active = 2 constructed = 66 copied = 0 destructed = 64
  Data2 instance: active = 1 constructed = 66 copied = 0 destructed = 65
  Data2 instance: active = 0 constructed = 66 copied = 0 destructed = 66

Exercises
=========

1. Calling ``delete`` on the address returned by ``new[]`` may cause problems.
   Write a program and analyze what the problems may be.
2. When using a single thread, what is the runtime overhead of the instance
   counting technique?  Write a program and analyze.

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
