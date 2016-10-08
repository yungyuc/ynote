========================================
Sharing Resources Between Python and C++
========================================

Resource management is a key to a healthy program.  No one wants to leak
resources (or worse, crash the program by resource mismanagement), but taking
care of resource life cycles isn't a cheerful task.  One great thing of Python
and other high-level languages is that they can gratify us by allowing clean
code like this:

.. code-block:: python

  def print_value(val):
    msg = "the value is %s" % val
    print(msg)
    # "msg" object gets automatically destroyed when the function returns

But the happy story ends when Python needs to talk to low-level code.  If some
data demand complex calculations, oftentimes Python is too slow.  Low-level
C/C++ will be required.  The data usually are large, and it's inefficient and
sometimes impractical to make copies between the langauges.  We'll need to
manage the resource life cycles across the language barrier.

I will discuss how shared pointers are used to share resources to and from
Python in two C++ wrapping libraries `boost.python
<www.boost.org/libs/python/>`__ and `pybind11
<http://pybind11.readthedocs.io>`__.  The former is a powerful and stable
wrapping tool and has been popular for a decade.  The latter brings all the
goodness from C++11 to the wrapping land.

.. contents:: What's in This Article
  :local:

Shared Pointers
===============

Shared pointers use reference counts to track the life cycle of an object.  The
tracked object is the resource to be managed.  As long as a shared pointer
exists, the reference count is positive, and then the resource is alive.  When
all shared pointers are destroyed to make the reference count descrease to
zero, the resource object gets deleted.

C++11 provides a shared pointer library ``std::shared_ptr``, but before the
standardization, ``boost::shared_ptr`` was the most popular one.  It probably
still is.  So far (version 1.62) boost.python only supports
``boost::shared_ptr``.  Both libraries allows to get the raw pointer and the
reference count:

.. literalinclude:: ex_shared_ptr.cpp
  :language: cpp
  :linenos:

.. note::

  I assume readers to be familiar with shared pointers.  But if you are not,
  keep in mind that it's not as innocent as it appears to be.  It shouldn't be
  used as a substitute for raw pointers.  ``std::unique_ptr``, on the other
  hand, is suggested to be used to replace a raw pointer whenever possible.
  
  For example, one common mistake with shared pointers is duplicated reference
  counters, and the program will crash because of double free:

  .. code-block:: cpp

    auto * resource = new Resource();
    auto ref1 = SHARED_PTR<Resource>(resource);
    // ooops, the duplicated counter will end up with double free
    auto ref2 = SHARED_PTR<Resource>(resource);

  The right way to do it is:

  .. code-block:: cpp

    auto * resource = new Resource();
    auto ref1 = SHARED_PTR<Resource>(resource);
    // copy the first shared pointer to use its counter
    auto ref2 = ref1;

  Catches abound in share pointers.  Be careful.

``PyObject`` also uses reference counts in a similar way.  So all we need to do
is to let ``PyObject`` and the shared pointer know the reference count of each
other, and we can freely exchange resources in both worlds.  This is how things
become interesting.  We have two wrapping libraries and two shared pointer
libraries, and their combinations give 3 different treatments: boost.python
discriminates ``boost::shared_ptr`` and ``std::shared_ptr``, while pybind11 sees
both shared pointers the same.

Boost.Python with ``boost::shared_ptr``
=======================================

Boost.python supports ``boost::shared_ptr`` as a holder type of a C++ object.
Boost.python creates a conversion while ``class_<Resource,
boost::shared_ptr<Resource>>`` is constructed.  It works well but the
conversion implementation may give us a bit surprise.  To show it, I extend the
shared pointer example with a boost.python wrapper:

.. literalinclude:: ex_bpy_to_python.cpp
  :language: cpp
  :linenos:

The ``Resource`` class is turned to a singleton, and a weak pointer tracks the
resource.  When we need a (strong) reference to the resource,
``Resource::make()`` returns a shared pointer.  Boost.python converts each of
the returned ``boost::shared_ptr<Resource>`` objects into a ``PyObject``.  The
C++ reference count is the same as the number of Python objects requested from
C++.

.. literalinclude:: ex_bpy_to_python.py
  :language: python
  :emphasize-lines: 8-9
  :linenos:

We'll see something strange when the Python object is passed back into C++.  I
make a method ``show_count_from_python()`` to uncover (:download:`or see the
full file <ex_bpy_from_python.cpp>`):

.. literalinclude:: ex_bpy_from_python.cpp
  :language: cpp
  :linenos:
  :lines: 33-36

The C++ internal counter says there are 10 shared pointers to the resource, but
the reference count of the ``boost::shared_ptr`` passed into
``show_count_from_python()`` is only 1!

.. literalinclude:: ex_bpy_from_python.py
  :language: python
  :emphasize-lines: 5,7
  :linenos:

Is there any chance that boost.python screws up ``boost::shared_ptr`` reference
counts?  By no means.  What happens is boost.python creates "another type" of
shared pointer pointing to the resource.  The boost.python-made shared pointer
doesn't use the original shared pointer's reference counter.  Instead, it
creates a deleter (``boost::python::converter::shared_ptr_delete``) that grabs
a reference to the ``PyObject`` (which contains the original shared pointer) to
be passed into C++:

.. code-block:: cpp

  struct shared_ptr_deleter {
      shared_ptr_deleter(handle<> owner);
      ~shared_ptr_deleter();
      void operator()(void const*);
      handle<> owner;
  };

So the ``boost::shared_ptr`` passed into C++ has a different reference count to
the original shared pointer.  The life cycle of the resource is collaboratively
managed by both ``boost::shared_ptr`` and ``PyObject``.

``boost::weak_ptr`` doesn't buy Boost.Python
============================================

So far we are good, as long as the resource is held by ``boost::shared_ptr``.
The references are consistent, although boost.python makes the reference counts
different depending on whether the shared pointer is returned directly from C++
or converted from Python.

But boost.python will make us miserable, when we want to track the resource
using ``boost::weak_ptr`` and initialize it using the share pointer returned
from boost.python.  I create such a holder class (:download:`or see the full
file <ex_bpy_hold_weak.cpp>`):

.. literalinclude:: ex_bpy_hold_weak.cpp
  :language: cpp
  :linenos:
  :lines: 39-47
  :emphasize-lines: 8

While feeding it the resource from
Python, we'll lose the reference in the weak pointer:

.. literalinclude:: ex_bpy_hold_weak.py
  :language: python
  :emphasize-lines: 8-9
  :linenos:

Work around Using ``boost::enable_shared_from_this``
====================================================

Sometimes we just need a weak reference, but ``shared_ptr_delete`` gets in our
way.  A right resolution is to get into the tedious business of writing our own
from-python conversion.

But if you have the liberty of changing the C++ code,
``boost::enable_shared_from_this`` provides a workaround (:download:`or see the
full file <ex_bpy_shared_from_this.cpp>`):

.. literalinclude:: ex_bpy_shared_from_this.cpp
  :language: cpp
  :linenos:
  :emphasize-lines: 1,4,10
  :lines: 9-14,39,40-42,49

By using ``boost::enable_shared_from_this``, the resource object knows where to
find the original reference counter, so we can recover it from the shared
pointer cooked by boost.python:

.. literalinclude:: ex_bpy_shared_from_this.py
  :language: python
  :emphasize-lines: 8
  :linenos:

Since ``boost::enable_shared_from_this`` is an expected companion to
``boost::shared_ptr``, the hack usually works.  It can also be applied in a
custom conversion to make it less hackish.

Boost.Python with ``std::shared_ptr``?
======================================

``std::shared_ptr`` isn't supported by boost.python, so generally we shouldn't
use it.  But because boost.python doesn't provide special treatment for it, it
can be a specimen to see what would happen if boost.python doesn't use the
deleter for the shared pointer.  Let's change the weak holder example to use
``std::shared_ptr`` (:download:`or see the full file <ex_bpy_std.cpp>`):

.. literalinclude:: ex_bpy_std.cpp
  :language: cpp
  :linenos:
  :lines: 1-5

Because ``boost::python::converter::shared_ptr_delete`` is not used for
``std::shared_ptr``, boost.python transfers the correct reference count:

.. literalinclude:: ex_bpy_std.py
  :language: python
  :emphasize-lines: 9
  :linenos:

.. note::

  Keep in mind that boost.python doesn't support ``std::shared_ptr``.  There
  will be unexpected errors to use ``std::shared_ptr`` as a boost.python holder
  type.

Pybind11
========

Pybind11 doesn't have the issue of the missing weak reference, no matter it's
``std::shared_ptr`` or ``boost::shared_ptr``.  An equivalent pybind11 wrapper
is like (full code is in :download:`std::shared_ptr <ex_pyb_std.cpp>` and
:download:`boost::shared_ptr <ex_pyb_boost.cpp>`):

.. literalinclude:: ex_pyb_std.cpp
  :language: cpp
  :linenos:
  :lines: 1,46-60

Pybind11 keeps different information through its casters.  When
``pybind11::return_value_policy::take_ownership`` is used (which is what
``return_value_policy::automatic`` falls back to), it increases the
``PyObject`` reference count instead of creating a new ``PyObject`` for each
shared pointer returned from C++.

.. literalinclude:: ex_pyb_std.py
  :language: python
  :emphasize-lines: 5-6,8-9
  :linenos:

.. note::

  ``pybind11::return_value_policy::take_ownership`` is what
  ``pybind11::return_value_policy::automatic`` falls back to, so essentially
  it's the default policy.

Shared pointers are a convenient way to transfer ownership of resources between
Python and C++.  A comprehensive wrapping tool like boost.python and pybind11
supports bi-directional transfer.  But things can be tricky when advanced
operations are performed, depending on the implementation of the wrapper
library and the shared pointer.  Sometimes, writing custom conversion or cast
code makes it more straight-forward.
