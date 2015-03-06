=============================
Recall Internal Laminar Flows
=============================

:date: 2013/12/18

My memory about the classic flow problems faded in the past several years.  The
notes here are my redemption.

Consider the steady-state, fully-developed, incompressible, viscous laminar
flow between two infinite long parallel plates.  :math:`x, y, z` are the axes
of the Cartesian coordinate system and all flow properties remain the same in
the :math:`z` direction.  The flow direction is toward :math:`+x`.  Let
:math:`u, v` be the velocity in the :math:`x, y` directions, respectively,
:math:`p` the pressure, and :math:`\tau_{\xi\eta}` the stress.  Let the lower
plate be at :math:`y = 0` and the upper plate be at :math:`y = a`, where
:math:`a` is a given constant.

Consider the :math:`x` component of the momentum equation on a infinitesimal
square control volume that's :math:`\mathrm{d} x` wide and :math:`\mathrm{d} y`
high:

.. math::

  & \left[p - \left(p + \frac{\partial p}{\partial x}\right)\right]\mathrm{d} y
  - \left[\tau_{yx} - \left(\tau_{yx}
                          + \frac{\partial \tau_{yx}}{\partial y}
                            \mathrm{d} y
                       \right)
    \right] \mathrm{d} x = 0 \\
  \Rightarrow & \frac{\partial\tau_{yx}}{\partial y} =
  \frac{\partial p}{\partial x}

Assume Newtonian fluid:

.. math::

  \tau_{yx} = \mu\left(\frac{\partial u}{\partial y}
                     + \frac{\partial v}{\partial x}\right)

where :math:`\mu` is the dynamic viscosity coefficient.  Because the flow is
fully developed, the vertical velocity :math:`v` is void and thus
:math:`\partial v / \partial x = 0`:

.. math::

  \tau_{yx} = \mu\frac{\partial u}{\partial y}

Then we obtain the following equation for :math:`u`:

.. math::

  \mu\frac{\partial^2 u}{\partial y^2} = \frac{\partial p}{\partial x}

Let :math:`\partial p / \partial x` be constant (this can be justified) and
apply the conditions of :math:`u(0) = u(a) = 0`:

.. math::

  u(y) = \frac{1}{2\mu}\frac{\partial p}{\partial x} y \left(y-a\right)
       = - \frac{a^2}{8\mu}\frac{\partial p}{\partial x}
         + \frac{1}{2\mu}\frac{\partial p}{\partial x}
           \left(y - \frac{a}{2}\right)^2

For :math:`\partial p / \partial x < 0`, :math:`u \ge 0` and the peak velocity
is :math:`u(y = a/2) = -(a^2/8\mu)\partial p / \partial x`.
