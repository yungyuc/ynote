=========================
Use Python Objects in C++
=========================

:date: 2018/5/6

C++ is difficult.  The language has so many catches and requires us to take care
of every detail.  The reason to use it is usually performance.  But learning C++
is so frustrating that when performance isn't mandatory, people try hard to
avoid it.  Things are getting better now.  Recently, C++11 standard looks just
like the right way to go for the language.  C++ is still hard, but the learning
path isn't as concealed as it did.

It now makes sense to encourage a Python programmer to invest in C++.  These are
two very different languages.  Each is good in its field.  Although both are
pronounced and designed to be general-purpose, and indeed are used for
everything, it's naive to use vanilla Python for anything calling for speed.  On
the other hand, C++ is too heavy-lifting for simple one-liners or scripts.  To
master the two very different languages gives a programmer powerful synergy.
There are alreay many wrappers developed to bridge them.  Python the interpreter
is also a well-organized C library easy to be used from C++.  The major thing
that Python programmers concerned was the complexity of C++ the language.  With
C++11, it is very much mitigated.

And we also have `pybind11 <http://pybind11.readthedocs.io/>`_, a compact
library providing comprehensive wrapping between Python and C++11.  It also
includes neat API for manipulating Python objects.  The API is very basic and
covers only a fraction of what Python C API does, but fun to use.  And it's the
real point of this post: make a note about manipulating Python objects with
pybind11.  Other parts of the library are important and probably more useful
than this.  But I find this particularly interesting.

I am starting with "hello, world".  But it's boring to just print a Python
:py:class:`str`, which doesn't differ much from C++ :cpp:class:`std::string`.
Let me use a container to do it:

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <vector>
  #include <string>
  namespace py = pybind11;
  PYBIND11_MODULE(_helloworld, mod) {
    mod.def(
      "do",
      []() {
        std::vector<char> v{'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'};
        py::list l;
        for (auto & i : v) {
          py::str s(std::string(1, i));
          l.append(s);
        }
        return l;
      },
      "a little more interesting hello world"
    );
  } /* end PYBIND11_PLUGIN(_helloworld) */

That's how you create a Python :py:class:`list` in C++.  And you see how it's
returned to Python and gets the famous hello, world:

.. code-block:: python
  :linenos:

  >>> import _helloworld
  >>> print(_helloworld.do())
  ['h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd']
  >>> print("".join(_helloworld.do()))
  hello, world

:cpp:class:`pybind11::list`
===========================

More on :cpp:class:`pybind11::list`.  Take a look at
https://github.com/pybind/pybind11/blob/master/include/pybind11/pytypes.h, it's
just a thin shell to access :c:type:`PyList` API.  The pybind11 support isn't
fancy, but enough for basic operations.

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <string>
  #include <iostream>
  namespace py = pybind11;
  PYBIND11_MODULE(_pylist, mod) {
    mod.def(
      "do",
      [](py::list & l) {
        // convert contents to std::string and send to cout
        std::cout << "std::cout:" << std::endl;
        for (py::handle o : l) {
          std::string str = py::cast<std::string>(o);
          std::cout << str << std::endl;
        }
      }
    );
  } /* end PYBIND11_PLUGIN(_pylist) */

Run the code.  Elements are converted to :cpp:class:`std::string` and sent to
standard output one by one:

.. code-block:: python
  :linenos:

  >>> import _pylist
  >>> # print the input list
  >>> _pylist.do(["a", "b", "c"])
  std::cout:
  a
  b
  c

pybind11 provides :cpp:class:`pybind11::list::append` to populate elements (we
saw it in the hello, world).  Spell it out:
 
.. code-block:: cpp
  :linenos:

  mod.def(
    "do2",
    [](py::list & l) {
      // create a new list
      std::cout << "py::print:" << std::endl;
      py::list l2;
      for (py::handle o : l) {
        std::string s = py::cast<std::string>(o);
        s = "elm:" + s;
        py::str s2(s);
        l2.append(s2); // populate contents
      }
      py::print(l2);
    }
  );

This is the result:

.. code-block:: python
  :linenos:

  >>> _pylist.do2(["d", "e", "f"])
  py::print:
  ['elm:d', 'elm:e', 'elm:f']

:cpp:class:`pybind11::tuple`
============================

:py:class:`tuple` is immutable and more restrictive than :py:class:`list`.
pybind11 provides API for reading it.  To creat a non-trivial tupple, we can
convert from a sequence object:

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <vector>
  namespace py = pybind11;
  PYBIND11_MODULE(_pytuple, mod) {
    mod.def(
      "do",
      [](py::args & args) {
        // build a list using py::list::append
        py::list l;
        for (py::handle h : args) {
          l.append(h);
        }
        // convert it to a tuple
        py::tuple t(l);
        // print it out
        py::print(py::str("{} len={}").format(t, t.size()));
        // print the element one by one
        for (size_t it=0; it<t.size(); ++it) {
          py::print(py::str("{}").format(t[it]));
        }
      }
    );
  } /* end PYBIND11_PLUGIN(_pytuple) */

Execution in Python:

.. code-block:: python
  :linenos:

  >>> import _pytuple
  >>> _pytuple.do("a", 7, 5.6)
  ('a', 7, 5.6) len=3
  a
  7
  5.6

:cpp:class:`pybind11::dict`
===========================

