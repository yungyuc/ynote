====================
C++ and C for Python
====================

Expectation from Python
=======================

Linear Wave
+++++++++++

The governing equation is

.. math::

  \frac{\partial u}{\partial t} + \frac{\partial u}{\partial x} = 0

We will see a propagating wave from left to right with phase velocity of unity.

.. literalinclude:: code/01_linear.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

.. figure:: image/01_linear.png
  :align: center
  :width: 100%

Inviscid Burgers Equation
+++++++++++++++++++++++++

The second example is a non-linear equation:

.. math::

  \frac{\partial u}{\partial t} + u \frac{\partial u}{\partial x} = 0

The wave propagates in a way that is not predictable.

.. literalinclude:: code/01_burgers.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

.. figure:: image/01_burgers.png
  :align: center
  :width: 100%

Pybind11 Build System
=====================

`Pybind11 <https://pybind11.readthedocs.io/>`__ is a header-only C++ template
library, that allows calling CPython API and provides C++ friendly semantics to
allow Python to call C++ constructs and vise versa.

A header-only library doesn't have anything to be built.  When we say
"building" pybind11, we mean to build the project that uses pybind11.

To build pybind11, we need CPython.  It optionally depends on numpy and eigen.
There are several suggested ways to build.  Here list those I think important:

Setuptools
++++++++++

