===================
Lax-Wendroff Method
===================

:date: 2015/5/24

.. pstake:: laxgrid
  :align: center

  Grid definition for the Lax-Wendroff Method

Consider the conservation law

.. math::
  :label: e:de

  \newcommand{\bvec}[1]{\mathbf{#1}}
  \newcommand{\defeq}{\buildrel{\text{def}}\over{=}}
  \newcommand{\dpd}[3][]{\mathinner{
  \dfrac{\partial{^{#1}}#2}{\partial{#3^{#1}}}
  }}
  %
  \bvec{u}_t + \bvec{f}_x = 0

Approximate the derivatives in Eq. :eq:`e:de` by central diffenrencing at
the half grid point :math:`(x_j, t^{n+1/2})`:

.. math::
  :label: e:difapprox

  \bvec{u}_t \cong \frac{\bvec{u}_j^{n+1} - \bvec{u}_j^n}{\Delta t}, \quad
  \bvec{f}_x \cong \frac{\bvec{f}_{j+1/2}^{n+1/2} - \bvec{f}_{j-1/2}^{n+1/2}}
                        {\Delta x}

The Lax-Wendroff method [Lax60]_ can be constructed by replacing the time and
space derivatives in Eq. :eq:`e:de` with the differencing terms in Eq.
:eq:`e:difapprox`.

.. math::
  :label: e:disde

  \bvec{u}_j^{n+1} = \bvec{u}_j^n
                   - \frac{\Delta t}{\Delta x} \left(
                       \bvec{f}_{j+1/2}^{n+1/2} - \bvec{f}_{j-1/2}^{n+1/2}
                     \right)

To obtain an explicit time-marching scheme from Eq. :eq:`e:disde`, first use
Taylor's theorem:

.. math::
  :label: e:taylort

  \bvec{f}\left(x \pm \frac{1}{2}\Delta x, t + \frac{1}{2}\Delta t\right) =
    \bvec{f}\left(x \pm \frac{1}{2}\Delta x, t\right)
  + \frac{\Delta t}{2} \bvec{f}_t\left(x \pm \frac{1}{2}\Delta x, t\right)
  + \mathcal{O}(\Delta t^2)

Aided by the original differential equation (Eq. :eq:`e:de`), the time
derivative of :math:`\bvec{f}` can be replaced by a space derivative of
:math:`\bvec{u}`:

.. math::

  \bvec{f}_t = \mathrm{A}\bvec{u}_t = -\mathrm{A}\bvec{f}_x
             = - \mathrm{A}^2\bvec{u}_x, \quad
  \mathrm{A} \defeq \bvec{f}_{\bvec{u}}

Approximate the functions at the half grid points by averaging:

.. math::

  \bvec{f}_{j \pm 1/2} \cong \frac{\bvec{f}_{j \pm 1}^n + \bvec{f}_j^n}{2}

Approximate the space derivative at the half grid points:

.. math::

  (\bvec{u}_x)_{j+1/2}^n \cong \frac{u_{j+1}^n - u_j^n}{\Delta x}, \quad
  (\bvec{u}_x)_{j-1/2}^n \cong \frac{u_j^n - u_{j-1}^n}{\Delta x}

By omitting the higher order terms (:math:`\mathcal{O}(\Delta t^2)`) in Eq.
:eq:`e:taylort`, the functions at the :math:`x` and :math:`t` half grid points
can be approximated by values at the integer grid points

.. math::

  \bvec{f}_{j+1/2}^{n+1/2} &\cong
    \frac{\bvec{f}_{j+1}^n + \bvec{f}_j^n}{2}
  - \frac{1}{2}\frac{\Delta t}{\Delta x} \mathrm{A}^2
    \left(\bvec{u}_{j+1}^n - \bvec{u}_j^n\right)
  \\
  \bvec{f}_{j-1/2}^{n+1/2} &\cong
    \frac{\bvec{f}_j^n + \bvec{f}_{j-1}^n}{2}
  - \frac{1}{2}\frac{\Delta t}{\Delta x} \mathrm{A}^2
    \left(\bvec{u}_j^n - \bvec{u}_{j-1}^n\right)

And thus

.. math::
  :label: e:genmarch

  \bvec{u}_j^{n+1} = \bvec{u}_j^n
    - \frac{1}{2}\frac{\Delta t}{\Delta x}
      \left(\bvec{f}_{j+1}^n - \bvec{f}_{j-1}^n\right)
    + \frac{1}{2}\frac{\Delta t^2}{\Delta x^2} \mathrm{A}^2
      \left(\bvec{u}_{j+1}^n - 2\bvec{u}_j^n + \bvec{u}_{j-1}^n\right)

Scalar Linear Example
=====================

Choose a simple function

.. math::

  f = au

The time-marching scheme can be written as

.. math::

  u_j^{n+1} = \frac{\nu(\nu+1)}{2} u_{j-1}^n
            + (1 - \nu^2)u_j^n
            + \frac{\nu(\nu-1)}{2} u_{j+1}^n

where

.. math::

  \nu \defeq \frac{a\Delta t}{\Delta x}

is the CFL number.

.. [Lax60] P. Lax and B. Wendroff, “Systems of conservation laws,”
  Comm. Pure Appl. Math., vol. 13, no. 2, pp. 217-237, May 1960.
  `DOI: 10.1002/cpa.3160130205 <http://dx.doi.org/10.1002/cpa.3160130205>`__
