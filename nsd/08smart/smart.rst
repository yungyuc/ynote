============================
Ownership and Smart Pointers
============================

.. contents:: Contents in the chapter
  :local:
  :depth: 1

Pointer and Reference
=====================

.. contents:: Contents in the section
  :local:
  :depth: 1

Raw Pointer
+++++++++++

The raw pointer allows us to directly manipulate the memory.  When using a
pointer, if it not pointing to an existing object, it is a good practice to
point it to ``nullptr``.

.. code-block:: cpp

  std::cout << "PlainData pointer initialized : ";
  // It is a good practice to initialize a raw pointer to nullptr.
  PlainData * ptr = nullptr;
  // Although nullptr will be integer 0, do not use the integer literal 0 or
  // the infamous macro NULL to represent nullity.
  put_ptr(std::cout, ptr) << std::endl;

``nullptr`` is the null address, which is generally just ``0x0``:

.. code-block:: none

  PlainData pointer initialized : 0x0000000000000000

The full example code can be found in :ref:`01_raw_pointer.cpp
<nsd-smart-example-raw>`.

Type of Null Pointer Literal
++++++++++++++++++++++++++++

In older C, there is a convention to use the macro ``NULL`` to for the null
address (``0x0``).  ``NULL`` should not be used with C++11 and beyond.  The
macro is merely a number and does not provide correct type information:

.. code-block:: cpp

  // The reason to not use 0 or NULL for the null pointer: they are not even
  // of a pointer type!
  static_assert(!std::is_pointer<decltype(0)>::value, "error");
  static_assert(!std::is_pointer<decltype(NULL)>::value, "error");
  // 0 is int
  static_assert(std::is_same<decltype(0), int>::value, "error");
  // int cannot be converted to a pointer.
  static_assert(!std::is_convertible<decltype(0), void *>::value, "error");
  // NULL is long
  static_assert(std::is_same<decltype(NULL), long>::value, "error");
  // long cannot be converted to a pointer, either.
  static_assert(!std::is_convertible<decltype(NULL), void *>::value, "error");

C++11 defines the new ``nullptr`` `pointer literal
<https://en.cppreference.com/w/cpp/language/nullptr>`__ to represent the null
pointer with type information (``std::nullptr_t``).  The compiler may use the
type information to perform more rigorous checks:

.. code-block:: cpp

  // Although nullptr is of type std::nullptr_t, not exactly a pointer ...
  static_assert(std::is_same<decltype(nullptr), std::nullptr_t>::value, "error");
  static_assert(!std::is_pointer<decltype(nullptr)>::value, "error");
  // It can be converted to a pointer.
  static_assert(std::is_convertible<decltype(nullptr), void *>::value, "error");
  static_assert(std::is_convertible<decltype(nullptr), PlainData *>::value, "error");

Dynamic Allocation
++++++++++++++++++

This is how to use a pointer to point to a memory block allocated for a class
using ``malloc()``:

.. code-block:: cpp

  // Allocate memory for PlainData and get the returned pointer.
  std::cout << "PlainData pointer after malloc: ";
  ptr = static_cast<PlainData *>(malloc(sizeof(PlainData)));
  put_ptr(std::cout, ptr) << std::endl;

The address of the allocated memory is stored in the pointer variable:

.. code-block:: none

  PlainData pointer after malloc: 0x00007fdd5e809800

Freeing the memory block takes the pointer:

.. code-block:: cpp

  // After free the memory, the pointer auto variable is not changed.
  std::cout << "PlainData pointer after free  : ";
  free(ptr);
  put_ptr(std::cout, ptr) << std::endl;

Freeing does not touch the pointer variable:

.. code-block:: none

  PlainData pointer after free  : 0x00007fdd5e809800

Use ``new`` for the allocation:

.. code-block:: cpp

  // Use new to allocate for and construct PlainData and get the returned
  // pointer.
  std::cout << "PlainData pointer after new   : ";
  ptr = new PlainData();
  put_ptr(std::cout, ptr) << std::endl;

The allocated memory happens to be the same as that returned by ``malloc()``:

.. code-block:: none

  PlainData pointer after new   : 0x00007fdd5e809800

``delete`` takes the pointer for deletion:

.. code-block:: cpp

  // After delete, the pointer auto variable is not changed, either.
  std::cout << "PlainData pointer after delete: ";
  delete ptr;
  put_ptr(std::cout, ptr) << std::endl;

``delete`` does not change the pointer variable either:

.. code-block:: none

  PlainData pointer after delete: 0x00007fdd5e809800

Reference
+++++++++

.. contents:: Contents in the section
  :local:
  :depth: 1

A reference works very similar to a pointer, but unlike a pointer, a reference
cannot be used to deallocate or destruct the object it references.  In general,
a reference is used just like an instance:

.. code-block:: cpp

  void manipulate_with_reference(PlainData & data)
  {
      std::cout << "Manipulate with reference      : ";
      put_ptr(std::cout, &data) << std::endl;

      for (size_t it=0; it < 1024*8; ++it)
      {
          data.buffer[it] = it;
      }
      // (... more meaningful work before returning.)

      // We cannot delete an object passed in with a reference.
  }

.. code-block:: none

  Manipulate with reference      : 0x00007fe94a808800

The full example code for using the reference can be found in
:ref:`02_reference.cpp <nsd-smart-example-ref>`.

RAII
++++

When using a pointer with dynamic memory, we need to make sure the life cycle
of the instance is managed correctly.  That is, to instantiate the class and
destroy the instance at proper places.  It is oftentimes cumbersome and
error-prone.

The C++ reference makes it easier when we do not need to manage the life cycle.
Because a reference cannot be used to destroy an instance, when a programmer
sees the use of a reference, it is clear that the lifecycle of the referenced
instance is not managed with the reference.  Programmers should take the
advantage to make the intention of their code clear.

But there are cases that the resource life cycle needs explicit management, and
references are not adequate.  A better way than the manual control shown above
is the technique of RAII (resource acquisition is initialization).  The basic
concept of RAII is to use the object life cycle to control the resource life
cycle.

With RAII, we can relax the treatment of always deleting the object in the same
function creating it.  RAII is directly related to the concept of ownership we
are introducing immediately.

Ownership
=========

In a complicated system, memory is not free immediately after allocation.
Consider the following example, where there are two worker functions with
different memory management behaviors.

Data Class
++++++++++

Our data object is large, and we don't want the expensive overhead from
frequent allocation and deallocation.

.. code-block:: cpp
  :linenos:

  class Data
  {

  public:

      constexpr const static size_t NELEM = 1024*8;

      using iterator = int *;
      using const_iterator = const int *;

      Data()
      {
          std::fill(begin(), end(), 0);
          std::cout << "Data @" << this << " is constructed" << std::endl;
      }

      ~Data()
      {
          std::cout << "Data @" << this << " is destructed" << std::endl;
      }

      const_iterator cbegin() const { return m_buffer; }
      const_iterator cend() const { return m_buffer+NELEM; }
      iterator begin() { return m_buffer; }
      iterator end() { return m_buffer+NELEM; }

      size_t size() const { return NELEM; }
      int   operator[](size_t it) const { return m_buffer[it]; }
      int & operator[](size_t it)       { return m_buffer[it]; }

      bool is_manipulated() const
      {
          for (size_t it=0; it < size(); ++it)
          {
              const int v = it;
              if ((*this)[it] != v) { return false; }
          }
          return true;
      }

  private:

      // A lot of data that we don't want to reconstruct.
      int m_buffer[NELEM];

  }; /* end class Data */

  void manipulate_with_reference(Data & data)
  {
      std::cout << "Manipulate with reference: " << &data << std::endl;

      for (size_t it=0; it < data.size(); ++it)
      {
          data[it] = it;
      }
      // In a real consumer function we will do much more meaningful operations.

      // However, we cannot destruct an object passed in with a reference.
  }

Separate Memory Operations
++++++++++++++++++++++++++

The memory allocation and deallocation is not consistent in ``worker1()`` and
``worker2()``.  This kind of problems are commonplace.

.. code-block:: cpp
  :linenos:

  Data * worker1()
  {
      // Create a new Data object.
      Data * data = new Data();

      // Manipulate the Data object.
      manipulate_with_reference(*data);

      return data;
  }

  /*
   * Code in this function is intentionally made to be lack of discipline to
   * demonstrate how ownership is messed up.
   */
  void worker2(Data * data)
  {
      // The prerequisite for the caller to write correct code is to read the
      // code and understand when the object is alive.
      if (data->is_manipulated())
      {
          delete data;
      }
      else
      {
          manipulate_with_reference(*data);
      }
  }

  int main(int, char **)
  {
      Data * data = worker1();
      std::cout << "Data pointer after worker 1: " << data << std::endl;
      worker2(data);
      std::cout << "Data pointer after worker 2: " << data << std::endl;

      // You have to read the code of worker2 to know that data could be
      // destructed.  In addition, the Data class doesn't provide a
      // programmatical way to detect whether or not the object is alive.  The
      // design of Data, worker1, and worker2 makes it impossible to write
      // memory-safe code.
  #ifdef CRASHME // The fenced code causes double free.
      delete data;
      std::cout << "Data pointer after delete: " << data << std::endl;
  #endif
  }

.. admonition:: Execution Results

  :download:`code/01_pointer/03_ownership.cpp`

  .. code-block:: console
    :caption: Build ``03_ownership.cpp``

    $ g++ 03_ownership.cpp -o 03_ownership -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``03_ownership``
    :linenos:

    $ ./03_ownership
    Data @0x7fb287008800 is constructed
    Manipulate with reference: 0x7fb287008800
    Data pointer after worker 1: 0x7fb287008800
    Data @0x7fb287008800 is destructed
    Data pointer after worker 2: 0x7fb287008800

  .. code-block:: console
    :caption: Build ``03_ownership.cpp`` with the crashing behavior

    $ g++ 03_ownership.cpp -o 03_ownership -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCRASHME

  .. code-block:: console
    :caption: Crash ``03_ownership``
    :linenos:

    $ ./03_ownership
    Data @0x7f8ef9808800 is constructed
    Manipulate with reference: 0x7f8ef9808800
    Data pointer after worker 1: 0x7f8ef9808800
    Data @0x7f8ef9808800 is destructed
    Data pointer after worker 2: 0x7f8ef9808800
    Data @0x7f8ef9808800 is destructed
    03_ownership(75158,0x114718e00) malloc: *** error for object 0x7f8ef9808800: pointer being freed was not allocated
    03_ownership(75158,0x114718e00) malloc: *** set a breakpoint in malloc_error_break to debug

What Is Ownership
+++++++++++++++++

The above example shows the problem of lack of ownership.  "Ownership" isn't
officially a language construct in C++, but is a common concept in many
programming language for dynamic memory management.

To put it simply, when the object is "owned" by a construct or piece of code,
it is assumed that it is safe for the piece of code to use that object.  The
ownership assures the life of the object, and the object is not destructed when
it is owned by someone.  It also means that the owner is responsible for making
sure the object gets destructed when it should be.

As we observed in the above example code, there is no way for us to let the
code to know the ownership, and it is unsafe to use the ``data`` object after
``worker2()`` is called.  The way C++ handles the situation is to use smart
pointers.

``unique_ptr``
==============

(Modern) C++ provides two smart pointers: ``unique_ptr`` and ``shared_ptr``.
We start with ``unique_ptr`` because it is lighter-weight.  A ``unique_ptr``
takes the same number of bytes of a raw pointer.  It may be a drop-in replace
with a raw pointer.

``unique_ptr`` should be used when there can only be one owner of the pointed
object.

