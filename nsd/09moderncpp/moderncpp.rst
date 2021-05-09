==========
Modern C++
==========

.. contents:: Contents in the chapter
  :local:
  :depth: 1

Copy Elision / Return Value Optimization (RVO)
==============================================

.. contents:: Contents in the section
  :local:
  :depth: 1

Copy elision is one of the two forms of optimization that are allowed to change
the side effects.  (The other is allocation elision and extension.)

Sometimes copy elision is also called return value optimization (RVO) or named
return value optimization (NRVO).

In the following example, even though no optimization flag is used, the copy is
optimized out.  The full example code is in :ref:`01_copy.cpp
<nsd-moderncpp-example-copy>`.

To show that the copy construction is not called, we will use a help class:

.. code-block:: cpp
  :linenos:

  class IsCopied
  {
  public:
      static IsCopied & instance()
      {
          // This is a singleton.
          static IsCopied inst;
          return inst;
      }

      IsCopied & void on() { m_status = true; return *this; }
      operator bool() const { return m_status; }
      ~IsCopied() = default;

  private:
      IsCopied() : m_status(false) {}
      IsCopied(IsCopied const & ) = delete;
      IsCopied(IsCopied       &&) = delete;
      IsCopied & operator=(IsCopied const & ) = delete;
      IsCopied & operator=(IsCopied       &&) = delete;
      bool m_status;
  };

.. note::

  In the copy constructor of the class of interest is called, an instance of
  this class ``IsCopied`` will set the status to be true.  This helper class is
  a singleton so there will be only one instance in the process.

In the testing program, we have a ``Data`` class whose copy status is tracked:

.. code-block:: cpp
  :linenos:

  class Data
  {
  public:
      constexpr const static size_t NELEM = 1024*8;
      Data()
      {
          std::cout << "Data constructed @" << this << std::endl;
      }
      Data(Data const & other)
      {
          copy_from(other);
          std::cout << "Data copied to @" << this << " from @" << &other << std::endl;
      }
      ~Data()
      {
          std::cout << "Data destructed @" << this << std::endl;
      }
  private:
      void copy_from(Data const & other)
      {
          for (size_t it=0; it < NELEM; ++it)
          {
              m_buffer[it] = other.m_buffer[it];
          }
          // Mark copied.
          IsCopied::instance().on();
      }
      // A lot of data that we don't want to reconstruct.
      int m_buffer[NELEM];
  };

In the beginning, check that nothing is copied before anything is constructed:

.. code-block:: cpp

  std::cout
      << (bool(IsCopied::instance()) ? "Something" : "Nothing")
      << " is copied" << std::endl;

Indeed there is nothing copied:

.. code-block:: none

  Nothing is copied

Now we call a series of helper functions to construct, process, and return the
``Data`` object:

.. code-block:: cpp

  Data data = worker2();

Let us take a look at the helper function ``worker2()`` and its inner helpers:

.. code-block:: cpp
  :caption: Manipulation code.

  void manipulate_with_reference(Data & data, int value)
  {
      std::cout << "Manipulate with reference: " << &data << std::endl;

      for (size_t it=0; it < data.size(); ++it)
      {
          data[it] = value + it;
      }
      // In a real consumer function we will do much more meaningful operations.

      // However, we cannot destruct an object passed in with a reference.
  }

.. code-block:: cpp
  :caption: Inner helper function that construct ``Data``.

  Data worker1()
  {
      Data data;

      // Manipulate the Data object.
      manipulate_with_reference(data, 3);

      return data;
  }

.. code-block:: cpp
  :caption: Outer helper function that obtain ``Data`` from ``worker1()``.

  Data worker2()
  {
      Data data = worker1();

      // Manipulate the Data object, again.
      manipulate_with_reference(data, 8);

      return data;
  }

You may be surprised when seeing the terminal prints showing no message of copy
construction:

.. code-block:: none

  Data constructed @0x7ffee9ebe1c0
  Manipulate with reference: 0x7ffee9ebe1c0
  Manipulate with reference: 0x7ffee9ebe1c0

.. note::

  You may guess that some optimizations disabled the copy construction, but the
  results remain the same with optimization:

  .. code-block:: console

    $ g++ 01_copy.cpp -o 01_copy -std=c++17 -g -O3

  or without it:

  .. code-block:: console

    $ g++ 01_copy.cpp -o 01_copy -std=c++17 -g -O0

We can call our copy checker again:

.. code-block:: cpp

  std::cout
      << (bool(IsCopied::instance()) ? "Something" : "Nothing")
      << " is copied" << std::endl;

It is confirmed that copy constructor is not called:

.. code-block:: none

  Nothing is copied
  Data destructed @0x7ffee9ebe1c0

Move Semantics and Copy Elision
===============================

Move semantics greatly helps us to avoid copying expensive resources.  To take
advantage of that, our ``Data`` class should be changed to use dynamic
allocation:

.. code-block:: cpp
  :linenos:

  class Data
  {

  public:

      constexpr const static size_t NELEM = 1024*8;

      Data()
      {
          m_buffer = new int[NELEM];
          std::cout << "Data constructed @" << this << std::endl;
      }

      Data(Data const & other)
      {
          m_buffer = new int[NELEM];
          copy_from(other);
          std::cout << "Data copied to @" << this << " from @" << &other << std::endl;
      }

      Data & operator=(Data const & other)
      {
          if (nullptr == m_buffer) { m_buffer = new int[NELEM]; }
          copy_from(other);
          std::cout << "Data copy assigned to @" << this << " from @" << &other << std::endl;
          return *this;
      }

      Data(Data && other)
      {
          m_buffer = other.m_buffer;
          other.m_buffer = nullptr;
          std::cout << "Data moved to @" << this << " from @" << &other << std::endl;
          Status::instance().set_moved();
      }

      Data & operator=(Data && other)
      {
          if (m_buffer) { delete[] m_buffer; }
          m_buffer = other.m_buffer;
          other.m_buffer = nullptr;
          std::cout << "Data move assigned to @" << this << " from @" << &other << std::endl;
          Status::instance().set_moved();
          return *this;
      }

      ~Data()
      {
          if (m_buffer) { delete[] m_buffer; }
          std::cout << "Data destructed @" << this << std::endl;
      }
  }

