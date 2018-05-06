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

All together
------------

.. code-block:: cpp
  :linenos:

  #include <memory>
  #include <iostream>

  struct Data {
    static size_t count;
    Data() { count++; }
    ~Data() { count--; }
  };
  size_t Data::count = 0;

  struct Worker {
    std::shared_ptr<Data> data;
    void work () {
      std::cout << "in Worker::work(): data.use_count() = " << data.use_count()
                << ", Data::count = " << Data::count << std::endl;
    }
  };

  std::shared_ptr<Data> new_shared_ptr() {
    std::shared_ptr<Data> data(new Data);
    std::cout << "in new_shared_ptr(): data.use_count() = " << data.use_count()
              << ", Data::count = " << Data::count << std::endl;
    return data;
  }

  void copy_shared_ptr(std::shared_ptr<Data> data) {
    std::cout << "in copy_shared_ptr(): data.use_count() = "
              << data.use_count()
              << ", Data::count = " << Data::count << std::endl;
  }

  void reference_shared_ptr(std::shared_ptr<Data> const & data) {
    std::cout << "in reference_shared_ptr(): data.use_count() = "
              << data.use_count()
              << ", Data::count = " << Data::count << std::endl;
  }

  int main(int argc, char ** argv) {
    auto data = new_shared_ptr();
    copy_shared_ptr(data);
    reference_shared_ptr(data);
    Worker worker = {data};
    worker.work();
    data = nullptr;
    std::cout << "after reset the local shared_ptr to null, "
              << "the shared_ptr::use_count drops to 0" << std::endl;
    reference_shared_ptr(data);
    std::cout << "the shared_ptr in worker isn't affected"
              << std::endl;
    reference_shared_ptr(worker.data);
    return 0;
  }

The output:

.. code-block:: none
  :linenos:

  in new_shared_ptr(): data.use_count() = 1, Data::count = 1
  in copy_shared_ptr(): data.use_count() = 2, Data::count = 1
  in reference_shared_ptr(): data.use_count() = 1, Data::count = 1
  in Worker::work(): data.use_count() = 2, Data::count = 1
  after reset the local shared_ptr to null, the shared_ptr::use_count drops to 0
  in reference_shared_ptr(): data.use_count() = 0, Data::count = 1
  the shared_ptr in worker isn't affected
  in reference_shared_ptr(): data.use_count() = 1, Data::count = 1

Move Semantics
==============

Lambda
======

Variadic Template
=================
