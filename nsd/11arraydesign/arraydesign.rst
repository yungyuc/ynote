=================
Array Code in C++
=================

.. contents:: Contents in the chapter
  :local:
  :depth: 1

As we have learned in :doc:`../04matrix/matrix`, :doc:`../05cache/cache`, and
:doc:`../06simd/simd`, fast code calls for regular access to compact data.  Not
all data structures offer fast runtime.  For writing fast code, arrays are the
simplest and most commonly used data structure.  We will discuss how to use
arrays to achieve high speed.

Python Is Slow
==============

.. contents:: Contents in the section
  :local:
  :depth: 1

When using Python for number-crunching, the fact of its slowness needs to be
kept in mind always.  Handling the slowness is the key to make Python run as
fast as C.

Here we consider a boundary value problem of the Laplace equation for
temperature distribution in a :math:`1\times1` square area.

.. math::

  & \frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2}
    = 0 \quad (0<x<1; 0<y<1) \\
  &\left\{\begin{array}{lll}
    u(0,y) = 0, & u(1,y) = \sin(\pi y) & \rule{4ex}{0pt} (0 \le y \le 1) \\
    u(x,0) = 0, & u(x,1) = 0 & \rule{4ex}{0pt} (0 \le x \le 1)
  \end{array}\right.

.. _nsd-arraydesign-python:

Laplace Equation in Python
++++++++++++++++++++++++++

To solve it numerically, we choose the finite-difference method.  The
finite-difference method needs a grid to discretize the spatial domain.  The
simplest spatial discretization is the homogeneous Cartesian grid.  Let's make
a :math:`51\times51` Cartesian grid.  The Python code is:

.. literalinclude:: code/01_grid.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

The full code is in :ref:`01_grid.py <nsd-arraydesign-example-grid>`.  Plot the
grid using matplotlib:

.. figure:: image/01_grid.png
  :align: center
  :width: 100%

  A Cartesian grid of :math:`51\times51` grid points.

After the grid is defined, we derive the finite-differencing formula.  Use the
Taylor series expansion to obtain the difference equation:

.. math::

  & \frac{u(x_{i+1}, y_j) - 2u(x_i, y_j) + u(x_{i-1}, y_j)}{(\Delta x)^2} \\
  &\quad + \frac{u(x_i, y_{j+1}) - 2u(x_i, y_j) + u(x_i, y_{j+1})}{(\Delta y)^2} = 0

Note :math:`\Delta x = \Delta y`.  The difference equation is rewritten as

.. math::

  u(x_i, y_j) = \frac{u(x_{i+1}, y_j) + u(x_{i-1}, y_j)
    + u(x_i, y_{j+1}) + u(x_i, y_{j-1})}{4}

Apply the point-Jacobi method to write a formula to iteratively solve the
difference equation:

.. math::

  u^{n+1}(x_i, y_i) = \frac{u^n(x_{i+1}, y_j) + u^n(x_{i-1}, y_j)
    + u^n(x_i, y_{j+1}) + u^n(x_i, y_{j-1})}{4}

where :math:`u^n` is the solution at the :math:`n`-th iteration.

Using the above formula, we are ready to implement the solver in Python.  The
code is short and straight-forward:

.. literalinclude:: code/01_solve_python_loop.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

The full code is in :ref:`01_solve_python_loop.py
<nsd-arraydesign-example-solvepy>`.  The solution can be plotted using
matplotlib:

.. figure:: image/01_solve_python_loop.png
  :name: nsd-arraydesign-laplace-python
  :align: center
  :width: 100%

  Numerical solution of the Laplace equation in Python.

The solution takes quite a while (4.797 seconds) to converge using 2097
iterations:

.. code-block:: pycon

  >>> with Timer():
  >>>    u, step, norm = solve_python_loop()
  >>> print(step)
  Wall time: 4.79688 s
  2097

.. _nsd-arraydesign-analytical:

Analytical Solution
+++++++++++++++++++

Is the calculation correct?  It is the first question to ask for any numerical
application.

We may compare the numerical solution to the analytical solution.  Recall the
PDE and its boundary conditions:

.. math::

  & \frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2}
    = 0 \quad (0<x<1; 0<y<1) \\
  &\left\{\begin{array}{lll}
    u(0,y) = 0, & u(1,y) = \sin(\pi y) & \rule{4ex}{0pt} (0 \le y \le 1) \\
    u(x,0) = 0, & u(x,1) = 0 & \rule{4ex}{0pt} (0 \le x \le 1)
  \end{array}\right.

Use separation of variable.  Assume the solution :math:`u(x,y) =
\phi(x)\psi(y)`.

.. math::

  & \frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2} = 0
    \quad \Rightarrow \quad
  \phi''\psi + \phi\psi'' = 0 \\
  & \Rightarrow \quad \frac{\phi''}{\phi} = -\frac{\psi''}{\psi} = \lambda


