=========================
Use Python Objects in C++
=========================

:date: draft on 2018/5/4

Ten years ago, if someone started to learn programming using C++, it would be a
frustrated journey.  The language has so many catches and duct tapes.  It takes
a lot of time to make it work, and even more efforts to make it work well.
Elegancy is usually only left to selected few who stand fast against the pain
in the process.  But to achieve uncompromisable performance, there isn't
another option.

C++ is still difficult.  But the C++11 standard looks just like the right part
of the language, and the enhancements it needs.  Programming in C++ doesn't
become easy, but the path doesn't look as concealed as it did.

It now makes sense for a Python programmer to invest in C++.  These are two
very different languages.  Each is good in its field.  Although both are
pronounced and designed to be general-purpose, and indeed are used for
everything, it's naive to use vanilla Python for anything needing speed.  On
the other hand, only C++ maniacs use it for one-liners or scripts that seldom
are executed more than twice.  But the two languages are surprisingly good
companions.  In the past decades almost a dozen tools are made to bridge C++
and Python.  Python the interpreter is also a well-organized C library easy to
be used from C++.  The major thing that Python programmers concern is really
the complexity of C++ the language.  With C++11, it is very much mitigated.

And we even have `pybind11 <http://pybind11.readthedocs.io/>`_ available.  It's
a compact library providing comprehensive wrapping between Python and C++11.
It also includes neat API for manipulating Python objects.  The API is very
basic and covers only a fraction of what Python C API does, but fun to use.
After saying this much, here's the point: this post is my note about
manipulating Python objects using pybind11.  It's so interesting that I'd like
to skip everything else in the library.

The first thing to do when meeting a new tool is to print "hello, world".  But
in that simple program, it's boring to use Python :py:class:`str` which isn't
too much different from C++ :cpp:class:`std::string`.  We should try something
more, like mutable containers:

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
returned to Python and to get the famous hello, world:

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
just a thin shell to access :c:type:`PyList` API.  It provides some more
features, but not really comprehensive.

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
        std::cout << "std::cout:" << std::endl;
        for (py::handle obj : l) {
          std::string str = py::cast<std::string>(obj);
          std::cout << str << std::endl;
        }
        std::cout << "py::print:" << std::endl;
        for (size_t it=0; it<l.size(); ++it) {
          py::print(py::str("{}").format(l[it]));
        }
        // operator+() doesn't work:
        // py::list newl = l + l;
      }
    );
  } /* end PYBIND11_PLUGIN(_pylist) */

:cpp:class:`pybind11::tuple`
============================

:py:class:`tuple` is more restrictive than :py:class:`list`.  pybind11 provides
API for reading it.  We can create a :cpp:class:`pybind11::tuple` from a
sequence object, index the content, get the length of it, but not add or modify
it.

.. code-block:: cpp
  :linenos:

  #include <pybind11/pybind11.h> // must be first
  #include <vector>
  namespace py = pybind11;
  PYBIND11_MODULE(_pytuple, mod) {
    mod.def(
      "do",
      [](py::args & args) {
        py::list l;
        for (py::handle h : args) {
          l.append(h);
        }
        py::tuple t(l);
        py::print(py::str("{} len={}").format(t, t.size()));
        for (size_t it=0; it<t.size(); ++it) {
          py::print(py::str("{}").format(t[it]));
        }
      }
    );
  } /* end PYBIND11_PLUGIN(_pytuple) */

:cpp:class:`pybind11::dict`
===========================

:cpp:class:`pybind11::dict` is slightly richer than the sequences.  In addition
to the obvious :cpp:func:`pybind11::dict::size`, it has
:cpp:func:`pybind11::dict::clear` and :cpp:func:`pybind11::dict::contains`.

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
        py::dict d;
        for (size_t it=0; it<args.size(); it+=2) {
          d[args[it]] = args[it+1];
        }
        return d;
      },
      "a little more interesting hello world"
    );
    mod.def(
      "do2",
      [](py::dict d, py::args & args) {
        for (py::handle h : args) {
          if (d.contains(h)) {
            std::cout << py::cast<std::string>(h)
                      << " is in the input dictionary" << std::endl;
          } else {
            std::cout << py::cast<std::string>(h)
                      << " isn't found in the input dictionary" << std::endl;
          }
        }
        std::cout << "remove everything in the input dictionary!" << std::endl;
        d.clear();
        return d;
      }
    );
  } /* end PYBIND11_PLUGIN(_pydict) */

:cpp:class:`pybind11::str`
==========================

I've used :cpp:class:`pybind11::str` many times in previous sample code.  Just
add one trick of C++11 literal for strings:

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

:cpp:class:`pybind11::handle` and :cpp:class:`pybind11::object`
===============================================================

:cpp:class:`pybind11::handle` is a thin wrapper in C++ to the Python
:c:type:`PyObject`.  It's the base class of all pybind11 classes that wrap
around Python types.

:cpp:class:`pybind11::object` is derived from :cpp:class:`pybind11::handle`,
and adds automatic reference counting.  The two classes offer bookkeeping for
pybind11.

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
        std::cout << "refcount with a new pybind11::handle: "
                  << h.ptr()->ob_refcnt << std::endl;
        {
          py::object o2(o);
          std::cout << "refcount with a new pybind11::object: "
                    << o2.ptr()->ob_refcnt << std::endl;
        }
        std::cout << "refcount after the new pybind11::object destructed: "
                  << o.ptr()->ob_refcnt << std::endl;
        h.inc_ref();
        std::cout << "refcount after h.inc_ref(): "
                  << h.ptr()->ob_refcnt << std::endl;
        h.dec_ref();
        std::cout << "refcount after h.dec_ref(): "
                  << h.ptr()->ob_refcnt << std::endl;
      }
    );
  } /* end PYBIND11_PLUGIN(_pyho) */

:cpp:class:`pybind11::none`
===========================

The last class is :cpp:class:`pybind11::none`.  It is just the :py:obj:`None`
object, or in the C API :c:type:`Py_None`.  :py:obj:`None` is also reference
counted, and it's convenient that in pybind11 we have a class representing it.

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
          std::cout << "it isn't None" << std::endl;
        }
      }
    );
  } /* end PYBIND11_PLUGIN(_pynone) */

Reference
=========

List of Python types supported in pybind11:
https://pybind11.readthedocs.io/en/stable/advanced/pycpp/object.html.
