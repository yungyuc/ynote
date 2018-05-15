================
C++11 for Python
================

:date: draft 2018/5/7

C++11 is a giant stride in the C++ history.  After 8 years of work, lots of `new
features <https://en.cppreference.com/w/cpp/language/history>`_ are added to the
language.  Most if not all have been longed for years.  They give the language a
new life.

But even if C++11 improves a lot, it's still C++.  In many cases the complex
language is an overkill.  That's why wrappers are commonplace: keep the
cycle-sensitive core in C++, and the rest in a higher-level language, like
Python.  The new standard introduces many new features, but I found the four
things are particularly useful in a C++-Python hybrid environment: smart
pointers, move semantics, lambda, and variadic template.

Smart Pointers
==============

Smart pointers are the key to make C++ friendly to Python.  Specifically,
:cpp:class:`std::shared_ptr` provides a compatibility layer to the resource
management model in Python.  In Python we use the reference count in
:c:type:`PyObject`, and C++ shared pointers work in the same way, but provides
much more features.  Therefore, although there are other smart pointers, what we
care is actually only shared pointers.  And we don't need to code up the
interfacing ourselves.  Boost.python and pybind11 do that for us.

Automatic destruction
---------------------

A shared pointer, except its construction and destruction, works similar to a
raw (standard) pointer.  See an example:

.. code-block:: cpp
  :linenos:

  #include <memory>
  #include <vector>
  #include <algorithm>
  #include <iostream>

  class Series {
    std::vector<int> m_data;
  public:
    int sum() const {
      const int ret = std::accumulate(m_data.begin(), m_data.end(), 0);
      std::cout << "Series::sum() = " << ret << std::endl;
      return ret;
    }
    static size_t count;
    Series(size_t size, int lead) : m_data(size) {
      for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
      count++;
    }
    ~Series() { count--; }
  };
  size_t Series::count = 0;

  void use_raw_pointer() {
    Series * series_ptr = new Series(10, 2);
    series_ptr->sum(); // call member function
    // OUT: Series::sum() = 65
    // remember to delete the object or we leak memory
    std::cout << "before explicit deletion, Series::count = "
              << Series::count << std::endl;
    // OUT: before explicit deletion, Series::count = 1
    delete series_ptr;
    std::cout << "after the resource is manually freed, Series::count = "
              << Series::count << std::endl;
    // OUT: after the resource is manually freed, Series::count = 0
  }

  void use_shared_pointer() {
    std::shared_ptr<Series> series_sptr(new Series(10, 3));
    series_sptr->sum(); // call member function
    // OUT: Series::sum() = 75
    // note shared_ptr handles deletion for series_sptr
  }

  int main(int argc, char ** argv) {
    // the common raw pointer
    use_raw_pointer();
    // now, shared_ptr
    use_shared_pointer();
    std::cout << "no memory leak: Series::count = "
              << Series::count << std::endl;
    // OUT: no memory leak: Series::count = 0
    return 0;
  }
.. sptr1.cpp

Let's start with the function :cpp:func:`use_raw_pointer`, showing how we use a
raw pointer.  First, allocate and initiate the object:

.. code-block:: cpp

  Series * series_ptr = new Series(10, 2);

Second, do what we want with the object:

.. code-block:: cpp

  series_ptr->sum(); // call member function
  // OUT: Series::sum() = 65

When finishing using the object, we need to free the resource.  A raw pointer
needs manual a treatment:

.. code-block:: cpp

  // remember to delete the object or we leak memory
  std::cout << "before explicit deletion, Series::count = "
            << Series::count << std::endl;
  // OUT: before explicit deletion, Series::count = 1
  delete series_ptr;
  std::cout << "after the resource is manually freed, Series::count = "
            << Series::count << std::endl;
  // OUT: after the resource is manually freed, Series::count = 0

If we don't delete it, after leaving the function we will neven access it
again.  Only OS can reclaim it when the program finishes.

A shared pointer can do this for us.  In the function
:cpp:func:`use_shared_pointer`, we don't manually delete the object:

.. code-block:: cpp

  std::shared_ptr<Series> series_sptr(new Series(10, 3));
  series_sptr->sum(); // call member function
  // OUT: Series::sum() = 75
  // note shared_ptr handles deletion for series_sptr