`Setuptools <https://setuptools.readthedocs.io/en/latest/>`__ is an enhancement
to Python built-in `distutils
<https://docs.python.org/3/library/distutils.html>`__.  Because pybind11 is
released to `PyPI <https://pypi.org>`__ as a Python package
(https://pypi.org/project/pybind11/), both setuptools and distutils can get the
header files and use them to build C++ file that use pybind11.

There is an example for using setuptools to build pybind11
(https://github.com/pybind/python_example/blob/master/setup.py):

.. code-block:: python
  :linenos:

  from setuptools import setup, Extension
  from setuptools.command.build_ext import build_ext
  import sys
  import setuptools

  __version__ = '0.0.1'


  class get_pybind_include(object):
      """Helper class to determine the pybind11 include path
      The purpose of this class is to postpone importing pybind11
      until it is actually installed, so that the ``get_include()``
      method can be invoked. """

      def __init__(self, user=False):
          self.user = user

      def __str__(self):
          import pybind11
          return pybind11.get_include(self.user)


  ext_modules = [
      Extension(
          'python_example',
          ['src/main.cpp'],
          include_dirs=[
              # Path to pybind11 headers
              get_pybind_include(),
              get_pybind_include(user=True)
          ],
          language='c++'
      ),
  ]


  # As of Python 3.6, CCompiler has a `has_flag` method.
  # cf http://bugs.python.org/issue26689
  def has_flag(compiler, flagname):
      """Return a boolean indicating whether a flag name is supported on
      the specified compiler.
      """
      import tempfile
      with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
          f.write('int main (int argc, char **argv) { return 0; }')
          try:
              compiler.compile([f.name], extra_postargs=[flagname])
          except setuptools.distutils.errors.CompileError:
              return False
      return True


  def cpp_flag(compiler):
      """Return the -std=c++[11/14/17] compiler flag.
      The newer version is prefered over c++11 (when it is available).
      """
      flags = ['-std=c++17', '-std=c++14', '-std=c++11']

      for flag in flags:
          if has_flag(compiler, flag): return flag

      raise RuntimeError('Unsupported compiler -- at least C++11 support '
                         'is needed!')


  class BuildExt(build_ext):
      """A custom build extension for adding compiler-specific options."""
      c_opts = {
          'msvc': ['/EHsc'],
          'unix': [],
      }
      l_opts = {
          'msvc': [],
          'unix': [],
      }

      if sys.platform == 'darwin':
          darwin_opts = ['-stdlib=libc++', '-mmacosx-version-min=10.7']
          c_opts['unix'] += darwin_opts
          l_opts['unix'] += darwin_opts

      def build_extensions(self):
          ct = self.compiler.compiler_type
          opts = self.c_opts.get(ct, [])
          link_opts = self.l_opts.get(ct, [])
          if ct == 'unix':
              opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
              opts.append(cpp_flag(self.compiler))
              if has_flag(self.compiler, '-fvisibility=hidden'):
                  opts.append('-fvisibility=hidden')
          elif ct == 'msvc':
              opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())
          for ext in self.extensions:
              ext.extra_compile_args = opts
              ext.extra_link_args = link_opts
          build_ext.build_extensions(self)

  setup(
      name='python_example',
      version=__version__,
      author='Sylvain Corlay',
      author_email='sylvain.corlay@gmail.com',
      url='https://github.com/pybind/python_example',
      description='A test project using pybind11',
      long_description='',
      ext_modules=ext_modules,
      install_requires=['pybind11>=2.4'],
      setup_requires=['pybind11>=2.4'],
      cmdclass={'build_ext': BuildExt},
      zip_safe=False,
  )

Cmake with a Sub-Directory
++++++++++++++++++++++++++

When the source tree is put in a sub-directory in your project, as mentioned in
the `document
<https://pybind11.readthedocs.io/en/stable/compiling.html#building-with-cmake>`__,
you can use cmake ``add_subdirectory`` to include the pybind11:

.. code-block:: cmake

  cmake_minimum_required(VERSION 2.8.12)
  project(example)

  add_subdirectory(pybind11)
  pybind11_add_module(example example.cpp)

Pybind11 provides the cmake command ``pybind11_add_module``.  It set various
flags to build your C++ code as an extension module.

Cmake with installed pybind11
+++++++++++++++++++++++++++++

If pybind11 is installed using cmake itself, the ``*.cmake`` files that
pybind11 supplies are installed to the specified location.  It's not needed to
write ``add_subdirectory`` in the ``CMakeLists.txt`` in your project.

Additional Wrapping Layer for Customization
===========================================

Wrapper needs to take care of the differences between the dynamic behaviors in
Python and the staticity in C++.  You can directly call pybind11 API.  But a
better way is to create another wrapping layer between the pybind11 and your
library code.  It allows us to insert additional code in a systematic way.
Since it is difficult to see the point in a small example, I pull the code for
`a bigger project 'turgon' <https://github.com/yungyuc/turgon>`__ for
demonstration.

Here is one way to implement the additional wrapping layer:

.. code-block:: cpp
  :linenos:

  #pragma once

  /*
   * Copyright (c) 2017, Yung-Yu Chen <yyc@solvcon.net>
   * BSD 3-Clause License, see COPYING
   */

  #include <pybind11/pybind11.h>
  #include <pybind11/numpy.h>
  #define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
  #include <numpy/arrayobject.h>

  #include <memory>
  #include <type_traits>

  PYBIND11_DECLARE_HOLDER_TYPE(T, std::unique_ptr<T>);
  PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

  #ifdef __GNUG__
  #  define SPACETIME_PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))
  #else
  #  define SPACETIME_PYTHON_WRAPPER_VISIBILITY
  #endif

  namespace spacetime
  {

  namespace python
  {

  /**
   * Helper template for pybind11 class wrappers.
   */
  template< class Wrapper, class Wrapped, class Holder = std::unique_ptr<Wrapped>, class WrappedBase = Wrapped >
  class
  SPACETIME_PYTHON_WRAPPER_VISIBILITY
  WrapBase {

  public:

      using wrapper_type = Wrapper;
      using wrapped_type = Wrapped;
      using wrapped_base_type = WrappedBase;
      using holder_type = Holder;
      using base_type = WrapBase< wrapper_type, wrapped_type, holder_type, wrapped_base_type >;
      using class_ = typename std::conditional<
          std::is_same< Wrapped, WrappedBase >::value
        , pybind11::class_< wrapped_type, holder_type >
        , pybind11::class_< wrapped_type, wrapped_base_type, holder_type >
      >::type;

      static wrapper_type & commit(pybind11::module * mod, const char * pyname, const char * clsdoc) {
          static wrapper_type derived(mod, pyname, clsdoc);
          return derived;
      }

      WrapBase() = delete;
      WrapBase(WrapBase const & ) = default;
      WrapBase(WrapBase       &&) = delete;
      WrapBase & operator=(WrapBase const & ) = default;
      WrapBase & operator=(WrapBase       &&) = delete;
      ~WrapBase() = default;

  #define DECL_ST_PYBIND_CLASS_METHOD(METHOD) \
      template< class... Args > \
      /* NOLINTNEXTLINE(bugprone-macro-parentheses) */ \
      wrapper_type & METHOD(Args&&... args) { \
          m_cls.METHOD(std::forward<Args>(args)...); \
          return *static_cast<wrapper_type*>(this); \
      }

      DECL_ST_PYBIND_CLASS_METHOD(def)
      DECL_ST_PYBIND_CLASS_METHOD(def_readwrite)
      DECL_ST_PYBIND_CLASS_METHOD(def_property)
      DECL_ST_PYBIND_CLASS_METHOD(def_property_readonly)
      DECL_ST_PYBIND_CLASS_METHOD(def_property_readonly_static)

  #undef DECL_ST_PYBIND_CLASS_METHOD

  protected:

      WrapBase(pybind11::module * mod, const char * pyname, const char * clsdoc)
        : m_cls(*mod, pyname, clsdoc)
      {}

  private:

      class_ m_cls;

  }; /* end class WrapBase */

  } /* end namespace python */

  } /* end namespace spacetime */

The following classes ``WrapGrid``, ``WrapField``, ``WrapSolver``,
``WrapCelm``, and ``WrapSelm`` show how ``WrapBase`` is used:

.. code-block:: c++
  :linenos:

  #pragma once

  /*
   * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
   * BSD 3-Clause License, see COPYING
   */

  #include "spacetime/python/common.hpp"

  namespace spacetime
  {

  namespace python
  {

  class
  SPACETIME_PYTHON_WRAPPER_VISIBILITY
  WrapGrid
    : public WrapBase< WrapGrid, Grid, std::shared_ptr<Grid> >
  {

      friend base_type;

      WrapGrid(pybind11::module * mod, const char * pyname, const char * clsdoc)
        : base_type(mod, pyname, clsdoc)
      {
          namespace py = pybind11;
          (*this)
              .def(
                  py::init([](real_type xmin, real_type xmax, size_t nelm) {
                      return Grid::construct(xmin, xmax, nelm);
                  }),
                  py::arg("xmin"), py::arg("xmax"), py::arg("nelm")
              )
              .def(
                  py::init([](xt::pyarray<wrapped_type::value_type> & xloc) {
                      return Grid::construct(xloc);
                  }),
                  py::arg("xloc")
              )
              .def("__str__", &detail::to_str<wrapped_type>)
              .def_property_readonly("xmin", &wrapped_type::xmin)
              .def_property_readonly("xmax", &wrapped_type::xmax)
              .def_property_readonly("ncelm", &wrapped_type::ncelm)
              .def_property_readonly("nselm", &wrapped_type::nselm)
              .def_property_readonly(
                  "xcoord",
                  static_cast<wrapped_type::array_type & (wrapped_type::*)()>(&wrapped_type::xcoord)
              )
              .def_property_readonly_static("BOUND_COUNT", [](py::object const &){ return Grid::BOUND_COUNT; })
          ;
      }

  }; /* end class WrapGrid */

  class
  SPACETIME_PYTHON_WRAPPER_VISIBILITY
  WrapField
    : public WrapBase< WrapField, Field, std::shared_ptr<Field> >
  {

      friend base_type;

      WrapField(pybind11::module * mod, const char * pyname, const char * clsdoc)
        : base_type(mod, pyname, clsdoc)
      {
          namespace py = pybind11;
          (*this)
              .def("__str__", &detail::to_str<wrapped_type>)
              .def_property_readonly("grid", [](wrapped_type & self){ return self.grid().shared_from_this(); })
              .def_property_readonly("nvar", &wrapped_type::nvar)
              .def_property(
                  "time_increment",
                  &wrapped_type::time_increment,
                  &wrapped_type::set_time_increment
               )
              .def_property_readonly("dt", &wrapped_type::dt)
              .def_property_readonly("hdt", &wrapped_type::hdt)
              .def_property_readonly("qdt", &wrapped_type::qdt)
              .def(
                  "celm",
                  static_cast<Celm (wrapped_type::*)(sindex_type, bool)>(&wrapped_type::celm_at<Celm>),
                  py::arg("ielm"), py::arg("odd_plane")=false
              )
              .def(
                  "selm",
                  static_cast<Selm (wrapped_type::*)(sindex_type, bool)>(&wrapped_type::selm_at<Selm>),
                  py::arg("ielm"), py::arg("odd_plane")=false
              )
          ;
      }

  }; /* end class WrapField */

  class
  SPACETIME_PYTHON_WRAPPER_VISIBILITY
  WrapSolver
    : public WrapSolverBase< WrapSolver, Solver >
  {

      using base_type = WrapSolverBase< WrapSolver, Solver >;
      using wrapper_type = typename base_type::wrapper_type;
      using wrapped_type = typename base_type::wrapped_type;

      friend base_type;
      friend base_type::base_type;

      WrapSolver(pybind11::module * mod, const char * pyname, const char * clsdoc)
        : base_type(mod, pyname, clsdoc)
      {
          namespace py = pybind11;
          (*this)
              .def
              (
                  py::init(static_cast<std::shared_ptr<wrapped_type> (*) (
                      std::shared_ptr<Grid> const &, typename wrapped_type::value_type, size_t
                  )>(&wrapped_type::construct))
                , py::arg("grid"), py::arg("time_increment"), py::arg("nvar")
              )
          ;
      }

  }; /* end class WrapSolver */

  class
  SPACETIME_PYTHON_WRAPPER_VISIBILITY
  WrapCelm
    : public WrapCelmBase< WrapCelm, Celm >
  {

      using base_type = WrapCelmBase< WrapCelm, Celm >;
      friend base_type::base_type::base_type;

      WrapCelm(pybind11::module * mod, const char * pyname, const char * clsdoc)
        : base_type(mod, pyname, clsdoc)
      {}

  }; /* end class WrapCelm */

  class
  SPACETIME_PYTHON_WRAPPER_VISIBILITY
  WrapSelm
    : public WrapSelmBase< WrapSelm, Selm >
  {

      using base_type = WrapSelmBase< WrapSelm, Selm >;
      friend base_type::base_type::base_type;

      WrapSelm(pybind11::module * mod, const char * pyname, const char * clsdoc)
        : base_type(mod, pyname, clsdoc)
      {}

  }; /* end class WrapSelm */

  } /* end namespace python */

  } /* end namespace spacetime */

  // vim: set et sw=4 ts=4:


Note that the use of ``WrapperBase`` saves little duplicated code, if any.  But
it facilitates to keep the ``.cpp`` file for building the Python extension to
be very short:

.. code-block:: cpp
  :linenos:

  /*
   * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
   * BSD 3-Clause License, see COPYING
   */

  #include "spacetime/python.hpp" // must be first

  #include "spacetime.hpp"

  #include <algorithm>
  #include <cstring>
  #include <memory>
  #include <utility>
  #include <vector>

  namespace
  {

  PyObject * initialize_spacetime(pybind11::module * mod)
  {
      namespace spy = spacetime::python;
      xt::import_numpy(); // otherwise numpy c api segfault.
      mod->doc() = "_libst: One-dimensional space-time CESE method code";
      spy::WrapGrid::commit(mod, "Grid", "Spatial grid data");
      spy::WrapField::commit(mod, "Field", "Solution data");
      return mod->ptr();
  }

  } /* end namespace */

  PYBIND11_MODULE(_libst, mod) // NOLINT
  {
      namespace spy = spacetime::python;
      spy::ModuleInitializer::get_instance()
          .add(initialize_spacetime)
          .add_solver<spy::WrapSolver, spy::WrapCelm, spy::WrapSelm>
          (&mod, "", "no equation")
          .add_solver<spy::WrapLinearScalarSolver, spy::WrapLinearScalarCelm, spy::WrapLinearScalarSelm>
          (&mod, "LinearScalar", "a linear scalar equation")
          .add_solver<spy::WrapInviscidBurgersSolver, spy::WrapInviscidBurgersCelm, spy::WrapInviscidBurgersSelm>
          (&mod, "InviscidBurgers", "the inviscid Burgers equation")
          .initialize(&mod)
      ;
  }

Wrapping API
============

Let's take a look at the helpers that pybind11 provides.

Function and Property
+++++++++++++++++++++

Let's use the `Grid` class as an example to demonstrate how to expose functions and properties.  We have a constructor:

.. code-block:: cpp

  .def
  (
      py::init
      (
          [](real_type xmin, real_type xmax, size_t nelm)
          {
              return Grid::construct(xmin, xmax, nelm);
          }
      )
    , py::arg("xmin"), py::arg("xmax"), py::arg("nelm")
  )

and a special-purpose function:

.. code-block:: cpp

  .def("__str__", &detail::to_str<wrapped_type>)

.. code-block:: pycon

  >>> grid = libst.Grid(0, 8, 4*64)
  >>> print('directly call Grid.__str__():', grid.__str__())
  directly call Grid.__str__(): Grid(xmin=0, xmax=8, ncelm=256)
  >>> print('call str(Grid):', str(grid))
  call str(Grid): Grid(xmin=0, xmax=8, ncelm=256)

Sometimes properties are more suitable for certain getters:

.. code-block:: cpp

  .def_property_readonly("xmin", &wrapped_type::xmin)
  .def_property_readonly("xmax", &wrapped_type::xmax)
  .def_property_readonly_static("BOUND_COUNT", [](py::object const &){ return Grid::BOUND_COUNT; })

As shown above, pybind11 supports static properties that are associated on the class instead of the instance.

.. code-block:: pycon

  >>> print('grid.BOUND_COUNT =', grid.BOUND_COUNT)
  grid.BOUND_COUNT = 2
  >>> print('grid.xmin =', grid.xmin)
  grid.xmin = 0.0
  >>> print('grid.xmax =', grid.xmax)
  grid.xmax = 8.0

  >>> print('Grid.BOUND_COUNT (number of points beyond spatial boundary) =', libst.Grid.BOUND_COUNT)
  Grid.BOUND_COUNT (number of points beyond spatial boundary) = 2
  >>> print('Grid.xmin =', libst.Grid.xmin)
  Grid.xmin = <property object at 0x110e9ffb0>
  >>> print('Grid.xmax =', libst.Grid.xmax)
  Grid.xmax = <property object at 0x110ea60b0>

Compare to pure Python object:

.. code-block:: python

  class PythonGrid:
      BOUND_COUNT = 2
      @property
      def xmin(self):
          return 0
      @property
      def xmax(self):
          return 8

.. code-block:: python

  >>> print(PythonGrid.BOUND_COUNT)
  2
  >>> print(PythonGrid.xmin)
  <property object at 0x1112daad0>
  >>> print(PythonGrid.xmax)
  <property object at 0x1112dab30>

In addition to ``def_property_readonly`` and ``def_property_readonly_static``,
pybind11 also provides:

* ``def_property`` and ``def_property_static`` for read/write properties with
  C++ accessors.
* ``def_readonly`` and ``def_readonly_static`` for read-only access to C++ data
  members.
* ``def_readwrite`` and ``def_readwrite_static`` for read/write access to C++
  data members.

Named and Keyword Arguments
+++++++++++++++++++++++++++

Pybind11 allows named arguments.  I take the advantage for wrapping the
constructor of ``Grid``:

.. code-block:: cpp

  .def
  (
      py::init
      (
          [](real_type xmin, real_type xmax, size_t nelm)
          {
              return Grid::construct(xmin, xmax, nelm);
          }
      )
    , py::arg("xmin"), py::arg("xmax"), py::arg("nelm")
  )

and a special-purpose function:

.. code-block:: cpp

  .def("__str__", &detail::to_str<wrapped_type>)

.. code-block:: pycon

  >>> grid = libst.Grid(xmin=0, xmax=8, nelm=4*64)
  >>> print('directly call Grid.__str__():', grid.__str__())
  directly call Grid.__str__(): Grid(xmin=0, xmax=8, ncelm=256)
  >>> print('call str(Grid):', str(grid))
  call str(Grid): Grid(xmin=0, xmax=8, ncelm=256)

``pybind11::arg`` also allows default value to the arguments (keyword
arguments).  For example, see what I have for all the solver classes:

.. code-block:: cpp

  .def
  (
      "selms"
    , [](wrapped_type & self, bool odd_plane)
      { return elm_iter_type(self.shared_from_this(), odd_plane, 0, true); }
    , py::arg("odd_plane")=false
  )

.. code-block:: python

  grid = libst.Grid(0, 4*2*np.pi, 4*64)
  cfl = 1
  dx = (grid.xmax - grid.xmin) / grid.ncelm
  dt = dx * cfl
  svr = libst.LinearScalarSolver(grid=grid, time_increment=dt)

.. code-block:: pycon

  >>> print(svr.selms())
  SolverElementIterator(selm, on_even_plane, current=0, nelem=257)
  >>> print(svr.selms(False))
  SolverElementIterator(selm, on_even_plane, current=0, nelem=257)
  >>> print(svr.selms(True))
  SolverElementIterator(selm, on_odd_plane, current=0, nelem=256)
  >>> print(svr.selms(odd_plane=False))
  SolverElementIterator(selm, on_even_plane, current=0, nelem=257)
  >>> print(svr.selms(odd_plane=True))
  SolverElementIterator(selm, on_odd_plane, current=0, nelem=256)

What Happens in Python Stays in Python (or Pybind11)
++++++++++++++++++++++++++++++++++++++++++++++++++++

When wrapping from C++ to Python, there are constructs only available in the
scripting language but not the low-level implementation.  When it happens,
write the adapting code in the pybind11 layer and do not pollute the low-level
implementation.

One example is Python iterator protocol.  The adapting code is:

.. code-block:: cpp
  :linenos:

  template< typename ST >
  class SolverElementIterator
  {

  public:

      using solver_type = ST;

      SolverElementIterator() = delete;
      SolverElementIterator(std::shared_ptr<ST> sol, bool odd_plane, size_t starting, bool selm)
        : m_solver(std::move(sol)), m_odd_plane(odd_plane), m_current(starting), m_selm(selm)
      {}

      typename ST::celm_type next_celm()
      {
          size_t ncelm = m_solver->grid().ncelm();
          if (m_odd_plane) { --ncelm; }
          if (m_current >= ncelm) { throw pybind11::stop_iteration(); }
          typename ST::celm_type ret = m_solver->celm(m_current, m_odd_plane);
          ++m_current;
          return ret;
      }

      typename ST::selm_type next_selm()
      {
          size_t nselm = m_solver->grid().nselm();
          if (m_odd_plane) { --nselm; }
          if (m_current >= nselm) { throw pybind11::stop_iteration(); }
          typename ST::selm_type ret = m_solver->selm(m_current, m_odd_plane);
          ++m_current;
          return ret;
      }

      bool is_selm() const { return m_selm; }
      bool on_odd_plane() const { return m_odd_plane; }
      size_t current() const { return m_current; }
      size_t nelem() const
      {
          size_t ret = is_selm() ? m_solver->grid().nselm() : m_solver->grid().ncelm();
          if (m_odd_plane) { --ret; }
          return ret;
      }

  private:

      std::shared_ptr<solver_type> m_solver;
      bool m_odd_plane;
      size_t m_current = 0;
      bool m_selm = false;

  }; /* end class SolverElementIterator */

The wrapping code is:

.. code-block:: cpp

  using elm_iter_type = SolverElementIterator<wrapped_type>;
  std::string elm_pyname = std::string(pyname) + "ElementIterator";
  pybind11::class_< elm_iter_type >(*mod, elm_pyname.c_str())
      .def("__str__", &detail::to_str<elm_iter_type>)
      .def("__iter__", [](elm_iter_type & self){ return self; })
      .def(
          "__next__"
        , [](elm_iter_type & self)
          {
              py::object ret;
              if (self.is_selm()) { ret = py::cast(self.next_selm()); }
              else                { ret = py::cast(self.next_celm()); }
              return ret;
          }
      )
  ;

Let's use a concrete solver of linear wave (governing equation is :math:`u_t +
u_x = 0`) to demonstrate how it works in Python:

.. literalinclude:: code/04_iter.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

.. figure:: image/04_iter.png
  :align: center
  :width: 100%

Manipulate Python Objects in C++
================================

Pybind11 provides C++ API for manipulating Python object (the C ``struct``
:c:type:`python:PyObject`), so that we don't need to dig into the Python C API
and worry about the reference counting by hand.

The first example is to create a ``None`` object from C++.

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code_none, m)
  {
      namespace py = pybind11;

      m
          .def
          (
              "create_none", []() { return py::none(); }
          )
      ;
  }

.. code-block:: pycon

  >>> print(type(create_none()))
  <class 'NoneType'>
  >>> assert None is create_none()
  >>> print(create_none())
  None

``pybind11::object`` is the C++ counterpart of :c:type:`python:PyObject` in C,
but automatically does reference counting for us.

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code_object, m)
  {
      namespace py = pybind11;

      m
          .def
          (
              "return_none"
            , []()
              {
                  py::object ret = py::none();
                  return ret;
              }
          )
      ;
  }

.. code-block:: pycon

  >>> print(return_none, return_none())
  <built-in method return_none of PyCapsule object at 0x1111b4300> None

Pybind11 allows to use ``pybind11::object::attr`` to assign attribute to a
Python object.

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code_attr, m)
  {
      namespace py = pybind11;

      m.attr("string_name") = "string_content";
  }

.. code-block:: pycon

  >>> print(type(string_name), string_name)
  <class 'str'> string_content

Pybind11 provides helpers to import Python module and access attibutes of every
Python object, including a Python module.

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code1, m)
  {
      namespace py = pybind11;

      m
          .def
          (
              "return_numpy_version"
            , []()
              {
                  py::object numpy = py::module::import("numpy");
                  return numpy.attr("__version__");
              }
          )
      ;

      m.attr("alias_to_return_numpy_version") = m.attr("return_numpy_version");
  }

.. code-block:: pycon

  >>> print(return_numpy_version())
  1.17.0.dev0+3c3ba10
  >>> import numpy as np
  >>> print(np.__version__)
  1.17.0.dev0+3c3ba10
  >>> assert np.__version__ is return_numpy_version()
  >>> print(return_numpy_version)
  <built-in method return_numpy_version of PyCapsule object at 0x1111b4060>
  >>> print(alias_to_return_numpy_version)
  <built-in method return_numpy_version of PyCapsule object at 0x1111b4060>

Python Containers
=================

Pybind11 provides C++ API for creating and manipulating the most important
Python containers: :py:class:`python:tuple`, :py:class:`python:list`, and
:py:class:`python:dict`.  See
https://pybind11.readthedocs.io/en/stable/advanced/pycpp/object.html and the
unit tests for more information.

:py:class:`python:tuple`
++++++++++++++++++++++++

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code_tuple, m)
  {
      namespace py = pybind11;

      py::tuple my_tuple = py::make_tuple("string_data_in_tuple", 10, 3.1415926);

      m.attr("my_tuple") = my_tuple;
  }

.. code-block:: pycon

  >>> print(type(my_tuple), my_tuple)
  <class 'tuple'> ('string_data_in_tuple', 10, 3.1415926)

:py:class:`python:list`
+++++++++++++++++++++++

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code2, m)
  {
      namespace py = pybind11;

      py::list my_list = py::list();

      my_list.append("string_data_in_list");
      my_list.append(11);
      my_list.append(2.71828);

      py::list my_list2 = py::make_tuple("string_data_in_list2", 12);

      m.attr("my_list") = my_list;
      m.attr("my_list2") = my_list2;
  }

.. code-block:: pycon

  >>> print(type(my_list), my_list)
  <class 'list'> ['string_data_in_list', 11, 2.71828]

  >>> print(type(my_list2), my_list2)
  <class 'list'> ['string_data_in_list2', 12]

:py:class:`python:dict`
+++++++++++++++++++++++

.. code-block:: cpp

  #include "pybind11/pybind11.h"

  PYBIND11_MODULE(code2, m)
  {
      namespace py = pybind11;

      py::dict my_dict;

      my_dict["key_string"] = "string_data_in_dict";
      my_dict["key_int"] = 13;
      my_dict["key_real"] = 1.414;

      m.attr("my_dict") = my_dict;
  }

.. code-block:: pycon

  >>> print(type(my_dict), my_dict)
  <class 'dict'> {'key_string': 'string_data_in_dict', 'key_int': 13, 'key_real': 1.414}

Use CPython API with Pybind11
=============================

We can use any Python C API with pybind11.

When we import ``pybind11/pybind11.h``, we don't need to import ``Python.h``,
becuase the former does it for us.  But please note that
``pybind11/pybind11.h`` or ``Python.h`` should be included before every other
inclusion.

.. code-block:: cpp

  #include "pybind11/pybind11.h"
  #include "Python.h" // Unnecessary

  using namespace pybind11;

  PYBIND11_MODULE(ex_long, m)
  {
      PyObject * v = PyLong_FromLong(2000000);
      m.attr("integer_value") = v;
      Py_DECREF(v);
  }

.. code-block:: pycon

  >>> print(type(integer_value), integer_value)
  <class 'int'> 2000000

:c:type:`python:PyObject` reference counting
++++++++++++++++++++++++++++++++++++++++++++

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  using namespace pybind11;

  static PyObject * s;

  PYBIND11_MODULE(ex_str, m)
  {
      s = PyUnicode_FromString("string_from_c");
      m.attr("string_value") = s;
      Py_DECREF(s);
      m
          .def
          (
              "show_ref_count_with_handle"
            , [](handle const & h)
              {
                  return Py_REFCNT(h.ptr());
              }
          )
          .def
          (
              "show_ref_count_with_object"
            , [](object const & o)
              {
                  return Py_REFCNT(o.ptr());
              }
          )
          .def
          (
              "show_string_value_ref_count"
            , [&]()
              {
                  return Py_REFCNT(s);
              }
          )
      ;
  }

.. code-block:: python
  :linenos:

  def check_string_value():
      print(type(string_value), string_value)
      print('before aliasing')
      print(show_ref_count_with_object(string_value), 'refcnt by object')
      print(show_ref_count_with_handle(string_value), 'refcnt by handle')
      print(sys.getrefcount(string_value), 'refcnt by sys')
      print(show_string_value_ref_count(), 'refcnt from c++')
      string_value_aliasing = string_value
      print('after aliasing')
      print(show_ref_count_with_object(string_value), 'refcnt by object')
      print(show_ref_count_with_handle(string_value), 'refcnt by handle')
      print(sys.getrefcount(string_value), 'refcnt by sys')
      print(show_string_value_ref_count(), 'refcnt from c++')

.. code-block:: pycon

  >>> check_string_value()
  <class 'str'> string_from_c
  before aliasing
  7 refcnt by object
  6 refcnt by handle
  5 refcnt by sys
  4 refcnt from c++
  after aliasing
  8 refcnt by object
  7 refcnt by handle
  6 refcnt by sys
  5 refcnt from c++

Pybind11 offers two low-level shorthands for reference counting:
``handle::inc_ref()`` and ``handle::dec_ref()``.  If we don't want to go so
low-level, it provides ``reinterpret_borrow`` and ``reinterpret_steal``
function templates.

Built-in Types
==============

It is possible to use Python C API along with pybind11.  I am going to should
how to do it.  Please keep in mind that the examples in the notes omit a lot of
error checking code that is necessary for a system to run correctly.  When you
need to use the C API, consult the manual: :doc:`python:c-api/index`.

Cached Value
++++++++++++

Python caches small (-5 to 256) integers (see `the code
<https://github.com/python/cpython/blob/4830f581af57dd305c02c1fd72299ecb5b090eca/Objects/longobject.c#L40>`__).
Don't get surprised when you see a large reference count for some of them
integers:

.. code-block:: pycon

  >>> print('ref counts of 0:', sys.getrefcount(0))
  ref counts of 0: 10198
  >>> print('ref counts of 257:', sys.getrefcount(257))
  ref counts of 257: 3

Real number doesn't have that cache:

.. code-block:: pycon

  >>> print(sys.getrefcount(0.0))
  3

Python interns strings consisting of alphanumerical and underscore characters.

.. code-block:: pycon

  >>> print('' is '')
  True
  >>> print(sys.getrefcount(''))
  5552

.. code-block:: python

  def check_string_intern():
      s1 = 'numerical'
      print(sys.getrefcount('numerical'))
      print(s1 is 'numerical')
      s2 = 'num' + 'erical'
      print(s1 is s2)
      print(sys.getrefcount('numerical'))

.. code-block:: pycon

  >>> check_string_intern()
  4
  True
  True
  5

Attribute Access
++++++++++++++++

The Python object protocol defines a set of API for accessing object attributes.  Here is a simple example that sets and gets an attribute of an object using the API:

.. code-block:: cpp

  int PyObject_SetAttr(PyObject *o, PyObject *attr_name, PyObject *v);
  PyObject* PyObject_GetAttr(PyObject *o, PyObject *attr_name);

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  using namespace pybind11;

  void attach_attr(PyObject * o, PyObject * name, PyObject * attr)
  {
      /*int ret =*/
      PyObject_SetAttr(o, name, attr);
  }

  PyObject * retrieve_attr(PyObject * o, PyObject * name)
  {
      PyObject * ret = PyObject_GetAttr(o, name);
      return ret;
  }

  PYBIND11_MODULE(ex_attr, m)
  {
      m
          .def
          (
              "attach_attr"
            , [](object & o, object & name, object & attr)
              {
                  attach_attr(o.ptr(), name.ptr(), attr.ptr());
              }
          )
          .def
          (
              "retrieve_attr"
            , [](object & o, object & name)
              {
                  handle(retrieve_attr(o.ptr(), name.ptr()));
              }
          )
      ;
  }

.. code-block:: pycon
  :linenos:

  >>> class Cls():
  >>>     pass
  >>> obj = Cls()
  >>> val = 'attached value'
  >>> print(sys.getrefcount(val))
  3
  >>>
  >>> attach_attr(obj, 'name', val)
  >>> print(sys.getrefcount(val))
  4
  >>>
  >>> print(obj.name is val)
  True
  >>> print(sys.getrefcount(val))
  4
  >>>
  >>> val2 = retrieve_attr(obj, 'name')
  >>> print(sys.getrefcount(val))
  5

There are shorthand versions of the API that takes C string for the attribute
name:

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  void attach_attr(PyObject * o, char const * name, PyObject * attr)
  {
      /*int ret =*/
      PyObject_SetAttrString(o, name, attr);
  }

  PyObject * retrieve_attr(PyObject * o, char const * name)
  {
      PyObject * ret = PyObject_GetAttrString(o, name);
      return ret;
  }

  PYBIND11_MODULE(ex_attr_by_string, m)
  {
      m
          .def
          (
              "attach_attr_by_string"
            , [](object & o, object & name, object & attr)
              {
                  std::string name_str = cast<std::string>(name);
                  attach_attr(o.ptr(), name_str.c_str(), attr.ptr());
              }
          )
          .def
          (
              "retrieve_attr_by_string"
            , [](object & o, object & name)
              {
                  std::string name_str = cast<std::string>(name);
                  handle(retrieve_attr(o.ptr(), name_str.c_str()));
              }
          )
      ;
  }

.. code-block:: pycon
  :linenos:

  >>> class Cls():
  >>>     pass
  >>> obj = Cls()
  >>> val = 'attached value'
  >>> print(sys.getrefcount(val))
  3
  >>>
  >>> attach_attr_by_string(obj, 'name', val)
  >>> print(sys.getrefcount(val))
  4
  >>>
  >>> print(obj.name is val)
  True
  >>> print(sys.getrefcount(val))
  4
  >>>
  >>> val2 = retrieve_attr_by_string(obj, 'name')
  >>> print(sys.getrefcount(val))
  5

See also the documentation of :doc:`python:c-api/object`.

Function Call
+++++++++++++

This section shows how to make Python function call from C.

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  PyObject * function_call(PyObject * callable, PyObject * args, PyObject * kw)
  {
      PyObject * ret = PyObject_Call(callable, args, kw);
      return ret;
  }

  PYBIND11_MODULE(ex_call, m)
  {
      m
          .def
          (
              "function_call"
            , [](object & o, tuple & t, dict & kw)
              {
                  return handle(function_call(o.ptr(), t.ptr(), kw.ptr()));
              }
          )
      ;
  }

.. code-block:: pycon
  :linenos:

  >>> def my_func(arg1, kw1='default'):
  >>>     return 'results: {}, {}'.format(arg1, kw1)
  >>>
  >>> print('(direct call)  ', my_func('first argument'))
  (direct call)   results: first argument, default
  >>> print('(function_call)', function_call(my_func, ('first argument',), {}))
  (function_call) results: first argument, default
  >>>
  >>> print('(direct call)  ', my_func('first argument', kw1='non default'))
  (direct call)   results: first argument, non default
  >>> print('(function_call)', function_call(my_func, ('first argument',), {'kw1': 'non default'}))
  (function_call) results: first argument, non default

See the documentation of :doc:`python:c-api/object` for other variants of the API.

Python C API for :py:class:`python:tuple`
+++++++++++++++++++++++++++++++++++++++++

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  PyObject * reverse_tuple(PyObject * tup)
  {
      PyObject * ret = PyTuple_New(PyTuple_Size(tup));

      for (Py_ssize_t i = 0 ; i < PyTuple_Size(tup) ; ++i)
      {
          PyObject * item = PyTuple_GetItem(tup, i);
          Py_INCREF(item);
          PyTuple_SetItem(ret, i, item); // This only works when 1 == Py_REFCNT(ret)
      }

      return ret;
  }

  PYBIND11_MODULE(ex_tuple, m)
  {
      m
          .def
          (
              "reverse_tuple"
            , [](tuple & t)
              {
                  return handle(reverse_tuple(t.ptr()));
              }
          )
      ;
  }

.. code-block:: pycon

  >>> tv0 = "value0"
  >>> tv1 = object()
  >>> tup = (tv0, tv1)
  >>> print(sys.getrefcount(tv1))
  3
  >>> rtup = reverse_tuple(tup)
  >>> print(sys.getrefcount(tv1))
  4

See :ref:`the C API documentation for the tuple protocol <python:tupleobjects>`
and the `code implementing
<https://github.com/python/cpython/blob/v3.8.0/Objects/tupleobject.c#L167>`__
:c:func:`python:PyTuple_SetItem`.

Python C API for :py:class:`python:dict`
++++++++++++++++++++++++++++++++++++++++

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  PyObject * make_dict()
  {
      PyObject * ret = PyDict_New();
      return ret;
  }

  void add_dict_item(PyObject * d, PyObject * k, PyObject * v)
  {
      /*int ret =*/
      PyDict_SetItem(d, k, v);
  }

  PYBIND11_MODULE(ex_dict, m)
  {
      m
          .def
          (
              "make_dict"
            , []()
              {
                  return handle(make_dict());
              }
          )
          .def
          (
              "add_dict_item"
            , [](dict & d, object & k, object & v)
              {
                  add_dict_item(d.ptr(), k.ptr(), v.ptr());
              }
          )
      ;
  }

.. code-block:: pycon

  >>> d0 = {}
  >>> d1 = make_dict()
  >>> print(d0 is d1)
  False
  >>> print(d0 == d1)
  True
  >>> d0['k1'] = 'value1'
  >>> print(d0)
  {'k1': 'value1'}
  >>> add_dict_item(d1, 'k1', 'value1')
  >>> print(d1)
  {'k1': 'value1'}
  >>> print(d0 == d1)
  True

See :ref:`the C API documentation for the dict protocol <python:dictobjects>`.

Python C API for :py:class:`python:list`
++++++++++++++++++++++++++++++++++++++++

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  PyObject * make_list_from_iterator(PyObject * o)
  {
      PyObject * iter = PyObject_GetIter(o);
      PyObject * ret = PyList_New(0);
      PyObject * item = nullptr;
      while (nullptr != (item = PyIter_Next(iter)))
      {
          PyList_Append(ret, item);
          Py_DECREF(item);
      }
      Py_DECREF(iter);
      return ret;
  }

  PYBIND11_MODULE(ex_list, m)
  {
      m
          .def
          (
              "make_list_from_iterator"
            , [](object & o)
              {
                  PyObject * ret = make_list_from_iterator(o.ptr());
                  return handle(ret);
              }
          )
      ;
  }

.. code-block:: pycon
  :linenos:

  >>> v0 = 'first value'
  >>> v1 = 'second value'
  >>> tup = (v0, v1)
  >>> print(sys.getrefcount(v1))
  4
  >>> lst = make_list_from_iterator(tup)
  >>> print(sys.getrefcount(v1))
  5
  >>> print(lst)
  ['first value', 'second value']

See :ref:`the C API documentation for the list protocol <python:listobjects>`
and :ref:`the C API documentation for the iterator protocol <python:iterator>`.

Useful Operations
=================

Import
++++++

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  PyObject * get_modules()
  {
      PyObject * sysmod = PyImport_ImportModule("sys");
      PyObject * modules = PyObject_GetAttrString(sysmod, "modules");
      Py_DECREF(sysmod);
      return modules;
  }

  PYBIND11_MODULE(ex_import, m)
  {
      m
          .def
          (
              "get_modules"
            , []()
              {
                  PyObject * ret = get_modules();
                  return handle(ret);
              }
          )
      ;
  }

.. code-block:: pycon

  >>> modules = get_modules();
  >>> print(type(modules), len(modules))
  <class 'dict'> 1146

Exception
+++++++++

.. code-block:: cpp
  :linenos:

  #include "pybind11/pybind11.h"

  #include <string>

  using namespace pybind11;

  PyObject * function_with_exception(PyObject * o)
  {
      // Normal processing code.
      PyObject * ret = nullptr;
      PyObject * item = nullptr;
      PyObject * iter = PyObject_GetIter(o);
      if (nullptr == iter) { goto error; }
      ret = PyList_New(0);
      if (nullptr == ret) { goto error; }
      while (nullptr != (item = PyIter_Next(iter)))
      {
          int status = PyList_Append(ret, item);
          Py_DECREF(item);
          if (0 != status) { goto error; }
      }
      Py_DECREF(iter);

      // Exception.
      PyErr_SetString(PyExc_RuntimeError, "intentional exception");

  error: // A good use of goto: clean up.
      Py_XDECREF(iter);
      Py_XDECREF(ret);
      return nullptr;
  }

  PYBIND11_MODULE(ex_except, m)
  {
      m
          .def
          (
              "function_with_exception"
            , [](object & o)
              {
                  PyObject * ret = function_with_exception(o.ptr());
                  if (nullptr == ret) { throw error_already_set(); }
                  return handle(ret);
              }
          )
      ;
  }

.. code-block:: pycon

  >>> try:
  >>>     function_with_exception(1)
  >>> except TypeError as e:
  >>>     print(e)
  >>> else:
  >>>     print('error not raised')
  'int' object is not iterable

.. code-block:: pycon

  >>> tup = ('first value', 'second value')
  >>> try:
  >>>     function_with_exception(('first value', 'second value'))
  >>> except RuntimeError as e:
  >>>     print(e)
  >>> else:
  >>>     print('error not raised')
  intentional exception

See also :ref:`python:api-exceptions` and :doc:`python:c-api/exceptions`.

Python Memory Management
========================

Python has its own memory manager.  When writing Python extension, they should
be used for :c:type:`python:PyObject`.  The memory managing system has three levels:

1. Raw memory interface: wrapper to the C standard memory managers.  It allows
  distinct addressed returned when requesting 0 byte.  GIL is not involved.
2. Normal memory interface: 'pymalloc' with small memory optimization.  GIL is
  required when calling.
3. Object memory interface: allocate for :c:type:`python:PyObject`.  GIL is
  required when calling.

The public API are:

.. code-block:: c

  void * PyMem_RawMalloc(size_t size);
  void * PyMem_RawCalloc(size_t nelem, size_t elsize);
  void * PyMem_RawRealloc(void *ptr, size_t new_size);
  void   PyMem_RawFree(void *ptr);

  void * PyMem_Malloc(size_t size);
  void * PyMem_Calloc(size_t nelem, size_t elsize);
  void * PyMem_Realloc(void *ptr, size_t new_size);
  void   PyMem_Free(void *ptr);

  void * PyObject_Malloc(size_t size);
  void * PyObject_Calloc(size_t nelem, size_t elsize);
  void * PyObject_Realloc(void *ptr, size_t new_size);
  void   PyObject_Free(void *ptr);

In
[`Include/cpython/pymem.h`](https://github.com/python/cpython/blob/v3.8.0/Include/cpython/pymem.h#L53),
Python provides a struct and a set of API to switch to custom memory managers:

.. code-block:: c

  typedef struct {
      /* user context passed as the first argument to the 4 functions */
      void *ctx;

      /* allocate a memory block */
      void* (*malloc) (void *ctx, size_t size);

      /* allocate a memory block initialized by zeros */
      void* (*calloc) (void *ctx, size_t nelem, size_t elsize);

      /* allocate or resize a memory block */
      void* (*realloc) (void *ctx, void *ptr, size_t new_size);

      /* release a memory block */
      void (*free) (void *ctx, void *ptr);
  } PyMemAllocatorEx;

  /* Get the memory block allocator of the specified domain. */
  PyAPI_FUNC(void) PyMem_GetAllocator(PyMemAllocatorDomain domain,
                                      PyMemAllocatorEx *allocator);

  /* Set the memory block allocator of the specified domain.

     The new allocator must return a distinct non-NULL pointer when requesting
     zero bytes.

     For the PYMEM_DOMAIN_RAW domain, the allocator must be thread-safe: the GIL
     is not held when the allocator is called.

     If the new allocator is not a hook (don't call the previous allocator), the
     PyMem_SetupDebugHooks() function must be called to reinstall the debug hooks
     on top on the new allocator. */
  PyAPI_FUNC(void) PyMem_SetAllocator(PyMemAllocatorDomain domain,
                                      PyMemAllocatorEx *allocator);

See the official documentation :ref:`python:customize-memory-allocators`.  The
public API is wrappers to the functions populated in the struct, e.g.:

.. code-block:: c

  void *
  PyMem_RawMalloc(size_t size)
  {
      /*
       * Limit ourselves to PY_SSIZE_T_MAX bytes to prevent security holes.
       * Most python internals blindly use a signed Py_ssize_t to track
       * things without checking for overflows or negatives.
       * As size_t is unsigned, checking for size < 0 is not required.
       */
      if (size > (size_t)PY_SSIZE_T_MAX)
          return NULL;
      return _PyMem_Raw.malloc(_PyMem_Raw.ctx, size);
  }

Also see `the code
<https://github.com/python/cpython/blob/v3.8.0/Objects/obmalloc.c#L562>`__.

Small Memory Optimization
+++++++++++++++++++++++++

Take a look at the documentation in `the
code<https://github.com/python/cpython/blob/v3.8.0/Objects/obmalloc.c#L766>`__.
This is the 'pymalloc', and it uses 256 KB for allocation not greater than 512
bytes.

Tracemalloc
+++++++++++

Tracemalloc is a standard library that uses the custom memory manager to
profile and debug Python memory use: :doc:`python:library/tracemalloc`.  We can
follow the implementation to create more specific analysis.

Exercises
=========

1. Use pybind11 to expose a memory buffer to Python as a numpy ndarray.

References
==========

.. [1] Project turgon (work in progress): https://github.com/yungyuc/turgon.

.. [2] S.C. Chang, "The Method of Space-Time Conservation Element and Solution
   Element -- A New Approach for Solving the Navier-Stokes and Euler
   Equations," J. Comput. Phys., 119, pp. 295-324, (1995).  DOI:
   `10.1006/jcph.1995.1137 <https://doi.org/10.1006/jcph.1995.1137>`__

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2: