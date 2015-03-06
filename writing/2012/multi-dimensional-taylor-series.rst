===============================
Multi-Dimensional Taylor Series
===============================

:date: 2012/10/8

Everyone knows how to write the `Taylor series
<http://mathworld.wolfram.com/TaylorSeries.html>`__.  Given a scalar function
:math:`f(\xi)`, its Taylor series can be written as

.. math::
  :label: e:1008:odexp

  f(\xi; \xi_r) = \sum_{k=0}^{\infty}
                  \frac{1}{k!} \frac{\mathrm{d}^k f}{\mathrm{d} \xi^k}
                  (\xi-\xi_r)^k

where :math:`\xi_r` is the centered point.  There is few challenge in obtaining
Eq. :eq:`e:1008:odexp`.  However, if the function of interest has more than one
variable, e.g., :math:`u(x_1, x_2, \ldots, x_N)`, Eq. :eq:`e:1008:odexp` can no
longer be used.

To obtain a formula for the Taylor series of a multi-dimensional function, it
is obvious that we should use partial derivatives :math:`\partial/\partial x_1,
\partial/\partial x_2, \ldots, \partial/\partial x_N` rather than simple
derivative :math:`\mathrm{d}/\mathrm{d}\xi`.  To insert the partial
derivatives, we need to use directional derivative.  To proceed, consider a
scalar function

.. math::

  u(x_1, x_2, \ldots, x_N)

in an :math:`\mathbb{E}^N`-space, in which the coordinate axes are denoted by

.. math::

  \mathbf{x} = (x_1, x_2, \ldots, x_N)

Let

.. math::

  \newcommand\defeq{\stackrel{\mathrm{def}}{=}}
  \mathbf{y} \defeq \mathbf{x} - \mathbf{x}_r

denote a vector pointing from a referencing point :math:`\mathbf{x}_r` to
:math:`\mathbf{x}`.  The directional derivative of :math:`u` is then written as

.. math::

  \nabla_yu = \vec{\nabla}u\cdot\left(\frac{\mathbf{y}}{y}\right)y
  = \left[\sum_{i=1}^N(x_i - {x_r}_i)\frac{\partial}{\partial x_i}\right] u

Similarly, higher-order directional derivatives can be written as

.. math::
  :label: e:1008:hodd

  \nabla_y^k u = \left[\sum_{i=1}^N(x_i-{x_r}_i)
                       \frac{\partial}{\partial x_i}\right]^k u

Aided by replacing the simple derivative in Eq. :eq:`e:1008:odexp` with the
directional derivative, the Taylor series of :math:`u` about :math:`\mathbf{x}_r`
can be expressed as

.. math::
  :label: e:1008:mdexp

  u(\mathbf{x}; \mathbf{x}_r) = \sum_{k=0}^{\infty} \frac{1}{k!}\nabla_y^k u

Substituting Eq. :eq:`e:1008:hodd` into Eq. :eq:`e:1008:mdexp` gives

.. math::
  :label: e:1008:mdtaylor

  u(\mathbf{x}; \mathbf{x}_r) = \sum_{k=0}^{\infty}
    \frac{1}{k!} \left[\sum_{i=1}^N(x_i-{x_r}_i)
                       \frac{\partial}{\partial x_i}\right]^k u

Equation is the Taylor series of a multi-dimensional function :math:`u(x_1,
x_2, \ldots, x_N)`.

It should be noted that `symmetry of the higher-order partial derivatives
<http://en.wikipedia.org/wiki/Symmetry_of_second_derivatives>`__ is not
assumed.  That is, the mixed partial derivatives are not commutative.
