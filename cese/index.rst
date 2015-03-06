======================================================================
The space-time Conservation Element and Solution Element (CESE) Method
======================================================================

The space-time conservation element and solution element (CESE) method is a
novel numerical method developed by `Sin-Chung Chang
<http://www.grc.nasa.gov/WWW/microbus/>`_ for time-accurate, high-fidelity
solution of hyperbolic partial different equations or conservation laws:

.. math::

  \frac{\partial\mathbf{u}}{\partial t}
  + \sum_{\iota=1}^3
    \frac{\partial\mathbf{f}^{(\iota)}(\mathbf{u})}{\partial x_{\iota}}
  = 0
  \quad \Rightarrow \quad
  \frac{\partial\mathbf{u}}{\partial t}
  + \sum_{\iota=1}^3 \mathrm{A}^{(\iota)}(\mathbf{u})
                     \frac{\partial\mathbf{u}}{\partial x_{\iota}}
  = 0

Key features of the CESE method include:

- Temporal coordinate is treated in the same way as spatial coordinates for
  calculating space-time fluxes.
- The method solves the integral form of the equations.
- The method uses staggered space-time mesh.
- Unstructured meshes are intrinsically used in multi-dimensional space.
- The method is mathematically simple.  Time-marching algorithms are based on
  enforcing space-time flux conservation.
- No Riemann solver is used.
- The gradients of solution are also independent unknowns.
- The Jacobian matrices in the first-order formulation fully characterize the
  equations to be solved.

Consequently, the CESE method provides the following merits:

- Nonlinearity is easily accommodated.  Shock-capturing is accurate and robust.
- The explicit time-marching algorithms are straightforward and efficient in
  terms of operation count.
- Complex geometry can be easily fit by using unstructured meshes with mixed
  shapes.
- The method is ideal for parallel computing.
- The method is generic to all conservation laws, i.e., changing solely the
  Jacobian matrices changes the equations to be solved.
- The method provides simple and effective treatments for non-reflective
  boundary conditions.

For more information, please consult the website of the `space-time CE/SE
working group <http://www.grc.nasa.gov/WWW/microbus/>`_.