Forced Move Is a Bad Idea
+++++++++++++++++++++++++

Although the move semantics indeed avoids copy the expensive buffer in the
``Data`` class, it cannot avoid copy the ``Data`` object itself.  However, copy
elision (RVO & NRVO) can.

.. code-block:: cpp
  :linenos:

  Data worker1()
  {
      Data data;

      // Manipulate the Data object.
      manipulate_with_reference(data, 3);

      return data;
  }

  Data worker2()
  {
      Data data = worker1();

      // Manipulate the Data object, again.
      manipulate_with_reference(data, 8);

  #ifdef FORCEMOVE
      // Explicit move semantics destroys copy elision.
      return std::move(data);
  #else
      return data;
  #endif
  }

  int main(int argc, char ** argv)
  {
      std::cout
          << "Status:"
          << (bool(Status::instance().is_copied()) ? " copied" : " uncopied")
          << (bool(Status::instance().is_moved()) ? " moved" : " unmoved")
          << std::endl;
      Data data = worker2();
      std::cout
          << "Status:"
          << (bool(Status::instance().is_copied()) ? " copied" : " uncopied")
          << (bool(Status::instance().is_moved()) ? " moved" : " unmoved")
          << std::endl;
  }

Compiler Does Copy Elision
++++++++++++++++++++++++++

.. admonition:: Execution Results

  :download:`code/03_elision/02_move.cpp`

  .. code-block:: console
    :caption: Build ``02_move.cpp`` without optimization

    $ g++ 02_move.cpp -o 02_move -std=c++17 -g

  .. code-block:: console
    :caption: Run ``02_move``
    :linenos:

    $ ./02_move
    Status: uncopied unmoved
    Data constructed @0x7ffee0ac61b8
    Manipulate with reference: 0x7ffee0ac61b8
    Manipulate with reference: 0x7ffee0ac61b8
    Status: uncopied unmoved
    Data destructed @0x7ffee0ac61b8

Forced Move Incurs More Operations
++++++++++++++++++++++++++++++++++

.. admonition:: Execution Results

  :download:`code/03_elision/02_move.cpp`

  .. code-block:: console
    :caption: Build ``02_move.cpp`` without optimization

    $ g++ 02_move.cpp -o 02_move -std=c++17 -g  -DFORCEMOVE

  .. code-block:: console
    :caption: Run ``02_move``
    :linenos:

    $ ./02_move
    Status: uncopied unmoved
    Data constructed @0x7ffee90cb0d0
    Manipulate with reference: 0x7ffee90cb0d0
    Manipulate with reference: 0x7ffee90cb0d0
    Data moved to @0x7ffee90cb168 from @0x7ffee90cb0d0
    Data destructed @0x7ffee90cb0d0
    Status: uncopied moved
    Data destructed @0x7ffee90cb168

Data Concatenation
==================

Because of copy elision, for readability it is prefer to write:

.. code-block:: cpp

  std::vector<int> worker_return();

than

.. code-block:: cpp

  void worker_argument(std::vector<int> & output /* output argument */);


Because in consumer code:

.. code-block:: cpp
  :linenos:

  // It reads clearly that the worker produces new result.
  std::vector<int> result = worker_return();

  // It takes a second to understand that the worker is using result as a buffer
  // for output.
  std::vector<int> result;
  worker_argument(result);

  /*
   * The result is pre-populated before sending to the worker.  From the
   * following lines we can't know how the worker will use result.
   *
   * By reading the worker signature we know that result may be used for output.
   * We can only be sure that result is used for output after reading the full
   * implemnetation of the worker.
   *
   * The worker may or may not expect the output argument to be pre-populated.
   * Regardless, it has to use runtime check to ensure either case.
   */
  std::vector<int> result(100);
  std::fill(result.begin(), result.end(), 7);
  worker_argument(result);

The ambiguity is a productivity killer.  (Runtime performance is another
story.)

Style 1: Return ``vector``
++++++++++++++++++++++++++

The first style returns a vector from inner and appends it in outer.  It is
easier to read and test.  The inner worker:

.. code-block:: cpp
  :linenos:

  std::vector<Data> inner1(size_t start, size_t len)
  {
      std::cout << "** inner1 begins with " << start << std::endl;
      std::vector<Data> ret;
      for (size_t it=0; it < len; ++it)
      {
          Data data(start+it);
          ret.emplace_back(std::move(data));
      }
      return ret;
  }

The outer worker:

.. code-block:: cpp
  :linenos:

  void outer1(size_t len)
  {
      std::cout << "* outer1 begins" << std::endl;
      std::vector<Data> vec;
      for (size_t it=0; it < len; ++it)
      {
          std::cout << std::endl;
          std::cout << "* outer1 loop it=" << it << " begins" << std::endl;
          std::vector<Data> subvec = inner1(vec.size(), it+1);
          std::cout << "* outer1 obtained inner1 at " << vec.size() << std::endl;
          vec.insert(
              vec.end()
            , std::make_move_iterator(subvec.begin())
            , std::make_move_iterator(subvec.end())
          );
          std::cout << "* outer1 inserted subvec.size()=" << subvec.size() << std::endl;
      }
      std::cout << "* outer1 result.size() = " << vec.size() << std::endl << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/03_elision/03_accumulate.cpp`

  .. code-block:: console
    :caption: Build ``03_accumulate.cpp`` with `-DOTYPE=1`

    $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DOTYPE=1

  .. code-block:: console
    :caption: Run ``03_accumulate``
    :linenos:

    $ ./03_accumulate
    * outer1 begins

    * outer1 loop it=0 begins
    ** inner1 begins with 0
    Data #0 constructed @0x7ffee4a620c8
    Data #0 moved to @0x7fe29d405ab0 from @0x7ffee4a620c8
    Data #0 destructed @0x7ffee4a620c8
    * outer1 obtained inner1 at 0
    Data #0 moved to @0x7fe29d405ac0 from @0x7fe29d405ab0
    * outer1 inserted subvec.size()=1
    Data #0 destructed @0x7fe29d405ab0

    * outer1 loop it=1 begins
    ** inner1 begins with 1
    Data #1 constructed @0x7ffee4a620c8
    Data #1 moved to @0x7fe29d405ab0 from @0x7ffee4a620c8
    Data #1 destructed @0x7ffee4a620c8
    Data #2 constructed @0x7ffee4a620c8
    Data #2 moved to @0x7fe29d405ae0 from @0x7ffee4a620c8
    Data #1 copied to @0x7fe29d405ad0 from @0x7fe29d405ab0
    Data #1 destructed @0x7fe29d405ab0
    Data #2 destructed @0x7ffee4a620c8
    * outer1 obtained inner1 at 1
    Data #1 moved to @0x7fe29d405b00 from @0x7fe29d405ad0
    Data #2 moved to @0x7fe29d405b10 from @0x7fe29d405ae0
    Data #0 copied to @0x7fe29d405af0 from @0x7fe29d405ac0
    Data #0 destructed @0x7fe29d405ac0
    * outer1 inserted subvec.size()=2
    Data #2 destructed @0x7fe29d405ae0
    Data #1 destructed @0x7fe29d405ad0

    * outer1 loop it=2 begins
    ** inner1 begins with 3
    Data #3 constructed @0x7ffee4a620c8
    Data #3 moved to @0x7fe29d504080 from @0x7ffee4a620c8
    Data #3 destructed @0x7ffee4a620c8
    Data #4 constructed @0x7ffee4a620c8
    Data #4 moved to @0x7fe29d5040a0 from @0x7ffee4a620c8
    Data #3 copied to @0x7fe29d504090 from @0x7fe29d504080
    Data #3 destructed @0x7fe29d504080
    Data #4 destructed @0x7ffee4a620c8
    Data #5 constructed @0x7ffee4a620c8
    Data #5 moved to @0x7fe29d5040d0 from @0x7ffee4a620c8
    Data #4 copied to @0x7fe29d5040c0 from @0x7fe29d5040a0
    Data #3 copied to @0x7fe29d5040b0 from @0x7fe29d504090
    Data #4 destructed @0x7fe29d5040a0
    Data #3 destructed @0x7fe29d504090
    Data #5 destructed @0x7ffee4a620c8
    * outer1 obtained inner1 at 3
    Data #3 moved to @0x7fe29d504120 from @0x7fe29d5040b0
    Data #4 moved to @0x7fe29d504130 from @0x7fe29d5040c0
    Data #5 moved to @0x7fe29d504140 from @0x7fe29d5040d0
    Data #2 copied to @0x7fe29d504110 from @0x7fe29d405b10
    Data #1 copied to @0x7fe29d504100 from @0x7fe29d405b00
    Data #0 copied to @0x7fe29d5040f0 from @0x7fe29d405af0
    Data #2 destructed @0x7fe29d405b10
    Data #1 destructed @0x7fe29d405b00
    Data #0 destructed @0x7fe29d405af0
    * outer1 inserted subvec.size()=3
    Data #5 destructed @0x7fe29d5040d0
    Data #4 destructed @0x7fe29d5040c0
    Data #3 destructed @0x7fe29d5040b0
    * outer1 result.size() = 6

    Data #5 destructed @0x7fe29d504140
    Data #4 destructed @0x7fe29d504130
    Data #3 destructed @0x7fe29d504120
    Data #2 destructed @0x7fe29d504110
    Data #1 destructed @0x7fe29d504100
    Data #0 destructed @0x7fe29d5040f0

The unwanted copies come from ``std::vector`` resizing.  To mitigate it, we
should mark the move constructor with ``noexcept``:

.. code-block:: cpp

  Data(Data && other) noexcept
  {
      m_serial = other.m_serial;
      m_buffer = other.m_buffer;
      other.m_buffer = nullptr;
      std::cout << "Data #" << m_serial << " moved to @" << this << " from @" << &other << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/03_elision/03_accumulate.cpp`

  .. code-block:: console
    :caption: Build ``03_accumulate.cpp`` with `-DMOVENOEXCEPT -DOTYPE=1`

    $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DMOVENOEXCEPT -DOTYPE=1

  .. code-block:: console
    :caption: Run ``03_accumulate``
    :linenos:

    $ ./03_accumulate
    * outer1 begins

    * outer1 loop it=0 begins
    ** inner1 begins with 0
    Data #0 constructed @0x7ffee11510a8
    Data #0 moved to @0x7fcf16405ab0 from @0x7ffee11510a8
    Data #0 destructed @0x7ffee11510a8
    * outer1 obtained inner1 at 0
    Data #0 moved to @0x7fcf16405ac0 from @0x7fcf16405ab0
    * outer1 inserted subvec.size()=1
    Data #0 destructed @0x7fcf16405ab0

    * outer1 loop it=1 begins
    ** inner1 begins with 1
    Data #1 constructed @0x7ffee11510a8
    Data #1 moved to @0x7fcf16405ab0 from @0x7ffee11510a8
    Data #1 destructed @0x7ffee11510a8
    Data #2 constructed @0x7ffee11510a8
    Data #2 moved to @0x7fcf16405ae0 from @0x7ffee11510a8
    Data #1 moved to @0x7fcf16405ad0 from @0x7fcf16405ab0
    Data #1 destructed @0x7fcf16405ab0
    Data #2 destructed @0x7ffee11510a8
    * outer1 obtained inner1 at 1
    Data #1 moved to @0x7fcf16405b00 from @0x7fcf16405ad0
    Data #2 moved to @0x7fcf16405b10 from @0x7fcf16405ae0
    Data #0 moved to @0x7fcf16405af0 from @0x7fcf16405ac0
    Data #0 destructed @0x7fcf16405ac0
    * outer1 inserted subvec.size()=2
    Data #2 destructed @0x7fcf16405ae0
    Data #1 destructed @0x7fcf16405ad0

    * outer1 loop it=2 begins
    ** inner1 begins with 3
    Data #3 constructed @0x7ffee11510a8
    Data #3 moved to @0x7fcf16504080 from @0x7ffee11510a8
    Data #3 destructed @0x7ffee11510a8
    Data #4 constructed @0x7ffee11510a8
    Data #4 moved to @0x7fcf165040a0 from @0x7ffee11510a8
    Data #3 moved to @0x7fcf16504090 from @0x7fcf16504080
    Data #3 destructed @0x7fcf16504080
    Data #4 destructed @0x7ffee11510a8
    Data #5 constructed @0x7ffee11510a8
    Data #5 moved to @0x7fcf165040d0 from @0x7ffee11510a8
    Data #4 moved to @0x7fcf165040c0 from @0x7fcf165040a0
    Data #3 moved to @0x7fcf165040b0 from @0x7fcf16504090
    Data #4 destructed @0x7fcf165040a0
    Data #3 destructed @0x7fcf16504090
    Data #5 destructed @0x7ffee11510a8
    * outer1 obtained inner1 at 3
    Data #3 moved to @0x7fcf16504120 from @0x7fcf165040b0
    Data #4 moved to @0x7fcf16504130 from @0x7fcf165040c0
    Data #5 moved to @0x7fcf16504140 from @0x7fcf165040d0
    Data #2 moved to @0x7fcf16504110 from @0x7fcf16405b10
    Data #1 moved to @0x7fcf16504100 from @0x7fcf16405b00
    Data #0 moved to @0x7fcf165040f0 from @0x7fcf16405af0
    Data #2 destructed @0x7fcf16405b10
    Data #1 destructed @0x7fcf16405b00
    Data #0 destructed @0x7fcf16405af0
    * outer1 inserted subvec.size()=3
    Data #5 destructed @0x7fcf165040d0
    Data #4 destructed @0x7fcf165040c0
    Data #3 destructed @0x7fcf165040b0
    * outer1 result.size() = 6

    Data #5 destructed @0x7fcf16504140
    Data #4 destructed @0x7fcf16504130
    Data #3 destructed @0x7fcf16504120
    Data #2 destructed @0x7fcf16504110
    Data #1 destructed @0x7fcf16504100
    Data #0 destructed @0x7fcf165040f0

Style 2: Use Output ``vector``
++++++++++++++++++++++++++++++

The second style uses an output argument which is passed from outer to inner.
The inner worker:

.. code-block:: cpp
  :linenos:

  void inner2(size_t start, size_t len, std::vector<Data> & result /* for output */)
  {
      std::cout << "** inner2 begins with " << start << std::endl;
      for (size_t it=0; it < len; ++it)
      {
          Data data(start+it);
          result.emplace_back(std::move(data));
      }
  }

The outer worker:

.. code-block:: cpp
  :linenos:

  void outer2(size_t len)
  {
      std::cout << "* outer2 begins" << std::endl;
      std::vector<Data> vec;
      for (size_t it=0; it < len; ++it)
      {
          std::cout << std::endl;
          std::cout << "* outer2 loop it=" << it << " begins" << std::endl;
          inner2(vec.size(), it+1, vec);
      }
      std::cout << "* outer2 result.size() = " << vec.size() << std::endl << std::endl;
  }

There is no longer the intermediate vector and it saves quite a number of
movement.  The prize we pay is less testability.

.. admonition:: Execution Results

  :download:`code/03_elision/03_accumulate.cpp`

  .. code-block:: console
    :caption: Build ``03_accumulate.cpp`` with `-DMOVENOEXCEPT -DOTYPE=2`

    $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DMOVENOEXCEPT -DOTYPE=2

  .. code-block:: console
    :caption: Run ``03_accumulate``
    :linenos:

    $ ./03_accumulate
    * outer2 begins

    * outer2 loop it=0 begins
    ** inner2 begins with 0
    Data #0 constructed @0x7ffee68c60c8
    Data #0 moved to @0x7f898c405ab0 from @0x7ffee68c60c8
    Data #0 destructed @0x7ffee68c60c8

    * outer2 loop it=1 begins
    ** inner2 begins with 1
    Data #1 constructed @0x7ffee68c60c8
    Data #1 moved to @0x7f898c405ad0 from @0x7ffee68c60c8
    Data #0 moved to @0x7f898c405ac0 from @0x7f898c405ab0
    Data #0 destructed @0x7f898c405ab0
    Data #1 destructed @0x7ffee68c60c8
    Data #2 constructed @0x7ffee68c60c8
    Data #2 moved to @0x7f898c405b00 from @0x7ffee68c60c8
    Data #1 moved to @0x7f898c405af0 from @0x7f898c405ad0
    Data #0 moved to @0x7f898c405ae0 from @0x7f898c405ac0
    Data #1 destructed @0x7f898c405ad0
    Data #0 destructed @0x7f898c405ac0
    Data #2 destructed @0x7ffee68c60c8

    * outer2 loop it=2 begins
    ** inner2 begins with 3
    Data #3 constructed @0x7ffee68c60c8
    Data #3 moved to @0x7f898c405b10 from @0x7ffee68c60c8
    Data #3 destructed @0x7ffee68c60c8
    Data #4 constructed @0x7ffee68c60c8
    Data #4 moved to @0x7f898c405b60 from @0x7ffee68c60c8
    Data #3 moved to @0x7f898c405b50 from @0x7f898c405b10
    Data #2 moved to @0x7f898c405b40 from @0x7f898c405b00
    Data #1 moved to @0x7f898c405b30 from @0x7f898c405af0
    Data #0 moved to @0x7f898c405b20 from @0x7f898c405ae0
    Data #3 destructed @0x7f898c405b10
    Data #2 destructed @0x7f898c405b00
    Data #1 destructed @0x7f898c405af0
    Data #0 destructed @0x7f898c405ae0
    Data #4 destructed @0x7ffee68c60c8
    Data #5 constructed @0x7ffee68c60c8
    Data #5 moved to @0x7f898c405b70 from @0x7ffee68c60c8
    Data #5 destructed @0x7ffee68c60c8
    * outer2 result.size() = 6

    Data #5 destructed @0x7f898c405b70
    Data #4 destructed @0x7f898c405b60
    Data #3 destructed @0x7f898c405b50
    Data #2 destructed @0x7f898c405b40
    Data #1 destructed @0x7f898c405b30
    Data #0 destructed @0x7f898c405b20

Style 3: Use a Class for Both Return and Output Argument
++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The third style uses a class so that it support both vector returning and
output argument for the vector.  The class is:

.. code-block:: cpp
  :linenos:

  struct Accumulator
  {

  public:
      // This can be called if consumers want the sub-operation one by one, and
      // make the code more testable. But it isn't really used in the example.
      std::vector<Data> inner1(size_t start, size_t len)
      {
          std::cout << "** Accumulator::inner1 begins with " << start << std::endl;
          std::vector<Data> ret;
          ret.reserve(len);
          inner2(start, len, ret);
          return ret;
      }

  private:
      void inner2(size_t start, size_t len, std::vector<Data> & ret)
      {
          std::cout << "** Accumulator::inner2 begins with " << start << std::endl;
          for (size_t it=0; it < len; ++it)
          {
              Data data(start+it);
              ret.emplace_back(std::move(data));
          }
      }

  public:
      // This is used when batch operation is in demand.
      void outer(size_t len)
      {
          std::cout << "* Accumulator::outer begins" << std::endl;
          result.reserve(len*(len+1)/2);
          for (size_t it=0; it < len; ++it)
          {
              std::cout << std::endl;
              std::cout << "* Accumulator::outer loop it=" << it << " begins" << std::endl;
              inner2(result.size(), it+1, result);
          }
          std::cout << "* Accumulator::outer result.size() = " << result.size() << std::endl << std::endl;
      }

  public:
      std::vector<Data> result;

  }; /* end struct Accumulator */

Although ``Accumulator::outer`` still calls the function
``Accumulator::inner2`` that takes an output argument, we also have the
function ``Accumulator::inner1`` that wraps around ``Accumulator::inner2`` and
make it testable.

To further reduce unwanted movements, we pre-calculate the number of elements
to be populated in the vector and reserve the space.

.. admonition:: Execution Results

  :download:`code/03_elision/03_accumulate.cpp`

  .. code-block:: console
    :caption: Build ``03_accumulate.cpp`` with `-DMOVENOEXCEPT -DOTYPE=3`

    $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DMOVENOEXCEPT -DOTYPE=3

  .. code-block:: console
    :caption: Run ``03_accumulate``
    :linenos:

    $ ./03_accumulate
    * Accumulator::outer begins

    * Accumulator::outer loop it=0 begins
    ** Accumulator::inner2 begins with 0
    Data #0 constructed @0x7ffee47640a8
    Data #0 moved to @0x7fdb66c05ab0 from @0x7ffee47640a8
    Data #0 destructed @0x7ffee47640a8

    * Accumulator::outer loop it=1 begins
    ** Accumulator::inner2 begins with 1
    Data #1 constructed @0x7ffee47640a8
    Data #1 moved to @0x7fdb66c05ac0 from @0x7ffee47640a8
    Data #1 destructed @0x7ffee47640a8
    Data #2 constructed @0x7ffee47640a8
    Data #2 moved to @0x7fdb66c05ad0 from @0x7ffee47640a8
    Data #2 destructed @0x7ffee47640a8

    * Accumulator::outer loop it=2 begins
    ** Accumulator::inner2 begins with 3
    Data #3 constructed @0x7ffee47640a8
    Data #3 moved to @0x7fdb66c05ae0 from @0x7ffee47640a8
    Data #3 destructed @0x7ffee47640a8
    Data #4 constructed @0x7ffee47640a8
    Data #4 moved to @0x7fdb66c05af0 from @0x7ffee47640a8
    Data #4 destructed @0x7ffee47640a8
    Data #5 constructed @0x7ffee47640a8
    Data #5 moved to @0x7fdb66c05b00 from @0x7ffee47640a8
    Data #5 destructed @0x7ffee47640a8
    * Accumulator::outer result.size() = 6

    Data #5 destructed @0x7fdb66c05b00
    Data #4 destructed @0x7fdb66c05af0
    Data #3 destructed @0x7fdb66c05ae0
    Data #2 destructed @0x7fdb66c05ad0
    Data #1 destructed @0x7fdb66c05ac0
    Data #0 destructed @0x7fdb66c05ab0

Evolution of the three styles demonstrate how one may develop sophisticated
code from a standalone helper to an optimized class library.

Variadic Template
=================

Variadic template allows us to capture any number of template arguments in a
function template.  Assuming we have 2 constructors for ``Data``:

.. code-block:: cpp
  :linenos:

  Data(size_t serial, ctor_passkey const &)
    : m_serial(serial)
  {
      m_buffer = new int[NELEM];
      initialize(0);
      std::cout << "Data #" << m_serial << " constructed @" << this
                << "(serial=" << m_serial << ")" << std::endl;
  }

  Data(size_t serial, int base, ctor_passkey const &)
    : m_serial(serial+base)
  {
      m_buffer = new int[NELEM];
      initialize(0);
      std::cout << "Data #" << m_serial << " constructed @" << this
                << "(serial=" << m_serial << ")"
                << "(base=" << base << ")" << std::endl;
  }

We will need two factories methods for them:

.. code-block:: cpp
  :linenos:

  static std::shared_ptr<Data> create(size_t serial)
  {
      return std::make_shared<Data>(serial, ctor_passkey());
  }

  static std::shared_ptr<Data> create(size_t serial, int base)
  {
      return std::make_shared<Data>(serial, int, ctor_passkey());
  }

It's tedious to add the corresponding factory functions, although it is not too
much an issue, since the compiler will complain.  Let's assume we forgot the
add the second factory overload and see what may happen.

.. admonition:: Execution Results

  :download:`code/04_template/01_factory.cpp`

  .. code-block:: console
    :caption: Build ``01_factory.cpp`` and show the missing factory method

    $ g++ 01_factory.cpp -o 01_factory -std=c++17 -g -O3 -DUSE_CREATE
    01_factory.cpp:142:37: error: too many arguments to function call, expected single argument 'serial', have 2 arguments
                data = Data::create(it, base);
                       ~~~~~~~~~~~~     ^~~~
    01_factory.cpp:22:5: note: 'create' declared here
        static std::shared_ptr<Data> create(size_t serial)
        ^
    1 error generated.

Variadic template can conveniently help us summarize the two overloads into one
template function, and also capture every new public constructor that will be
added in the future.

.. code-block:: cpp
  :linenos:

  template < typename ... Args >
  static std::shared_ptr<Data> make(Args && ... args)
  {
      // Forget about the 'forward' for now. It will be discussed later.
      return std::make_shared<Data>(std::forward<Args>(args) ..., ctor_passkey());
  }

Run the following code:

.. code-block:: cpp
  :linenos:

  void outer1(size_t len)
  {
      std::cout << "* outer1 begins" << std::endl;
      std::vector<std::shared_ptr<Data>> vec;
      for (size_t it=0; it < len; ++it)
      {
          std::cout << std::endl;
          std::cout << "* outer1 loop it=" << it << " begins" << std::endl;
          std::vector<std::shared_ptr<Data>> subvec = inner1(vec.size(), it+1);
          std::cout << "* outer1 obtained inner1 at " << vec.size() << std::endl;
          vec.insert(
              vec.end()
            , std::make_move_iterator(subvec.begin())
            , std::make_move_iterator(subvec.end())
          );
          std::cout << "* outer1 inserted subvec.size()=" << subvec.size() << std::endl;
      }
      std::cout << "* outer1 result.size() = " << vec.size() << std::endl << std::endl;

      std::cout << "* outer1 end" << std::endl << std::endl;
  }

  std::vector<std::shared_ptr<Data>> inner1(size_t base, size_t len)
  {
      std::cout << "** inner1 begins with " << base << std::endl;
      std::vector<std::shared_ptr<Data>> ret;
      for (size_t it=0; it < len; ++it)
      {
          std::shared_ptr<Data> data;
          if (0 == base)
          {
              data = Data::make(it);
          }
          else
          {
              data = Data::make(it, base);
          }
          ret.emplace_back(data);
      }
      return ret;
  }

.. admonition:: Execution Results

  :download:`code/04_template/01_factory.cpp`

  .. code-block:: console
    :caption: Build ``01_factory.cpp``

    $ g++ 01_factory.cpp -o 01_factory -std=c++17 -g -O3

  .. code-block:: console
    :caption: Run ``01_factory``
    :linenos:

    $ ./01_factory
    * outer1 begins

    * outer1 loop it=0 begins
    ** inner1 begins with 0
    Data #0 constructed @0x7ff4af405ac8(serial=0)
    * outer1 obtained inner1 at 0
    * outer1 inserted subvec.size()=1

    * outer1 loop it=1 begins
    ** inner1 begins with 1
    Data #1 constructed @0x7ff4af405b28(serial=1)(base=1)
    Data #2 constructed @0x7ff4af405b68(serial=2)(base=1)
    * outer1 obtained inner1 at 1
    * outer1 inserted subvec.size()=2

    * outer1 loop it=2 begins
    ** inner1 begins with 3
    Data #3 constructed @0x7ff4af405bf8(serial=3)(base=3)
    Data #4 constructed @0x7ff4af405c38(serial=4)(base=3)
    Data #5 constructed @0x7ff4af405c78(serial=5)(base=3)
    * outer1 obtained inner1 at 3
    * outer1 inserted subvec.size()=3
    * outer1 result.size() = 6

    * outer1 end

    Data #5 destructed @0x7ff4af405c78
    Data #4 destructed @0x7ff4af405c38
    Data #3 destructed @0x7ff4af405bf8
    Data #2 destructed @0x7ff4af405b68
    Data #1 destructed @0x7ff4af405b28
    Data #0 destructed @0x7ff4af405ac8

Perfect Forwarding
==================

In the previous section we used ``std::forward``, which enables perfect
forwarding:

.. code-block:: cpp
  :linenos:

  template < typename ... Args >
  static std::shared_ptr<Data> make(Args && ... args)
  {
      return std::make_shared<Data>(std::forward<Args>(args) ..., ctor_passkey());
  }

The template forwards the reference.  Like ``std::move``, it serves as a cast
to rvalue reference.  The difference is that:

1. ``std::move`` unconditionally casts the input to rvalue reference.
2. ``std::forward`` casts to rvalue reference only when it can.

When we write ``Data &&``, it is a rvalue reference.  With ``T &&`` as a
template argument, when we write ``T &&``, it can be either lvalue or rvalue,
and is called universal reference.  The rule of thumb is that when ``T`` is a
deductible type (``auto &&`` falls into this category too), ``T &&`` is a
universal reference rather a strict rvalue reference.

So ``std::forward<Args>(args)`` preserves the type of reference of the
arguments, and the pattern is called perfect forwarding.  Because the arguments
of ``Data`` constructors were both fundamental types, it doesn't matter whether
or not we use perfect forwarding.  To demonstrate how it works, we add the two
wrapper:

.. code-block:: cpp
  :linenos:

  // Proxy to copy and move constructor.
  Data(Data const &  other, ctor_passkey const &) : Data(std::forward<Data const &>(other)) {}
  Data(Data       && other, ctor_passkey const &) : Data(std::forward<Data &&>(other)) {}

And we use a slightly different ``outer``:

.. code-block:: cpp
  :linenos:

  void outer1(size_t len)
  {
      std::cout << "* outer1 begins" << std::endl;
      std::vector<std::shared_ptr<Data>> vec;
      for (size_t it=0; it < len; ++it)
      {
          std::cout << std::endl;
          std::cout << "* outer1 loop it=" << it << " begins" << std::endl;
          std::vector<std::shared_ptr<Data>> subvec = inner1(vec.size(), it+1);
          std::cout << "* outer1 obtained inner1 at " << vec.size() << std::endl;
          vec.insert(
              vec.end()
            , std::make_move_iterator(subvec.begin())
            , std::make_move_iterator(subvec.end())
          );
          std::cout << "* outer1 inserted subvec.size()=" << subvec.size() << std::endl;
      }
      std::cout << "* outer1 result.size() = " << vec.size() << std::endl << std::endl;

      // Exercise the perfect forwarding.
      vec.emplace_back(Data::make(*vec[0]));
      vec.emplace_back(Data::make(std::move(*vec[1])));

      std::cout << "* outer1 end" << std::endl << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/04_template/01_factory.cpp`

  .. code-block:: console
    :caption: Build ``01_factory.cpp`` and show perfect forwarding

    $ g++ 01_factory.cpp -o 01_factory -std=c++17 -g -O3 -DSHOW_PERFECT_FORWARD

  .. code-block:: console
    :caption: Run ``01_factory``
    :linenos:

    $ ./01_factory
    * outer1 begins

    * outer1 loop it=0 begins
    ** inner1 begins with 0
    Data #0 constructed @0x7ff6abc05ac8(serial=0)
    * outer1 obtained inner1 at 0
    * outer1 inserted subvec.size()=1

    * outer1 loop it=1 begins
    ** inner1 begins with 1
    Data #1 constructed @0x7ff6abc05b28(serial=1)(base=1)
    Data #2 constructed @0x7ff6abc05b68(serial=2)(base=1)
    * outer1 obtained inner1 at 1
    * outer1 inserted subvec.size()=2

    * outer1 loop it=2 begins
    ** inner1 begins with 3
    Data #3 constructed @0x7ff6abc05bf8(serial=3)(base=3)
    Data #4 constructed @0x7ff6abc05c38(serial=4)(base=3)
    Data #5 constructed @0x7ff6abc05c78(serial=5)(base=3)
    * outer1 obtained inner1 at 3
    * outer1 inserted subvec.size()=3
    * outer1 result.size() = 6

    Data #0 copied to @0x7ff6abc05cb8 from @0x7ff6abc05ac8
    Data #1 moved to @0x7ff6abc05ba8 from @0x7ff6abc05b28
    * outer1 end

    Data #1 destructed @0x7ff6abc05ba8
    Data #0 destructed @0x7ff6abc05cb8
    Data #5 destructed @0x7ff6abc05c78
    Data #4 destructed @0x7ff6abc05c38
    Data #3 destructed @0x7ff6abc05bf8
    Data #2 destructed @0x7ff6abc05b68
    Data #1 destructed @0x7ff6abc05b28
    Data #0 destructed @0x7ff6abc05ac8

Lambda Expression
=================

C++ lambda expression enables a shorthand for anonymous function.  The syntax
(no variable is captured) is:

.. code-block:: cpp

  [] (/* arguments */) { /* body */ }

It works basically like a functor.

.. code-block:: cpp
  :linenos:

  struct Functor
  {
      bool operator()(int v)
      {
          return 0 == v % 23;
      }
  }; /* end struct Functor */

  int main(int argc, char ** argv)
  {
      std::vector<int> data(63712);
      for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

      std::cout
          << "Number divisible by 23 (count by functor): "
          << std::count_if(data.begin(), data.end(), Functor())
          << std::endl;

      std::cout
          << "Number divisible by 23 (count by lambda): "
          << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%23; })
          << std::endl;

      return 0;
  }

.. admonition:: Execution Results

  :download:`code/05_lambda/01_lambda.cpp`

  .. code-block:: console
    :caption: Build ``01_lambda.cpp`` and show perfect forwarding

    $ g++ 01_lambda.cpp -o 01_lambda -std=c++17 -g -O3

  .. code-block:: console
    :caption: Run ``01_lambda``
    :linenos:

    $ ./01_lambda
    Number divisible by 23 (count by functor): 2771
    Number divisible by 23 (count by lambda): 2771

Keep a Lambda in a Local Variable
+++++++++++++++++++++++++++++++++

Lambda is considered as anonymous function, but we can give it a "name" by
assigning it to a variable.  There are two choices: ``auto`` or
``std::function``.

.. code-block:: cpp
  :linenos:

  int main(int argc, char ** argv)
  {
      std::vector<int> data(63712);
      for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

      std::cout
          << "Number divisible by 23 (count by lambda inline): "
          << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%23; })
          << std::endl;

      auto condition = [](int v){ return 0 == v%23; };

      std::cout
          << "Number divisible by 23 (count by lambda in auto): "
          << std::count_if(data.begin(), data.end(), condition)
          << std::endl;

      std::function<bool (int)> condition_function = [](int v){ return 0 == v%23; };

      std::cout
          << "Number divisible by 23 (count by lambda in std::function): "
          << std::count_if(data.begin(), data.end(), condition_function)
          << std::endl;

      return 0;
  }

.. admonition:: Execution Results

  :download:`code/05_lambda/02_stored.cpp`

  .. code-block:: console
    :caption: Build ``02_stored.cpp``

    $ g++ 02_stored.cpp -o 02_stored -std=c++17 -g -O3

  .. code-block:: console
    :caption: Run ``02_stored``
    :linenos:

    $ ./02_stored
    Number divisible by 23 (count by lambda inline): 2771
    Number divisible by 23 (count by lambda in auto): 2771
    Number divisible by 23 (count by lambda in std::function): 2771

Difference between ``auto`` and ``std::function``
+++++++++++++++++++++++++++++++++++++++++++++++++

Although both ``auto`` and ``std::function`` can hold a lambda, the two ways
are not exactly the same.  A lambda works like a functor and the ``auto`` type
reflects that.  A ``std::function`` is more versatile than it, and takes more
memory as well.

This is a list of targets (callables) that a ``std::function`` can hold: free
functions, member functions, functors, lambda expressions, and bind
expressions.

.. code-block:: cpp
  :linenos:

  std::cout
      << std::endl
      << "The differences between lambda and std::function"
      << std::endl;
  std::cout
      << "type name of lambda: "
      << typeid(condition).name() << std::endl;
  std::cout
      << "type name of std::function: "
      << typeid(condition_function).name() << std::endl;

  std::cout
      << "size of lambda: "
      << sizeof(condition) << std::endl;
  std::cout
      << "size of std::function: "
      << sizeof(condition_function) << std::endl;

.. admonition:: Execution Results

  :download:`code/05_lambda/02_stored.cpp`

  .. code-block:: console
    :caption: Build ``02_stored.cpp`` and show the difference in types

    $ g++ 02_stored.cpp -o 02_stored -std=c++17 -g -O3 -DSHOW_DIFF

  .. code-block:: console
    :caption: Run ``02_stored``
    :linenos:

    $ ./02_stored
    Number divisible by 23 (count by lambda inline): 2771
    Number divisible by 23 (count by lambda in auto): 2771
    Number divisible by 23 (count by lambda in std::function): 2771

    The differences between lambda and std::function
    type name of lambda: Z4mainE3$_1
    type name of std::function: NSt3__18functionIFbiEEE
    size of lambda: 1
    size of std::function: 48

Closure
=======

So far our use of lambda expressions doesn't capture any local variables.  When
it does, we call the lambda expression a closure.

We must tell the compiler what type of capture the lambda expression would like
to use.  Otherwise the compilation fails.

.. code-block:: cpp
  :linenos:

  int main(int argc, char ** argv)
  {
      std::vector<int> data(63712);
      for (size_t i=0 ; i<data.size(); ++i) { data[i] = i;}

      int divisor = 23;

  #if WRONG_CAPTURE
      std::cout
          << "Count (wrong capture): "
          << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%divisor; })
          << " (divisor: " << divisor << ")"
          << std::endl;
  #endif

      return 0;
  }

.. admonition:: Execution Results

  :download:`code/05_lambda/03_closure.cpp`

  .. code-block:: console
    :caption: Show compiler error of wrong capture in ``03_closure.cpp``

    $ g++ 03_closure.cpp -o 03_closure -std=c++17 -g -O3 -DWRONG_CAPTURE
    03_closure.cpp:16:77: error: variable 'divisor' cannot be implicitly captured in a lambda with no capture-default specified
            << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%divisor; })
                                                                                ^
    03_closure.cpp:11:9: note: 'divisor' declared here
        int divisor = 23;
            ^
    03_closure.cpp:16:52: note: lambda expression begins here
            << std::count_if(data.begin(), data.end(), [](int v){ return 0 == v%divisor; })
                                                       ^
    1 error generated.

We may explicitly tell the compiler that we want `divisor` to be captured by the lambda expression by value:

.. code-block:: cpp

  int divisor = 23;

  std::cout
      << "Count (lambda explicitly capture by value): "
      << std::count_if(data.begin(), data.end(), [divisor](int v){ return 0 == v%divisor; })
      << " (divisor: " << divisor << ")"
      << std::endl;

Use ``=`` to implicitly capture by value:

.. code-block:: cpp

  std::cout
      << "Count (lambda implicitly capture by value): "
      << std::count_if(data.begin(), data.end(), [=](int v){ return 0 == v%divisor; })
      << " (divisor: " << divisor << ")"
      << std::endl;

Use ``&`` to capture by reference:

.. code-block:: cpp
  :linenos:

  std::cout
      << "Count (lambda explicitly capture by reference): "
      << std::count_if(data.begin(), data.end(), [&divisor](int v){ divisor = 10; return 0 == v%divisor; })
      << " (divisor: " << divisor << ")"
      << std::endl;

``&`` can also be put standalone in ``[]`` to indicate that the default capture is
by reference.

The execution results:

.. admonition:: Execution Results

  :download:`code/05_lambda/03_closure.cpp`

  .. code-block:: console
    :caption: Build ``03_closure.cpp``

    $ g++ 03_closure.cpp -o 03_closure -std=c++17 -g -O3

  .. code-block:: console
    :caption: Run ``03_closure``
    :linenos:

    $ ./03_closure
    Count (lambda explicitly capture by value): 2771 (divisor: 23)
    Count (lambda implicitly capture by value): 2771 (divisor: 23)
    Count (lambda explicitly capture by reference): 6372 (divisor: 10)

Comments on Functional Style
++++++++++++++++++++++++++++

The lambda expression and closure allow functional style of programming.  As
shown in the ``std::count_if`` example, it is a convenient tool to reduce the
lines of code.  It generally makes the code looks cleaner and easier to
maintain.  That buys us time to do more important things or optimize
performance hotspot.

But there are times that we cannot entrust the optimization to the compiler.
Lambda expressions are not the easiest place to add intrinsics or assemblies.

When working on a container object equipped with proper iterator interface, I
go with the functional style.  The lambda expression may help avoid expensive
intermediate buffers.  It works well at least for the initial prototype.

Exercises
=========

1. Measure the performance between using an output vector and returning a new
   vector.

References
==========

.. [1] `Copy elision
  <https://en.cppreference.com/w/cpp/language/copy_elision>`__ at
  cppreference.com .

.. [2] `Lambda expressions
  <https://en.cppreference.com/w/cpp/language/lambda>`__ at cppreference.com .

.. [3] `C++ Lambdas Under The Hood
  <https://web.mst.edu/~nmjxv3/articles/lambdas.html>`__

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