:cpp:class:`pybind11::dict` is slightly richer than the sequences.  This is how
to create a :py:class:`dict` from a :py:class:`tuple` in C++:

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <string>
  #include <stdexcept>
  #include <iostream>
  namespace py = pybind11;
  PYBIND11_MODULE(_pydict, mod) {
    mod.def(
      "do",
      [](py::args & args) {
        if (args.size() % 2 != 0) {
          throw std::runtime_error("argument number must be even");
        }
        // create a dict from the input tuple
        py::dict d;
        for (size_t it=0; it<args.size(); it+=2) {
          d[args[it]] = args[it+1];
        }
        return d;
      }
    );
  } /* end PYBIND11_PLUGIN(_pydict) */

Result:

.. code-block:: python
  :linenos:

  >>> import _pydict
  >>> d = _pydict.do("a", 7, "b", "name", 10, 4.2)
  >>> print(d)
  {'a': 7, 'b': 'name', 10: 4.2}

In addition to the obvious :cpp:func:`pybind11::dict::size`, it has
:cpp:func:`pybind11::dict::clear` and :cpp:func:`pybind11::dict::contains`.  The
second example uses them to process the created dict:

.. code-block:: cpp
  :linenos:

  mod.def(
    "do2",
    [](py::dict d, py::args & args) {
      for (py::handle h : args) {
        if (d.contains(h)) {
          std::cout << py::cast<std::string>(h)
                    << " is in the input dictionary" << std::endl;
        } else {
          std::cout << py::cast<std::string>(h)
                    << " is not found in the input dictionary" << std::endl;
        }
      }
      std::cout << "remove everything in the input dictionary!" << std::endl;
      d.clear();
      return d;
    }
  );

Then the dictionary becomes empty:

.. code-block:: python
  :linenos:

  >>> d2 = _pydict.do2(d, "b", "d")
  b is in the input dictionary
  d is not found in the input dictionary
  remove everything in the input dictionary!
  >>> print("The returned dictionary is empty:", d2)
  The returned dictionary is empty: {}
  >>> print("The first dictionary becomes empty too:", d)
  The first dictionary becomes empty too: {}
  >>> print("Are the two dictionaries the same?", d2 is d)
  Are the two dictionaries the same? True

:cpp:class:`pybind11::str`
==========================

I've used :cpp:class:`pybind11::str` many times in previous examples.  Here I
just bring up one more trick: C++11 literal for strings.

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <iostream>
  namespace py = pybind11;
  using namespace py::literals; // to bring in the `_s` literal
  PYBIND11_MODULE(_pystr, mod) {
    mod.def(
      "do",
      []() {
        py::str s("python string {}"_s.format("formatting"));
        py::print(s);
      }
    );
  } /* end PYBIND11_PLUGIN(_pystr) */

Result:

.. code-block:: python
  :linenos:

  >>> import _pystr
  >>> _pystr.do()
  python string formatting

:cpp:class:`pybind11::handle` and :cpp:class:`pybind11::object`
===============================================================

:cpp:class:`pybind11::handle` is a thin wrapper in C++ to the Python
:c:type:`PyObject`.  It's the base class of all pybind11 classes that wrap
around Python types.

:cpp:class:`pybind11::object` is derived from :cpp:class:`pybind11::handle`, and
adds automatic reference counting.  The two classes offer bookkeeping for Python
objects in pybind11.

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <iostream>
  namespace py = pybind11;
  using namespace py::literals; // to bring in the `_s` literal
  PYBIND11_MODULE(_pyho, mod) {
    mod.def(
      "do",
      [](py::object const & o) {
        std::cout << "refcount in the beginning: "
                  << o.ptr()->ob_refcnt << std::endl;
        py::handle h(o);
        std::cout << "no increase of refcount with a new pybind11::handle: "
                  << h.ptr()->ob_refcnt << std::endl;
        {
          py::object o2(o);
          std::cout << "increased refcount with a new pybind11::object: "
                    << o2.ptr()->ob_refcnt << std::endl;
        }
        std::cout << "decreased refcount after the new pybind11::object destructed: "
                  << o.ptr()->ob_refcnt << std::endl;
        h.inc_ref();
        std::cout << "manually increases refcount after h.inc_ref(): "
                  << h.ptr()->ob_refcnt << std::endl;
        h.dec_ref();
        std::cout << "manually descrases refcount after h.dec_ref(): "
                  << h.ptr()->ob_refcnt << std::endl;
      }
    );
  } /* end PYBIND11_PLUGIN(_pyho) */

See the change of the reference count.

.. code-block:: python
  :linenos:

  >>> import _pyho
  >>> _pyho.do(["name"])
  refcount in the beginning: 3
  no increase of refcount with a new pybind11::handle: 3
  increased refcount with a new pybind11::object: 4
  decreased refcount after the new pybind11::object destructed: 3
  manually increases refcount after h.inc_ref(): 4
  manually descrases refcount after h.dec_ref(): 3

:cpp:class:`pybind11::none`
===========================

The last class covered in this note is :cpp:class:`pybind11::none`.  It is just
the :py:obj:`None` object, or in the C API :c:type:`Py_None`.  :py:obj:`None` is
also reference counted, and it's convenient that in pybind11 we have a class
representing it.

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <iostream>
  namespace py = pybind11;
  using namespace py::literals; // to bring in the `_s` literal
  PYBIND11_MODULE(_pynone, mod) {
    mod.def(
      "do",
      [](py::object const & o) {
        if (o.is(py::none())) {
          std::cout << "it is None" << std::endl;
        } else {
          std::cout << "it is not None" << std::endl;
        }
      }
    );
  } /* end PYBIND11_PLUGIN(_pynone) */

See the test result:

.. code-block:: python
  :linenos:

  >>> import _pynone
  >>> _pynone.do(None)
  it is None
  >>> _pynone.do(False)
  it is not None

Reference
=========

List of Python types supported in pybind11:
https://pybind11.readthedocs.io/en/stable/advanced/pycpp/object.html.
