=========================================
Matrix-Vector Form of The Euler Equations
=========================================

:date: 2015/4/21

.. contents:: Table of Contents
  :local:

.. math::

  \newcommand{\bvec}[1]{\mathbf{#1}}
  \newcommand{\defeq}{\buildrel{\text{def}}\over{=}}

Governing Equations
===================

The Euler equations consist of the mass conservation

.. math::
  :label: euler.mass

  \frac{\partial\rho}{\partial t} + \frac{\partial\rho v_j}{\partial x_j}
    = 0

momentum conservation

.. math::
  :label: euler.momentum

  \frac{\partial\rho v_i}{\partial t} 
    + \frac{\partial\rho v_iv_j}{\partial x_j}
    = \frac{\partial p}{\partial x_j} + \rho b_i

and energy conservation

.. math::
  :label: euler.energy

  \frac{\partial}{\partial t}
      \left[\rho\left( e + \frac{v_k^2}{2} \right)\right]
    + \frac{\partial}{\partial x_j}
        \left[\rho\left( e + \frac{v_k^2}{2} \right)v_j\right]
    = \rho \dot{q} - \frac{\partial pv_j}{\partial x_j} + \rho b_jv_j

Einstein's index summation convention is used.

Equations :eq:`euler.mass`, :eq:`euler.momentum`, and :eq:`euler.energy` aren't
closed even if we choose :math:`\bvec{b}` and :math:`\dot{q}` as given.  We
have 5 equtions but 6 unknowns (:math:`\rho`, :math:`\bvec{v}`, :math:`p`, and
:math:`e`).  To close the system of equations, I use the equation of state:

.. math::
  :label: euler:eos

  p = \rho RT

Internal energy is related to temperate:

.. math::
  :label: euler:internal

  e = c_vT = \frac{RT}{\gamma-1} = \frac{1}{\gamma-1}\frac{p}{\rho}

By adding the two equations (Eqs. :eq:`euler:eos` and :eq:`euler:internal`) and
one variable :math:`T`, the equations are closed.

Vector Form
===========

Define the conservation variables:

.. math::
  :label: euler:unknown

  \bvec{u} \defeq \left(\begin{array}{c}
    u_1 \\ u_2 \\ u_3 \\ u_4 \\ u_5
  \end{array}\right) = \left(\begin{array}{c}
    \rho \\ \rho v_1 \\ \rho v_2 \\ \rho v_3 \\
    \rho\left(e+\frac{v_k^2}{2}\right)
  \end{array}\right)

Aided by writing the pressure with :math:`\bvec{u}`:

.. math::

  p = (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)

the conservation equations (Eqs. :eq:`euler.mass`, :eq:`euler.momentum`, and
:eq:`euler.energy`) can be cast to use only :math:`\bvec{u}`:

.. math::
  :label: euler:gov1

  \frac{\partial u_1}{\partial t}
    + \frac{\partial u_2}{\partial x_1}
    + \frac{\partial u_3}{\partial x_2}
    + \frac{\partial u_4}{\partial x_3} = 0

.. math::
  :label: euler:gov2

  \begin{aligned} &\frac{\partial u_2}{\partial t}
    + \frac{\partial}{\partial x_1}\left(\frac{u_2^2}{u_1}\right)
    + \frac{\partial}{\partial x_2}\left(\frac{u_2u_3}{u_1}\right)
    + \frac{\partial}{\partial x_3}\left(\frac{u_2u_4}{u_1}\right) = \\
    &\quad -\frac{\partial}{\partial x_1}\left[
        (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)
      \right] + b_1u_1
  \end{aligned}

.. math::
  :label: euler:gov3

  \begin{aligned} &\frac{\partial u_3}{\partial t}
    + \frac{\partial}{\partial x_1}\left(\frac{u_2u_3}{u_1}\right)
    + \frac{\partial}{\partial x_2}\left(\frac{u_3^2}{u_1}\right)
    + \frac{\partial}{\partial x_3}\left(\frac{u_3u_4}{u_1}\right) = \\
    &\quad -\frac{\partial}{\partial x_2}\left[
        (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)
      \right] + b_2u_1
  \end{aligned}

.. math::
  :label: euler:gov4

  \begin{aligned} &\frac{\partial u_4}{\partial t}
    + \frac{\partial}{\partial x_1}\left(\frac{u_2u_4}{u_1}\right)
    + \frac{\partial}{\partial x_2}\left(\frac{u_3u_4}{u_1}\right)
    + \frac{\partial}{\partial x_3}\left(\frac{u_4^2}{u_1}\right) = \\
    &\quad -\frac{\partial}{\partial x_3}\left[
        (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)
      \right] + b_3u_1
  \end{aligned}

.. math::
  :label: euler:gov5

  \begin{aligned} &\frac{\partial u_5}{\partial t}
    + \frac{\partial}{\partial x_1}\left(\frac{u_2u_5}{u_1}\right)
    + \frac{\partial}{\partial x_2}\left(\frac{u_3u_5}{u_1}\right)
    + \frac{\partial}{\partial x_3}\left(\frac{u_4u_5}{u_1}\right) = \\
    &\quad - \frac{\partial}{\partial x_1}\left[
        (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)
        \frac{u_2}{u_1}
      \right] \\
    &\quad - \frac{\partial}{\partial x_2}\left[
        (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)
        \frac{u_3}{u_1}
      \right] \\
    &\quad - \frac{\partial}{\partial x_3}\left[
        (\gamma-1)\left(u_5 - \frac{u_2^2+u_3^2+u_4^2}{2u_1}\right)
        \frac{u_4}{u_1}
      \right]
    + \rho\dot{q} + b_1u_2 + b_2u_3 + b_3u_4
  \end{aligned}

Then organize Eqs. :eq:`euler:gov1` -- :eq:`euler:gov5` into a vector form:

.. math::
  :label: euler:vec

  \frac{\partial\bvec{u}}{\partial t}
    + \sum_{\mu=1}^3 \frac{\partial\bvec{f}^{(\mu)}}{\partial x_{\mu}}
    = 0

The flux functions are defined as:

.. math::
  :label: euler:flux1

  \bvec{f}^{(1)} &= \left(\begin{array}{c}
    f^{(1)}_1 \\ f^{(1)}_2 \\ f^{(1)}_3 \\ f^{(1)}_4 \\ f^{(1)}_5
  \end{array}\right) \defeq \left(\begin{array}{l}
    u_2 \\
    (\gamma-1)u_5 - \frac{\gamma-3}{2}\frac{u_2^2}{u_1}
      - \frac{\gamma-1}{2}\frac{u_3^2}{u_1}
      - \frac{\gamma-1}{2}\frac{u_4^2}{u_1} \\
    \frac{u_2u_3}{u_1} \\
    \frac{u_2u_4}{u_1} \\
    \gamma\frac{u_2u_5}{u_1}
      - \frac{\gamma-1}{2}\frac{u_2^2+u_3^2+u_4^2}{u_1}\frac{u_2}{u_1}
  \end{array}\right)
  
.. math::
  :label: euler:flux2

  \bvec{f}^{(2)} &= \left(\begin{array}{c}
    f^{(2)}_1 \\ f^{(2)}_2 \\ f^{(2)}_3 \\ f^{(2)}_4 \\ f^{(2)}_5
  \end{array}\right) \defeq \left(\begin{array}{l}
    u_3 \\
    \frac{u_2u_3}{u_1} \\
    (\gamma-1)u_5 - \frac{\gamma-1}{2}\frac{u_2^2}{u_1}
      - \frac{\gamma-3}{2}\frac{u_3^2}{u_1}
      - \frac{\gamma-1}{2}\frac{u_4^2}{u_1} \\
    \frac{u_3u_4}{u_1} \\
    \gamma\frac{u_3u_5}{u_1}
      - \frac{\gamma-1}{2}\frac{u_2^2+u_3^2+u_4^2}{u_1}\frac{u_3}{u_1}
  \end{array}\right)

.. math::
  :label: euler:flux3

  \bvec{f}^{(3)} &= \left(\begin{array}{c}
    f^{(3)}_1 \\ f^{(3)}_2 \\ f^{(3)}_3 \\ f^{(3)}_4 \\ f^{(3)}_5
  \end{array}\right) \defeq \left(\begin{array}{l}
    u_4 \\
    \frac{u_2u_4}{u_1} \\
    \frac{u_3u_4}{u_1} \\
    (\gamma-1)u_5 - \frac{\gamma-1}{2}\frac{u_2^2}{u_1}
      - \frac{\gamma-1}{2}\frac{u_3^2}{u_1}
      - \frac{\gamma-3}{2}\frac{u_4^2}{u_1} \\
    \gamma\frac{u_4u_5}{u_1}
      - \frac{\gamma-1}{2}\frac{u_2^2+u_3^2+u_4^2}{u_1}\frac{u_4}{u_1}
  \end{array}\right)

Nomenclature
============

:math:`\bvec{x} \defeq (x_1, x_2, x_3)^t`
  Space vector.

:math:`t`
  Time.

:math:`\rho`
  Mass density.

:math:`\bvec{v} \defeq (v_1, v_2, v_3)^t`
  Flow velocity vector.

:math:`p`
  Pressure.

:math:`\bvec{b} \defeq (b_1, b_2, b_3)^t`
  Body force vector.

:math:`e`
  Internal energy density.

:math:`\dot{q}`
  Heat generation rate per unit volume.

:math:`R`
  Universal gas constant.

:math:`T`
  Temperature.

:math:`c_v`
  Specific heat at constant volume.

:math:`c_p`
  Specific heat at constant pressure.

:math:`\gamma \defeq c_p/c_v`
  Ratio of specific heat.

:math:`\bvec{u} \defeq (u_1, u_2, u_3, u_4, u_5)^t`
  Conservation variables.

:math:`\bvec{f}^{(1)}, \bvec{f}^{(2)}, \bvec{f}^{(3)}`
  Vector flux functions.