.. code-block:: cpp
  :linenos:

  static_assert(sizeof(Data *) == sizeof(std::unique_ptr<Data>), "unique_ptr should take only a word");

  std::unique_ptr<Data> worker1()
  {
      // Create a new Data object.
      std::unique_ptr<Data> data = std::make_unique<Data>();

      // Manipulate the Data object.
      manipulate_with_reference(*data);

      return data;
  }

  void worker2(std::unique_ptr<Data> data)
  {
      if (data->is_manipulated())
      {
          data.reset();
      }
      else
      {
          manipulate_with_reference(*data);
      }
  }

  int main(int, char **)
  {
      std::unique_ptr<Data> data = worker1();
      std::cout << "Data pointer after worker 1: " << data.get() << std::endl;

  #ifdef COPYNOWORK
      worker2(data);
  #else
      worker2(std::move(data));
  #endif
      std::cout << "Data pointer after worker 2: " << data.get() << std::endl;

      data.reset();
      std::cout << "Data pointer after delete: " << data.get() << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/01_pointer/04_unique.cpp`

  .. code-block:: console
    :caption: Build ``04_unique.cpp``

    $ g++ 04_unique.cpp -o 04_unique -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``04_unique``
    :linenos:

    $ ./04_unique
    Data @0x7fee5a008800 is constructed
    Manipulate with reference: 0x7fee5a008800
    Data pointer after worker 1: 0x7fee5a008800
    Data @0x7fee5a008800 is destructed
    Data pointer after worker 2: 0x0
    Data pointer after delete: 0x0

  .. code-block:: console
    :caption: Copy not working in ``04_unique.cpp``

    $ g++ 04_unique.cpp -o 04_unique -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCOPYNOWORK
    04_unique.cpp:97:13: error: call to implicitly-deleted copy constructor of 'std::unique_ptr<Data>'
        worker2(data);
                ^~~~
    /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:2518:3: note: copy constructor is implicitly deleted because
          'unique_ptr<Data, std::__1::default_delete<Data> >' has a user-declared move constructor
      unique_ptr(unique_ptr&& __u) _NOEXCEPT
      ^
    04_unique.cpp:79:36: note: passing argument to parameter 'data' here
    void worker2(std::unique_ptr<Data> data)
                                       ^
    1 error generated.

``shared_ptr``
==============

Unlike ``unique_ptr``, ``shared_ptr`` allows multiple owners.  It maintains a
reference counter.  When the ``shared_ptr`` object is constructed, the counter
increments.  When the pointer object (note, not the pointed object) is
destructed, the counter decrements.  When the counter decrements from 1, the
pointed object gets destructed.

``std::shared_ptr`` provides ``use_count()`` function for showing the reference
counts.  This reference counting technique is commonplace for managing
ownership, and it appears in many other languages.

.. code-block:: cpp
  :linenos:

  static_assert(sizeof(Data *) < sizeof(std::shared_ptr<Data>), "shared_ptr uses more than a word");

  std::shared_ptr<Data> worker1()
  {
      // Create a new Data object.
      std::shared_ptr<Data> data = std::make_shared<Data>();

      std::cout << "worker 1 data.use_count(): " << data.use_count() << std::endl;

      // Manipulate the Data object.
      manipulate_with_reference(*data);

      return data;
  }

  void worker2(std::shared_ptr<Data> data)
  {
      std::cout << "worker 2 data.use_count(): " << data.use_count() << std::endl;

      if (data->is_manipulated())
      {
          data.reset();
      }
      else
      {
          manipulate_with_reference(*data);
      }
  }

  int main(int, char **)
  {
      std::shared_ptr<Data> data = worker1();
      std::cout << "Data pointer after worker 1: " << data.get() << std::endl;

      worker2(data);
      std::cout << "Data pointer after worker 2: " << data.get() << std::endl;

      data.reset();
      std::cout << "Data pointer after reset from outside: " << data.get() << std::endl;
      std::cout << "main data.use_count(): " << data.use_count() << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/01_pointer/05_shared.cpp`

  .. code-block:: console
    :caption: Build ``05_shared.cpp``

    $ g++ 05_shared.cpp -o 05_shared -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``05_shared``
    :linenos:

    $ ./05_shared
    Data @0x7ffbac500018 is constructed
    worker 1 data.use_count(): 1
    Manipulate with reference: 0x7ffbac500018
    Data pointer after worker 1: 0x7ffbac500018
    worker 2 data.use_count(): 2
    Data pointer after worker 2: 0x7ffbac500018
    Data @0x7ffbac500018 is destructed
    Data pointer after reset from outside: 0x0
    main data.use_count(): 0

Raw Pointers vs Smart Pointers
==============================

The rule of thumb is to always start with smart pointers.  When in doubt, use
``unique_ptr``.  ``unique_ptr`` forces a developer to think clearly about
whether or not multiple owners are necessary.  Only use ``shared_ptr`` when it
is absolutely necessary.  The reference counter is much more expensive than it
looks.

Make ``Data`` Exclusively Managed by ``shared_ptr``
===================================================

Sometimes we know a big resource (our ``Data`` class) must not be constructed
and destructed lightly, and it must be managed by a ``shared_ptr``.  The
overhead of reference counting is negligible compare to other operations or we
simply have to tolerate.  In this case, we do not want anyone to directly call
the ``Data`` constructor:

.. code-block:: cpp
  :linenos:

  // We want to forbid it.
  Data * raw_pointer = new Data;

And allow only the ``shared_ptr`` construction:

.. code-block:: cpp
  :linenos:

  // We want this to work:
  std::shared_ptr<Data> sptr1(new Data);
  // Or this:
  std::shared_ptr<Data> sptr2 = std::make_shared<Data>();

The problem is, if ``new Data`` is forbidden, ``std::shared_ptr<Data>(new
Data)`` is forbidden too.  How can we only turn off the first but not the
second?

One idea is to use private constructor:

.. code-block:: cpp
  :linenos:

  class Data
  {
  private:
      Data() {}
  public:
      static std::shared_ptr<Data> make()
      {
          std::shared_ptr<Data> ret(new Data);
          return ret;
      }
  };

  std::shared_ptr<Data> func()
  {
  #ifdef THISDOESNOTWORK
      std::shared_ptr<Data> data(new Data);
  #else
      std::shared_ptr<Data> data = Data::make();
  #endif
      // Do work.
      return ret;
  }

This works because the constructor is called from the ``Data`` class, in the
line of ``std::shared_ptr<Data> ret(new Data)``.

Now consider ``std::shared_ptr<Data> ret = std::make_shared<Data>()``, the
private constructor fails to compile, because the function template
``std::make_shared`` cannot access the private constructor!  We need to work it
out.  You may think of ``friend``.  In some cases it works, but note that
``std::make_shared`` is a function template, not a function.  Friendship to a
function template is not straight-forward.  Moreover, ``std::make_shared`` does
a lot of things behind the scene.  Simply making friend with that function
template may or may not work.  It depends on how STL implement
``std::make_shared``.

A sound resolution is to use the passkey pattern:

.. code-block:: cpp
  :linenos:

  class Data
  {
  private:
      class ctor_passkey {};
  public:
      static std::shared_ptr<Data> make()
      {
          std::shared_ptr<Data> ret = std::make_shared<Data>(ctor_passkey());
          return ret;
      }
      Data() = delete;
      Data(ctor_passkey const &) {}
      // TODO: Copyability and moveability should be considered, but we leave
      // them for now.
  };

  std::shared_ptr<Data> worker1()
  {
      // Create a new Data object.
      std::shared_ptr<Data> data;

  #ifdef CTORNOWORK
      data = std::shared_ptr<Data>(new Data);
  #endif

  #ifdef MAKENOWORK
      data = std::make_shared<Data>();
  #endif

      data = Data::make();

      std::cout << "worker 1 data.use_count(): " << data.use_count() << std::endl;

      // Manipulate the Data object.
      manipulate_with_reference(*data);

      return data;
  }

  void worker2(std::shared_ptr<Data> data)
  {
      std::cout << "worker 2 data.use_count(): " << data.use_count() << std::endl;

      if (data->is_manipulated())
      {
          data.reset();
      }
      else
      {
          manipulate_with_reference(*data);
      }
  }

  int main(int, char **)
  {
      std::shared_ptr<Data> data = worker1();
      std::cout << "Data pointer after worker 1: " << data.get() << std::endl;

      worker2(data);
      std::cout << "Data pointer after worker 2: " << data.get() << std::endl;

      data.reset();
      std::cout << "Data pointer after reset from outside: " << data.get() << std::endl;
      std::cout << "main data.use_count(): " << data.use_count() << std::endl;
  }

  static_assert(sizeof(Data *) * 2 == sizeof(std::shared_ptr<Data>), "shared_ptr should use two word");

.. admonition:: Execution Results

  :download:`code/02_shared/01_fully.cpp`

  .. code-block:: console
    :caption: Build ``01_fully.cpp``

    $ g++ 01_fully.cpp -o 01_fully -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``01_fully``
    :linenos:

    $ ./01_fully
    Data @0x7fb36ad00018 is constructed
    worker 1 data.use_count(): 1
    Manipulate with reference: 0x7fb36ad00018
    Data pointer after worker 1: 0x7fb36ad00018
    worker 2 data.use_count(): 2
    Data pointer after worker 2: 0x7fb36ad00018
    Data @0x7fb36ad00018 is destructed
    Data pointer after reset from outside: 0x0
    main data.use_count(): 0

  .. code-block:: console
    :caption: Error message when constructor doesn't work

    $ g++ 01_fully.cpp -o 01_fully -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCTORNOWORK
    01_fully.cpp:91:38: error: call to deleted constructor of 'Data'
        data = std::shared_ptr<Data>(new Data);
                                         ^
    01_fully.cpp:22:5: note: 'Data' has been explicitly marked deleted here
        Data() = delete;
        ^
    1 error generated.

  .. code-block:: console
    :caption: Error message when ``make_shared`` doesn't work

    $ g++ 01_fully.cpp -o 01_fully -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DMAKENOWORK
    In file included from 01_fully.cpp:1:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/iostream:37:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/ios:215:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/__locale:14:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/string:504:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/string_view:175:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/__string:57:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/algorithm:643:
    /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:4398:5: error: static_assert failed due to requirement
          'is_constructible<Data>::value' "Can't construct object in make_shared"
        static_assert(is_constructible<_Tp, _Args...>::value, "Can't construct object in make_shared");
        ^             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    01_fully.cpp:95:17: note: in instantiation of function template specialization 'std::__1::make_shared<Data>' requested here
        data = std::make_shared<Data>();
                    ^
    In file included from 01_fully.cpp:1:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/iostream:37:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/ios:215:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/__locale:14:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/string:504:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/string_view:175:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/__string:57:
    In file included from /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/algorithm:643:
    /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:2201:46: error: call to deleted constructor of 'Data'
      __compressed_pair_elem(__value_init_tag) : __value_() {}
                                                 ^
    /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:2294:42: note: in instantiation of member function
          'std::__1::__compressed_pair_elem<Data, 1, false>::__compressed_pair_elem' requested here
          : _Base1(std::forward<_U1>(__t1)), _Base2(std::forward<_U2>(__t2)) {}
                                             ^
    /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:3567:12: note: in instantiation of function template specialization
          'std::__1::__compressed_pair<std::__1::allocator<Data>, Data>::__compressed_pair<std::__1::allocator<Data>, std::__1::__value_init_tag>'
          requested here
            :  __data_(_VSTD::move(__a), __value_init_tag()) {}
               ^
    /Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:4405:26: note: in instantiation of member function
          'std::__1::__shared_ptr_emplace<Data, std::__1::allocator<Data> >::__shared_ptr_emplace' requested here
        ::new(__hold2.get()) _CntrlBlk(__a2, _VSTD::forward<_Args>(__args)...);
                             ^
    01_fully.cpp:95:17: note: in instantiation of function template specialization 'std::__1::make_shared<Data>' requested here
        data = std::make_shared<Data>();
                    ^
    01_fully.cpp:22:5: note: 'Data' has been explicitly marked deleted here
        Data() = delete;
        ^
    2 errors generated.

Get ``shared_ptr`` from ``this``
================================

Occasionally we get the ``Data`` object without the ``shared_ptr`` that manages
it, but want to give the ownership to the caller:

.. code-block:: cpp
  :linenos:

  class Data
  {
  public:
      Data * get_raw_ptr()
      {
          // Returning raw pointer discards the ownership management.
          return this;
      }

      std::shared_ptr<Data> get_shared_ptr()
      {
          // XXX: Recreate a shared_ptr will duplicate the reference counter, and
          // later results into double free.
          return std::shared_ptr<Data>(this);
      }
  };

The above code (``get_shared_ptr()``) naively creates an alternate
``shared_ptr`` object, and will results in double free in the caller:

.. code-block:: cpp
  :linenos:

  int main(int, char **)
  {
      std::shared_ptr<Data> data = Data::make();
      std::cout << "data.use_count(): " << data.use_count() << std::endl;

  #ifdef DUPSHARED
      std::shared_ptr<Data> holder2 = data->get_shared_ptr();
  #endif

      data.reset();
      std::cout << "data.use_count() after data.reset(): " << data.use_count() << std::endl;

  #ifdef DUPSHARED
      std::cout << "holder2.use_count(): " << holder2.use_count() << std::endl;
      holder2.reset();
      // This line never gets reached since the the above line causes double free
      // and crash.
      std::cout << "holder2.use_count() after holder2.reset(): " << holder2.use_count() << std::endl;
  #endif
  }

.. admonition:: Execution Results

  :download:`code/02_shared/02_duplicate.cpp`

  .. code-block:: console
    :caption: Build ``02_duplicate.cpp``

    $ g++ 02_duplicate.cpp -o 02_duplicate -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``02_duplicate``
    :linenos:

    $ ./02_duplicate
    Data @0x7faaf0d00018 is constructed
    data.use_count(): 1
    Data @0x7faaf0d00018 is destructed
    data.use_count() after data.reset(): 0
    holder2.use_count(): 1
    Data @0x7faaf0d00018 is destructed
    02_duplicate(76813,0x10d1c7e00) malloc: *** error for object 0x7faaf0d00018: pointer being freed was not allocated
    02_duplicate(76813,0x10d1c7e00) malloc: *** set a breakpoint in malloc_error_break to debug

Never Recreate ``shared_ptr`` from Raw Pointer
==============================================

The right way to get a ``std::shared_ptr`` from a ``shared_ptr``-managed object
is to use ``std::enable_shared_from_this``:

.. code-block:: cpp
  :linenos:

  class Data
    : public std::enable_shared_from_this<Data>
  {
  public:
      std::shared_ptr<Data> get_shared_ptr()
      {
          // This is the right way to get the shared pointer from within the
          // object.
          return shared_from_this();
      }
  };

  int main(int, char **)
  {
      std::shared_ptr<Data> data = Data::make();
      std::cout << "data.use_count(): " << data.use_count() << std::endl;

      std::shared_ptr<Data> holder2 = data->get_shared_ptr();
      std::cout << "data.use_count() after holder2: " << data.use_count() << std::endl;

      data.reset();
      std::cout << "data.use_count() after data.reset(): " << data.use_count() << std::endl;

      std::cout << "holder2.use_count() before holder2.reset(): " << holder2.use_count() << std::endl;
      holder2.reset();
      std::cout << "holder2.use_count() after holder2.reset(): " << holder2.use_count() << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/02_shared/03_fromthis.cpp`

  .. code-block:: console
    :caption: Build ``03_fromthis.cpp``

    $ g++ 03_fromthis.cpp -o 03_fromthis -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``03_fromthis``
    :linenos:

    $ ./03_fromthis
    Data @0x7fc5bed00018 is constructed
    data.use_count(): 1
    data.use_count() after holder2: 2
    data.use_count() after data.reset(): 0
    holder2.use_count() before holder2.reset(): 1
    Data @0x7fc5bed00018 is destructed
    holder2.use_count() after holder2.reset(): 0

Cyclic Reference
================

When two object use a pair of ``shared_ptr`` to point to each other, the cyclic
reference will create a memory leak:

.. code-block:: cpp
  :linenos:

  class Data
    : public std::enable_shared_from_this<Data>
  {
  public:
      std::shared_ptr<Child>   child() const { return m_child; }
      std::shared_ptr<Child> & child()       { return m_child; }
  private:
      std::shared_ptr<Child> m_child;
  };

  class Child
    : public std::enable_shared_from_this<Child>
  {
  private:
      class ctor_passkey {};
  public:
      Child() = delete;
      Child(std::shared_ptr<Data> const & data, ctor_passkey const &) : m_data(data) {}
      static std::shared_ptr<Child> make(std::shared_ptr<Data> const & data)
      {
          std::shared_ptr<Child> ret = std::make_shared<Child>(data, ctor_passkey());
          data->child() = ret;
          return ret;
      }
  private:
      std::shared_ptr<Data> m_data;
  };

  int main(int, char **)
  {
      std::shared_ptr<Data> data = Data::make();
      std::shared_ptr<Child> child = Child::make(data);
      std::cout << "data.use_count(): " << data.use_count() << std::endl;
      std::cout << "child.use_count(): " << child.use_count() << std::endl;

      std::weak_ptr<Data> wdata(data);
      std::weak_ptr<Child> wchild(child);

      data.reset();
      std::cout << "wdata.use_count() after data.reset(): " << wdata.use_count() << std::endl;
      std::cout << "wchild.use_count() after data.reset(): " << wchild.use_count() << std::endl;

      child.reset();
      std::cout << "wdata.use_count() after child.reset(): " << wdata.use_count() << std::endl;
      std::cout << "wchild.use_count() after child.reset(): " << wchild.use_count() << std::endl;
      // Oops, the reference count doesn't reduce to 0!
  }

.. admonition:: Execution Results

  :download:`code/02_shared/04_cyclic.cpp`

  .. code-block:: console
    :caption: Build ``04_cyclic.cpp``

    $ g++ 04_cyclic.cpp -o 04_cyclic -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``04_cyclic``
    :linenos:

    $ ./04_cyclic
    Data @0x7f8f48d00018 is constructed
    data.use_count(): 2
    child.use_count(): 2
    wdata.use_count() after data.reset(): 1
    wchild.use_count() after data.reset(): 2
    wdata.use_count() after child.reset(): 1
    wchild.use_count() after child.reset(): 1

Use ``weak_ptr`` to Break Cyclic Reference
++++++++++++++++++++++++++++++++++++++++++

In the above demonstration we use ``weak_ptr`` to get the reference count without
increasing it.  ``weak_ptr`` can also be used to break the cyclic reference.  In
the following example, the ``Child`` object replaces ``shared_ptr`` with ``weak_ptr``
to point to ``Data``:

.. code-block:: cpp
  :linenos:

  class Child
    : public std::enable_shared_from_this<Child>
  {
  private:
      class ctor_passkey {};
  public:
      Child() = delete;
      Child(std::shared_ptr<Data> const & data, ctor_passkey const &) : m_data(data) {}
      static std::shared_ptr<Child> make(std::shared_ptr<Data> const & data)
      {
          std::shared_ptr<Child> ret = std::make_shared<Child>(data, ctor_passkey());
          data->child() = ret;
          return ret;
      }
  private:
      // Replace shared_ptr with weak_ptr to Data.
      std::weak_ptr<Data> m_data;
  };

  int main(int, char **)
  {
      std::shared_ptr<Data> data = Data::make();
      std::shared_ptr<Child> child = Child::make(data);
      std::cout << "data.use_count(): " << data.use_count() << std::endl;
      std::cout << "child.use_count(): " << child.use_count() << std::endl;

      std::weak_ptr<Data> wdata(data);
      std::weak_ptr<Child> wchild(child);

      child.reset();
      std::cout << "wdata.use_count() after child.reset(): " << wdata.use_count() << std::endl;
      std::cout << "wchild.use_count() after child.reset(): " << wchild.use_count() << std::endl;

      data.reset();
      std::cout << "wdata.use_count() after data.reset(): " << wdata.use_count() << std::endl;
      std::cout << "wchild.use_count() after data.reset(): " << wchild.use_count() << std::endl;
  }

.. admonition:: Execution Results

  :download:`code/02_shared/05_weak.cpp`

  .. code-block:: console
    :caption: Build ``05_weak.cpp``

    $ g++ 05_weak.cpp -o 05_weak -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

  .. code-block:: console
    :caption: Run ``05_weak``
    :linenos:

    $ ./05_weak
    Data @0x7fe6f8500018 is constructed
    data.use_count(): 1
    child.use_count(): 2
    wdata.use_count() after child.reset(): 1
    wchild.use_count() after child.reset(): 1
    Data @0x7fe6f8500018 is destructed
    wdata.use_count() after data.reset(): 0
    wchild.use_count() after data.reset(): 0

Reminder: Avoid ``weak_ptr``
++++++++++++++++++++++++++++

Using ``weak_ptr`` to break cyclic reference should only be considered as a
workaround, rather than a full resolution.  We sometimes need it since the
reference cycle may not be as obvious as it is in our example.  For example,
there may be 3 or 4 levels of references in the cycle.  ``weak_ptr`` has a
similar interface to ``shared_ptr``.  When we are troubleshooting
resource-leaking issues, replacing ``shared_ptr`` with ``weak_ptr`` can work as
a quick-n-dirty hotfix.

The right treatment is to sort out the ownership.  It's not easy when the
system is complex.  The rule of thumb is that, as we mentioned earlier, you
should avoid using ``shared_ptr`` unless you really need it.  And most of the
time the need appears in a higher-level and heavy-weight container, rather than
the lower-level small objects.  For small objects, we should try to limit the
lifecycle and use raw pointers or stack.

Exercises
=========

1. Write code so that when ``std::unique_ptr`` is destructed, the object it
   points to doesn't destruct.
2. Create vectors of 1,000,000 elements of (i) raw pointers, (ii) ``unique_ptr``,
   and (iii) ``shared_ptr``, respectively, and measure difference of the
   performance.
3. Compare the runtime performance between ``shared_ptr(new Type)`` and
   ``make_shared<Type>``.  Explain why there is a difference of performance.

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