.. math::

  \left\{\begin{array}{ll}
    \phi'' - \lambda\phi = 0, & \phi(0) = 0 \\
    \psi'' + \lambda\psi = 0, & \psi(0) = 0, \, \psi(1) = 0
  \end{array}\right.

:math:`\lambda` is the eigenvalue.  The general solution of the ODE of
:math:`\psi` can be obtained as

.. math::

  \psi(y) = c\cos(\kappa y) + d\sin(\kappa y), \quad \kappa^2 = \lambda

Substitute the boundary conditions to the general solution

.. math::

  & \psi(0) = c = 0 \, \Rightarrow \, c = 0 \\
  & \psi(1) = d\sin(\kappa) = 0
    \, \Rightarrow \, \kappa = n\pi, \, n = 1, 2, 3, \ldots \\
  & \psi(y) = \sum_{n=1}^{\infty} \psi_n(y),
    \; \mbox{where} \; \psi_n(y) = d_n\sin(n\pi y) \\
  & \Rightarrow \psi(y) = \sum_{n=1}^{\infty} d_n \sin(n\pi y)

Substitute the eigenvalue $\lambda$ into the ODE of :math:`\phi`

.. math::

  \phi'' - (n\pi)^2\phi = 0

The general solution is

.. math::

  \phi_n(x) = a_n\cosh(\kappa x) + b_n\sinh(\kappa x)

Apply the boundary condition :math:`\phi_n(0) = a_n = 0` and obtain
:math:`\phi_n(x) = b_n\sinh(\kappa x)`.

The solution :math:`u(x, y)` can now be written as

.. math::

  u(x,y) = \sum_{n=1}^{\infty}\phi_n(x)\psi_n(y)
    = \sum_{n=1}^{\infty} \alpha_n \sinh(n\pi x)\sin(n\pi y)

where $\alpha_n = b_nd_n$.  Apply the last boundary condition

.. math::

  u(1,y) = \sin(\pi y) = \sum_{n=1}^{\infty}\alpha_n\sinh(n\pi)\sin(n\pi)

It is obtained that :math:`\alpha_1 = \sinh^{-1}(\pi)` and :math:`\alpha_k = 0
\forall k = 2, 3, \ldots`.  The solution of :math:`u` is obtained:

.. math::

  u(x, y) = \frac{\sinh(\pi x)}{\sinh(\pi)} \sin(\pi y)

.. literalinclude:: code/01_solve_analytical.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

The full code is in :ref:`01_solve_analytical.py
<nsd-arraydesign-example-solveana>`.  The solution is plotted:

.. figure:: image/01_solve_analytical.png
  :name: nsd-arraydesign-laplace-analytical
  :align: center
  :width: 100%

  Analytical solution of the Laplace equation.

Say :math:`u_a` is the analytical solution.  The numerical solution is compared
against the analytical solution, and the result :math:`|u-u_a|_{\infty} <
0.005` is good enough.

.. code-block:: pycon

  >>> uanalytical = solve_analytical()
  >>> # Calculate the L inf norm.
  >>> print("Linf of difference is %f" % np.abs(u - uanalytical).max())
  Linf of difference is 0.004962

.. _nsd-arraydesign-numpy:

Array-Based Code with Numpy
+++++++++++++++++++++++++++

To speed up the slow Python loops, numpy is usually a solution.  Numpy
implements fast calculations in C.  By using numpy, the intensive calculation
is delegated to C.

.. literalinclude:: code/02_solve_array.py
  :language: python
  :linenos:
  :start-after: # [begin example]
  :end-before: # [end example]

The full code is in :ref:`02_solve_array.py <nsd-arraydesign-example-solvenp>`.
The solution is verified to be the same as that with naive Python loops and
then plotted:

.. figure:: image/02_solve_array.png
  :name: nsd-arraydesign-laplace-numpy
  :align: center
  :width: 100%

  Numerical solution of the Laplace equation using numpy.

The speed is much better: less than 0.1 second.  Compared to that of the naive
Python loops, the speed up is more than 50x (to be exact, :ref:`87x
<runtime-comparison>`):

.. code-block:: pycon

  >>> # Run the Python solver
  >>> with Timer():
  >>>     u, step, norm = solve_array()
  Wall time: 0.0552309 s

.. _nsd-arraydesign-cpp:

Nested Loop in C++
++++++++++++++++++

While numpy does speed up the naive Python loops, it has two drawbacks:

1. It is still not optimal speed.
2. The code is not easy to read, compared to the element-wise code in the loop
   version.  Nested loop reads more straight-forward for our point-Jacobi
   method.

Here we will see how much faster it can get to move the code from numpy to C++.
To simplify the algorithm implementation, I use a library named `xtensor
<http://xtensor.readthedocs.io/>`_.  It defines the multi-dimensional array
data structure suitable for compile-time optimization.  An array library like
that makes it easy to organize the code and achieve faster runtime.

Except the parentheses, the C++ version looks almost the same as the Python
version.

.. literalinclude:: code/solve_cpp.cpp
  :language: cpp
  :linenos:
  :start-after: // [begin example]
  :end-before: // [end example]

The full C++ code is in :ref:`solve_cpp.cpp
<nsd-arraydesign-example-solvecpp>`.  The Python driver code for the C++ kernel
is in :ref:`03_solve_cpp.py <nsd-arraydesign-example-solvecpppy>`.  We verify
the C++ code is to verify the solution is the same as before.  It is plotted:

.. figure:: image/03_solve_cpp.png
  :name: nsd-arraydesign-laplace-cpp
  :align: center
  :width: 100%

  Numerical solution of the Laplace equation using C++.

Then we time the C++ version:

.. code-block:: pycon

  >>> with Timer():
  >>>     u, step, norm = solve_cpp.solve_cpp(uoriginal)
  Wall time: 0.0251369 s

It is twice as fast as the numpy version (0.0552309 seconds).

Overall Comparison
++++++++++++++++++

Now make a comparison for the three different implementations.

Runtime Performance
-------------------

By putting all timing data in a table, it is clear how much the C++ version
wins:

.. list-table:: Runtime with Python loop, Numpy array, and C++ loop.
  :name: runtime-comparison
  :align: center

  * - Code
    - Time (s)
    - Speedup
    -
  * - Python nested loop
    - 4.797
    - 1 (baseline)
    - n/a
  * - Numpy array
    - 0.055
    - 87.22
    - 1 (baseline)
  * - C++ nested loop
    - 0.025
    - 191.88
    - 2.2

Readability and/or Maintainability
----------------------------------

To compare the easy-of-reading among the three versions, we list the three
different computing kernels side by side:

.. code-block:: python
  :name: nsd-arraydesign-kernel-py
  :caption:
    Element-wise kernel using pure Python (full case in
    :ref:`nsd-arraydesign-python`).

  for it in range(1, nx-1):
      for jt in range(1, nx-1):
          un[it,jt] = (u[it+1,jt] + u[it-1,jt] + u[it,jt+1] + u[it,jt-1]) / 4

.. code-block:: python
  :name: nsd-arraydesign-kernel-np
  :caption:
    Element-wise kernel using numpy (full case in
    :ref:`nsd-arraydesign-numpy`).

  un[1:nx-1,1:nx-1] = (u[2:nx,1:nx-1] + u[0:nx-2,1:nx-1] +
                       u[1:nx-1,2:nx] + u[1:nx-1,0:nx-2]) / 4

.. code-block:: cpp
  :name: nsd-arraydesign-kernel-cpp
  :caption:
    Element-wise kernel using C++ (full case in :ref:`nsd-arraydesign-cpp`).

  for (size_t it=1; it<nx-1; ++it)
  {
      for (size_t jt=1; jt<nx-1; ++jt)
      {
          un(it,jt) = (u(it+1,jt) + u(it-1,jt) + u(it,jt+1) + u(it,jt-1)) / 4;
      }
  }

Major Source of Overhead: Data Preparation
==========================================

.. contents:: Contents in the section
  :local:
  :depth: 1

Numerical calculation takes time.  Intuitively, developers spend time on
optimizing the number-crunching code.  However, for a useful application, the
house-keeping code for preparing the calculation data and post-processing the
results is equally important.

In our previous example of solving the Laplace equation, all the conditions are
hard-coded.  It's OK for the teaching purpose, but not useful to those who
don't know so much about the math and numerical.  This time, I will use an
example of curve fitting to show how the house-keeping code affects
performance, and xtensor comes to help.

We will do polynomial curve fitting for data in groups of variable length.

.. literalinclude:: code/data_prep.cpp
  :language: cpp
  :linenos:
  :end-before: // [end example]

.. code-block:: pycon

  >>> with Timer():
  >>>     # np.unique returns a sorted array.
  >>>     xdata = np.unique(np.random.sample(1000000) * 1000)
  >>>     ydata = np.random.sample(len(xdata)) * 1000
  Wall time: 0.114635 s

.. code-block:: python

  import data_prep

  plt.rc('figure', figsize=(12, 8))

  def plot_poly_fitted(i):
      slct = (xdata>=i)&(xdata<(i+1))
      sub_x = xdata[slct]
      sub_y = ydata[slct]
      poly = data_prep.fit_poly(sub_x, sub_y, 3)
      print(poly)
      poly = np.poly1d(poly)
      xp = np.linspace(sub_x.min(), sub_x.max(), 100)
      plt.plot(sub_x, sub_y, '.', xp, poly(xp), '-')

.. code-block:: pycon

  >>> plot_poly_fitted(10)

.. figure:: image/04_fit_poly.png
  :align: center
  :width: 100%

Now, let's see the impact to runtime from the house-keeping code outside the
calculating helper.

.. code-block:: pycon

  >>> with Timer():
  >>>     # Do the calculation for the 1000 groups of points.
  >>>     polygroup = np.empty((1000, 3), dtype='float64')
  >>>     for i in range(1000):
  >>>         # Use numpy to build the point group.
  >>>         slct = (xdata>=i)&(xdata<(i+1))
  >>>         sub_x = xdata[slct]
  >>>         sub_y = ydata[slct]
  >>>         polygroup[i,:] = data_prep.fit_poly(sub_x, sub_y, 2)
  Wall time: 1.49671 s

.. code-block:: pycon

  >>> with Timer():
  >>>     # Using numpy to build the point groups takes a lot of time.
  >>>     data_groups = []
  >>>     for i in range(1000):
  >>>         slct = (xdata>=i)&(xdata<(i+1))
  >>>         data_groups.append((xdata[slct], ydata[slct]))
  Wall time: 1.24653 s

.. code-block:: pycon

  >>> with Timer():
  >>>     # Fitting helper runtime is much less than building the point groups.
  >>>     polygroup = np.empty((1000, 3), dtype='float64')
  >>>     for it, (sub_x, sub_y) in enumerate(data_groups):
  >>>         polygroup[it,:] = data_prep.fit_poly(sub_x, sub_y, 2)
  Wall time: 0.215859 s

It's very productive to write house-keeping code in Python.  As we see, the
price to pay is the runtime, and oftentimes memory as well.  But to spend 5x
the runtime in house-keeping code is intolerable.  We need to write C++ to
speed up.

Now see the ``fit_polys()`` C++ helper.  It detects the point group right
before fitting.

.. code-block:: pycon

  >>> with Timer():
  >>>     rbatch = data_prep.fit_polys(xdata, ydata, 2)
  Wall time: 0.21058 s

Struct of Array and Array of Struct
===================================

.. contents:: Contents in the section
  :local:
  :depth: 1

.. code-block:: cpp
  :linenos:

  struct StructOfArray
  {
      std::vector<double> x;
      std::vector<double> y;
  };

  struct PointProxy
  {
      StructOfArray * soa;
      size_t idx;
      double   x() const { return soa.x[idx]; }
      double & x()       { return soa.x[idx]; }
      double   y() const { return soa.y[idx]; }
      double & y()       { return soa.y[idx]; }
  };

  /*
   * Array of struct:
   */
  struct Point
  {
      double x, y;
  };

  using ArrayOfStruct = std::vector<Point>;

Conversion between Dynamic and Static
=====================================

.. contents:: Contents in the section
  :local:
  :depth: 1

It's not a bad idea to do it manually.  Spelling out the static to dynamic
conversion makes it clear what do we want to do.  When we work in the
inner-most loop, no :c:type:`python:PyObject` or virtual function table should
be there.

.. code-block:: cpp
  :linenos:

  template <size_t ND>
  class SpaceBase
  {
  public:
      static constexpr const size_t NDIM = ND;
      using serial_type = uint32_t;
      using real_type = double;
  }; /* end class SpaceBase */

  class StaticGrid1d
    : public StaticGridBase<1>
  {
  }; /* end class StaticGrid1d */

  class StaticGrid2d
    : public StaticGridBase<2>
  {
  }; /* end class StaticGrid2d */

  class StaticGrid3d
    : public StaticGridBase<3>
  {
  }; /* end class StaticGrid3d */

  /*
   * WrapStaticGridBase has the pybind11 wrapping code.
   */

  class WrapStaticGrid1d
    : public WrapStaticGridBase< WrapStaticGrid1d, StaticGrid1d >
  {
  }; /* end class WrapStaticGrid1d */

  class WrapStaticGrid2d
    : public WrapStaticGridBase< WrapStaticGrid2d, StaticGrid2d >
  {
  }; /* end class WrapStaticGrid2d */

  class WrapStaticGrid3d
    : public WrapStaticGridBase< WrapStaticGrid3d, StaticGrid3d >
  {
  }; /* end class WrapStaticGrid3d */

  PYBIND11_MODULE(_modmesh, mod)
  {
      WrapStaticGrid1d::commit(mod);
      WrapStaticGrid2d::commit(mod);
      WrapStaticGrid3d::commit(mod);
  }

Example: ``pybind11::cppfunction``
++++++++++++++++++++++++++++++++

``pybind11::cppfunction``
-------------------------

https://github.com/pybind/pybind11/blob/v2.4.3/include/pybind11/pybind11.h#L56

.. code-block:: cpp
  :linenos:

  /// Wraps an arbitrary C++ function/method/lambda function/.. into a callable Python object
  class cpp_function : public function {
  public:
      cpp_function() { }
      cpp_function(std::nullptr_t) { }

      /// Construct a cpp_function from a vanilla function pointer
      template <typename Return, typename... Args, typename... Extra>
      cpp_function(Return (*f)(Args...), const Extra&... extra) {
          initialize(f, f, extra...);
      }

      /// Construct a cpp_function from a lambda function (possibly with internal state)
      template <typename Func, typename... Extra,
                typename = detail::enable_if_t<detail::is_lambda<Func>::value>>
      cpp_function(Func &&f, const Extra&... extra) {
          initialize(std::forward<Func>(f),
                     (detail::function_signature_t<Func> *) nullptr, extra...);
      }

      /// Construct a cpp_function from a class method (non-const)
      template <typename Return, typename Class, typename... Arg, typename... Extra>
      cpp_function(Return (Class::*f)(Arg...), const Extra&... extra) {
          initialize([f](Class *c, Arg... args) -> Return { return (c->*f)(args...); },
                     (Return (*) (Class *, Arg...)) nullptr, extra...);
      }

      /// Construct a cpp_function from a class method (const)
      template <typename Return, typename Class, typename... Arg, typename... Extra>
      cpp_function(Return (Class::*f)(Arg...) const, const Extra&... extra) {
          initialize([f](const Class *c, Arg... args) -> Return { return (c->*f)(args...); },
                     (Return (*)(const Class *, Arg ...)) nullptr, extra...);
      }

  // ...
  }

``pybind11::cppfunction::initialize``
-------------------------------------

https://github.com/pybind/pybind11/blob/v2.4.3/include/pybind11/pybind11.h#L98

.. code-block:: cpp

  /// Special internal constructor for functors, lambda functions, etc.
  template <typename Func, typename Return, typename... Args, typename... Extra>
  void initialize(Func &&f, Return (*)(Args...), const Extra&... extra) {
    // ...
  }

``pybind11::cppfunction::dispatch``
-----------------------------------

https://github.com/pybind/pybind11/blob/v2.4.3/include/pybind11/pybind11.h#L423

.. code-block:: cpp

  static PyObject *dispatcher(PyObject *self, PyObject *args_in, PyObject *kwargs_in) {
    // ...
  }

Insert Profiling Code
=====================

.. contents:: Contents in the section
  :local:
  :depth: 1

In addition to using OS-provided profiling tools, e.g., Linux's perf and
Macos's Instruments, we should also add a custom profiling layer in the code.
You may need to port your code to a platform that doesn't have very good system
profiler.  Your custom profiler will become the safety net.

.. code-block:: cpp
  :linenos:

  /*
   * MODMESH_PROFILE defined: Enable profiling API.
   */
  #ifdef MODMESH_PROFILE

  #define MODMESH_TIME(NAME) \
      ScopedTimer local_scoped_timer_ ## __LINE__(NAME);

  /*
   * No MODMESH_PROFILE defined: Disable profiling API.
   */
  #else // MODMESH_PROFILE

  #define MODMESH_TIME(NAME)

  #endif // MODMESH_PROFILE
  /*
   * End MODMESH_PROFILE.
   */

  struct ScopedTimer
  {

      ScopedTimer() = delete;

      ScopedTimer(const char * name) : m_name(name) {}

      ~ScopedTimer()
      {
          TimeRegistry::me().add(m_name, m_sw.lap());
      }

      StopWatch m_sw;
      char const * m_name;

  }; /* end struct ScopedTimer */

  // Manually
  void StaticGrid1d::fill(StaticGrid1d::real_type val)
  {
      MODMESH_TIME("StaticGrid1d::fill");
      std::fill(m_coord.get(), m_coord.get()+m_nx, val);
  }

.. code-block:: pycon

  >>> import modmesh as mm
  >>>
  >>> gd = mm.StaticGrid1d(1000000)
  >>> for it in range(100):
  >>>     gd.fill(0)
  >>> print(mm.time_registry.report())
  StaticGrid1d.__init__ : count = 1 , time = 6.36e-06 (second)
  StaticGrid1d.fill : count = 100 , time = 0.0346712 (second)
  StaticGrid1d::fill : count = 100 , time = 0.0344818 (second)

Exercises
=========

1. xtensor allows writing array-based code in C++, just like what numpy does
   for Python.  Use xtensor to write array-based code in C++ by modifying the
   C++ version of the point-Jacobi solver.  The array-based C++ version should
   not have the inner loops.
2. By allowing changing the signature of the function ``fit_poly()``, how can
   we ensure the shapes of ``xarr`` and ``yarr`` to be the same, without the
   explicit check with ``"xarr and yarr size mismatch"``?  Write code to show.

References
==========

.. [1] xtensor; multi-dimensional arrays with broadcasting and lazy computing:
   https://xtensor.readthedocs.io

.. [2] xtensor-python; Python bindings for the xtensor C++ multi-dimensional
   array library: https://xtensor-python.readthedocs.io

.. [3] pybind11 — Seamless operability between C++11 and Python:
   https://pybind11.readthedocs.io/en/stable/

.. [4] IPython / Jupyter integration for pybind11:
   https://github.com/aldanor/ipybind

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