:cpp:class:`std::shared_ptr` deletes the object when it goes out of the scope.
Outside :cpp:func:`use_shared_pointer`, we see the object is deleted:

.. code-block:: cpp

  use_shared_pointer();
  std::cout << "no memory leak: Series::count = "
            << Series::count << std::endl;
  // OUT: no memory leak: Series::count = 0
  return 0;

Resource ownership
------------------

A shared pointer is capable of automatic deletion of unused objects because it
tracks shared ownership.  The last owner of the pointer is responsible for
freeing the object.  Reference counting is a common technique to implement it.

The use of ownership wasn't obvious in the previous example, since the shared
pointer was used locally.  Let's see another example that returns a shared
pointer:

.. code-block:: cpp
  :linenos:

  #include <memory>
  #include <vector>
  #include <algorithm>
  #include <iostream>

  class Series {
    std::vector<int> m_data;
  public:
    int sum() const {
      const int ret = std::accumulate(m_data.begin(), m_data.end(), 0);
      std::cout << "Series::sum() = " << ret << std::endl;
      return ret;
    }
    static size_t count;
    Series(size_t size, int lead) : m_data(size) {
      for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
      count++;
    }
    ~Series() { count--; }
  };
  size_t Series::count = 0;

  std::shared_ptr<Series> make_shared_pointer(size_t size, int lead) {
    return std::shared_ptr<Series>(new Series(size, lead));
  }

  void use_shared_pointer(const std::shared_ptr<Series> & series_sptr) {
    series_sptr->sum(); // call member function
    // OUT: Series::sum() = 65
  }

  int main(int argc, char ** argv) {
    // create a shared pointer
    auto series_sptr = make_shared_pointer(10, 2);
    // now, shared_ptr
    use_shared_pointer(series_sptr);
    // the object is still alive
    std::cout << "Series::count = " << Series::count << std::endl;
    // OUT: Series::count = 1
    // reset the pointer
    series_sptr = nullptr;
    std::cout << "no memory leak: Series::count = "
              << Series::count << std::endl;
    // OUT: no memory leak: Series::count = 0
    return 0;
  }
.. sptr2.cpp

In the function :cpp:func:`make_shared_pointer`, we create a shared pointer and
return it:

.. code-block:: cpp

  return std::shared_ptr<Series>(new Series(size, lead));

In :cpp:func:`use_shared_pointer`, we take and use it:

.. code-block:: cpp

  void use_shared_pointer(const std::shared_ptr<Series> & series_sptr) {
    series_sptr->sum(); // call member function
    // OUT: Series::sum() = 65
  }

But this time, the object isn't destructed at the end of the function.  Outside
the function we still see the object alive, because in :cpp:func:`main` we
still own the shared pointer:

.. code-block:: cpp

  // now, shared_ptr
  use_shared_pointer(series_sptr);
  // the object is still alive
  std::cout << "Series::count = " << Series::count << std::endl;
  // OUT: Series::count = 1

The object gets deleted when we say it's not used anymore, by setting the
shared pointer to null:

.. code-block:: cpp

  // reset the pointer
  series_sptr = nullptr;
  std::cout << "no memory leak: Series::count = "
            << Series::count << std::endl;
  // OUT: no memory leak: Series::count = 0

This starts to show the power of a shared pointer.  It frees programmers from
the tedious book-keeping for pointers.  A shared pointer clearly defines when
an object should be destructed, and does it automatically.  The interface is a
drop-in replacement of a raw pointer.  Thus, when resources need to be shared,
we usually think of a shared pointer.

But keep in mind that the convenience comes with costs, although we aren't
discussing it here.

Enable from this
----------------

Pointers are used both from outside and inside of a class.  But when we want to
use a shared pointer form inside the object the pointer points to, can we just
create a new :cpp:class:`std::shared_ptr`?

.. code-block:: cpp

  class Series {
  //...
    std::shared_ptr<Series> get_this_bad() {
      return std::shared_ptr<Series>(this);
    }
  //...
  };

No!  When you create the bad shared pointer, it looks fine.  But after it is
destructed, you will get double free:

.. code-block:: cpp

  std::shared_ptr<Series> sp1(new Series(10, 2));
  assert(sp1.use_count() == 1)
  auto sp2 = sp1->get_this_bad();
  assert(sp2.use_count() == 1) // this isn't 2 and is wrong
  sp2 = nullptr;
  assert(sp1->count == 0) // uhoh, Series object is destructed
  sp1 = nullptr; // double free!  This gets you segfault if you are lucky

We need :cpp:class:`std::enable_shared_from_this` and the helper function
:cpp:func:`shared_from_this` it provides.  To use a shared pointer from inside
the object it points to, the class needs to be derived from
:cpp:class:`std::enable_shared_from_this`.  Note it's a class template and you
should provide the derived class as the template argument:

.. code-block:: cpp
  :linenos:

  #include <memory>
  #include <vector>
  #include <algorithm>
  #include <iostream>

  class Series : public std::enable_shared_from_this<Series> {
    std::vector<int> m_data;
  public:
    int sum() const {
      const int ret = std::accumulate(m_data.begin(), m_data.end(), 0);
      std::cout << "Series::sum() = " << ret << std::endl;
      return ret;
    }
    static size_t count;
    Series(size_t size, int lead) : m_data(size) {
      for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
      count++;
    }
    ~Series() { count--; }
    std::shared_ptr<Series> get_this_bad() {
      // this will result in double free! don't do this
      return std::shared_ptr<Series>(this);
    }
    std::shared_ptr<Series> get_this_good() {
      return shared_from_this(); // by std::enable_shared_from_this
    }
  };
  size_t Series::count = 0;

  std::shared_ptr<Series> make_shared_pointer(size_t size, int lead) {
    return std::shared_ptr<Series>(new Series(size, lead));
  }

  int main(int argc, char ** argv) {
    // create a shared pointer
    auto sp1 = make_shared_pointer(10, 2);
    std::cout << "sp1.use_count() = "
              << sp1.use_count() << std::endl;
    // OUT: sp1.use_count() = 1
    // recreate a new shared pointer from the existing one
    auto sp2 = sp1->get_this_good();
    std::cout << "sp2.use_count() = "
              << sp2.use_count() << std::endl;
    // OUT: sp1.use_count() = 2
    return 0;
  }

By using :cpp:func:`shared_from_this`, we get a correct reference count from
inside the class.

Ensure to Share
---------------

The risk of double free doesn't only appear when one creates a shared pointer
from within the object.  It's easy to make a similar mistake when one first
uses a shared pointer:

.. code-block:: cpp

  Series * p1 = new Series(3, 7);
  std::shared_ptr<Series> sp1(p1);
  // wrong! Double free when both pointers are destructed
  std::shared_ptr<Series> sp2(p1);

To prevent this mistake, we can hide the constructor of :cpp:class:`Series`, so
that no one can get a raw pointer from a newly constructed object:

.. code-block:: cpp
  :linenos:

  #include <memory>
  #include <vector>
  #include <algorithm>
  #include <iostream>

  class Series : public std::enable_shared_from_this<Series> {
    std::vector<int> m_data;
    // private constructor
    Series(size_t size, int lead) : m_data(size) {
      for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
      count++;
    }
  public:
    int sum() const {
      const int ret = std::accumulate(m_data.begin(), m_data.end(), 0);
      std::cout << "Series::sum() = " << ret << std::endl;
      return ret;
    }
    static size_t count;
    ~Series() { count--; }
    // factory method to construct the object
    // and put it in the shared pointer
    static std::shared_ptr<Series> make(size_t size, int lead) {
      return std::shared_ptr<Series>(new Series(size, lead));
    }
  };
  size_t Series::count = 0;

  int main(int argc, char ** argv) {
    // create a shared pointer
    auto sp1 = Series::make(10, 2);
    std::cout << "sp1.use_count() = "
              << sp1.use_count() << std::endl;
    // OUT: sp1.use_count() = 1
    Series o2(*sp1); // uhoh, we forgot copy construction!
    o2.sum();
    // OUT: Series::sum() = 65
    return 0;
  }
.. sptr4.cpp

There are two key points.  First, we make the constructor private:

.. code-block:: cpp

  // private constructor
  Series(size_t size, int lead) : m_data(size) {
    for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
    count++;
  }

Second, we provide a static factory method to construct the object and return
the shared pointer managing it:

.. code-block:: cpp

  // factory method to construct the object
  // and put it in the shared pointer
  static std::shared_ptr<Series> make(size_t size, int lead) {
    return std::shared_ptr<Series>(new Series(size, lead));
  }

Because the class doesn't allow to be constructed from outside, the factory
method is the only way to create a new instance, and then all instances must be
managed by a shared pointer.

Hold on, didn't we miss something?  Copy constructor!

.. code-block:: cpp

  Series o2(*sp1); // uhoh, we forgot copy construction!
  o2.sum();
  // OUT: Series::sum() = 65

Let's say we don't want the object to be copyable.  For a resource object
holding a lot of memory, it's not uncommon.  Instead of allowing the object to
be copied, it is foreced to use the idiom of transfer ownership.

.. code-block:: cpp

  // no copy, no move
  Series(Series const & ) = delete;
  Series(Series       &&) = delete;
  Series & operator=(Series const & ) = delete;
  Series & operator=(Series       &&) = delete;

That's it.  We have a class totally managed by a shared pointer.  I probably
can add one more comment about performace.  The reference count of share
pointers requires atomic operation, and it's not free.  The cost is especially
significant when multiple threads are in use.  Put synchronization aside, the
reference couter needs to be dynamically allocated.  The pointed instance
itself needs to be on the heap as well.  Then there are two allocations.  This
is why shared pointers are a performance killer for small objects.  But even
for large objects, we hope to reduce the allocation overhead.

:cpp:func:`std::make_shared` can help.  It only make allocation once for both
the class and the reference counter.  The use is simple:

.. code-block:: cpp

  // factory method to construct the object
  // and put it in the shared pointer
  static std::shared_ptr<Series> make(size_t size, int lead) {
    return std::make_shared<Series>(size, lead, ctor_passkey());
  }

What is that :cpp:class:`ctor_passkey`?  It's there because
:cpp:func:`std::make_shared` cannot work with a private constructor!  But we
want no one but the class itself to access the constructor.  That
:cpp:class:`ctor_passkey` is the solution:

.. code-block:: cpp

  private:
    struct ctor_passkey {};
  public:
    Series(size_t size, int lead, ctor_passkey const &) : m_data(size) {
      for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
      count++;
    }
    static std::shared_ptr<Series> make(size_t size, int lead) {
      return std::make_shared<Series>(size, lead, ctor_passkey());
    }

Since :cpp:class:`ctor_passkey` can only be used inside the class, no one from
outside can call the constructor.  Our system isn't compromised.  (And without
additional overhead.  The compiler optimizes away the :cpp:class:`ctor_passkey`
object since it's not used at all.)

Move Semantics
==============

High-performance number-crunching code needs large arrays as memory buffers.
When using large arrays, we don't want to copy them frequently.  For example,
it's challenging to fit a :math:`50,000 \times 50,000` double-precision dense
matrix into memory, not to say copy it.

Before C++11, there are some cases that unnecessary copy isn't avoidable:

.. code-block:: cpp
  :linenos:

  #include <algorithm>
  #include <cstdio>

  struct Storage {
    int * data = nullptr;
    size_t size = 0;
    size_t reserved = 0;
    Storage() = delete;
    Storage(size_t size, size_t reserved)
    : data(new int[reserved]), size(size), reserved(reserved) {
      std::printf("Storage(this=%p)::Storage(size_t, size_t)\n", this);
    }
    Storage(Storage const & other)
    : data(new int[other.reserved])
    , size(other.size), reserved(other.reserved) {
      // this takes time and memory
      std::printf("Storage(this=%p)::Storage(const &): costly copy\n", this);
      std::copy(other.data, other.data+other.size, data);
    }
    ~Storage() {
      std::printf("Storage(this=%p)::~Storage()\n", this);
      if (data) { delete[] data; }
    }
    void append(int v) {
      if (reserved <= size) {
        std::printf("Storage(this=%p)::append(int) add more space\n", this);
        reserved = (size+1)*2;
        int * newdata = new int[reserved];
        std::copy(data, data+size, newdata);
        std::swap(data, newdata);
        delete[] newdata;
      }
      data[size] = v;
      size++;
    }
  };

  Storage extend(Storage stor) {
    std::printf("extend() &stor=%p\n", &stor);
    stor.append(stor.data[stor.size-1]);
    std::printf("extend() finish\n");
    return stor;
  }

  int main(int argc, char ** argv) {
    std::printf("Normal construction:\n");
    Storage stor1(2, 4);
    std::printf("stor1  (this=%p).size = %ld\n", &stor1, stor1.size);
    // OUT: Storage(this=0x7ffc355e3540)::Storage(size_t, size_t)
    // OUT: stor1  (this=0x7ffc355e3540).size = 2

    std::printf("Prepare to extend stor1:\n");
    Storage stor2 = extend(stor1);
    // OUT: Storage(this=0x7ffc355e3580)::Storage(const &): costly copy
    // OUT: extend() &stor=0x7ffc355e3580
    // OUT: extend() finish
    // OUT: Storage(this=0x7ffc355e3560)::Storage(const &): costly copy
    // OUT: Storage(this=0x7ffc355e3580)::~Storage()
    std::printf("stor2  (this=%p).size = %ld\n", &stor2, stor2.size);
    // OUT: stor2  (this=0x7ffc355e3560).size = 3
    // OUT: Storage(this=0x7ffc355e3560)::~Storage()
    // OUT: Storage(this=0x7ffc355e3540)::~Storage()

    return 0;
  }

The :cpp:func:`extend` function that we want to use results into two
:cpp:class:`Storage` instances, although only the first is necessary.  It
copies from ``stor1`` so that we aren't changing it:

.. code-block:: cpp

  Storage stor2 = extend(stor1);
  // OUT: Storage(this=0x7ffc355e3580)::Storage(const &): costly copy

But upon :cpp:func:`extend` returning, the second copy is perform to prepare
a temporary object for returning:

.. code-block:: cpp
  :emphasize-lines: 3

  // OUT: extend() &stor=0x7ffc355e3580
  // OUT: extend() finish
  // OUT: Storage(this=0x7ffc355e3560)::Storage(const &): costly copy
  // OUT: Storage(this=0x7ffc355e3580)::~Storage()

The return value is not eligible for copy elision (return value optimization,
RVO), because it is not a local variable, but from the function's argument.
Compiler must call a constructor for it, although it's nothing more than a
temporary variable whose resources can be moved to whoever needs it at the
caller.

The C++11 move semantics provides a solution.  By adding a *move constructor*
(a constructor that takes a rvalue reference), compiler knows how to treat the
return value more efficiently:

.. code-block:: cpp

  Storage(Storage && other)
  : data(other.data), size(other.size), reserved(other.reserved) {
    // this is much faster
    std::printf("Storage(this=%p)::Storage(&&): cheaper move\n", this);
    other.size = 0;
    other.reserved = 0;
    other.data = nullptr;
  }

For the argument of :cpp:func:`extend`, the copy constructor is still used to
copy the data to a new instance.  But when the function returns, the move
constructor is used to move the data from the copy-constructed temporary to the
``stor2`` variable at the caller.

.. code-block:: cpp
  :emphasize-lines: 5

  Storage stor2 = extend(stor1);
  // OUT: Storage(this=0x7ffe2ead4de0)::Storage(const &): costly copy
  // OUT: extend() &stor=0x7ffe2ead4de0
  // OUT: extend() finish
  // OUT: Storage(this=0x7ffe2ead4dc0)::Storage(&&): cheaper move
  // OUT: Storage(this=0x7ffe2ead4de0)::~Storage()
  std::printf("stor2  (this=%p).size = %ld\n", &stor2, stor2.size);
  // OUT: stor2  (this=0x7ffe2ead4dc0).size() = 3
  // OUT: Storage(this=0x7ffe2ead4dc0)::~Storage()
  // OUT: Storage(this=0x7ffe2ead4da0)::~Storage()

In the above example, it could be confusing why compiler knows the move
constructor should be used.  The standard requires a compiler to either elide
the return copy or treat the return instance as a rvalue when returning a local
variable.  Because :cpp:func:`extend` returns its function argument, RVO
doesn't engage.  The function then effectly works like:

.. code-block:: cpp
  :emphasize-lines: 5

  Storage extend(Storage stor) {
    std::printf("extend() &stor=%p\n", &stor);
    stor.append(stor.data[stor.size-1]);
    std::printf("extend() finish\n");
    return std::move(stor);
  }

Thus when a move constructor of :cpp:class:`Storage` is available, it is called
for the return value.

Lambda
======

Variadic Template
=================
