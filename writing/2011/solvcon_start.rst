============================
Getting Started with SOLVCON
============================

.. contents::

This is a two-hour course to provide an overview of SOLVCON by doing examples
contained in SOLVCON.  The course contains:

#. Installation and test (30 min):

   - How to use the developing version of SOLVCON (15 min)
   - How to unpack and use the released version of SOLVCON (10 min)
   - Use of ``PYTOHNPATH`` (5 min)
#. Learn by examples (70 min):

   - Layout of examples (5 min)
   - The simplest example: supersonic flow past a blunt body (15 min)
   - Complex pre- and post-processing: reflection of oblique shock wave (20 min)
   - Alternate physical model: group velocity of Beryl (15 min)
   - Output to parallel VTK format (10 min)
   - Other examples (5 min)
#. Other resources (10 min):

Installation and Test
=====================

Developing Version of SOLVCON
+++++++++++++++++++++++++++++

#. Follow the direction at http://solvcon.net/guide.html for installation.

   - Make sure all the dependencies are met.  Use::

       $ python -c "import sys; print sys.version"

     to check the version of your Python (must be 2.6).
#. Install to your home directory.  See
   http://solvcon.net/guide.html#rebuild-reinstall\ .

   - Use::

       $ python -c "import solvcon; print solvcon; print solvcon.__version__"

     to check your installation.  There must be a ``+`` in the version number.
#. Perform unit tests with the two approaches specified at
   http://solvcon.net/guide.html#unit-test\ .

Let ``$SCSRC`` be the location of the cloned SOLVCON.

Released Version of SOLVCON
+++++++++++++++++++++++++++

#. Follow the direction at http://solvcon.net/guide.html to install it in your
   home directory.  This will override the previous installation.
#. Check SOLVCON version and perform unit test again.

Use PYTHONPATH
++++++++++++++

What is ``PYTHONPATH`` environment variable:
http://docs.python.org/tutorial/modules.html#the-module-search-path

Use::

  $ PYTHONPATH=$SCSRC python -c "import solvcon; print solvcon"

to demonstrate the effect.

Examples
========

#. Examples locate at ``$SCSRC/examples``.
#. Examples are categorized as:

   - ``euler/``: the Euler equations solver
   - ``elastic/``: the velocity-stress equations solver
   - ``visout/``: visualization and output
   - ``misc/``: miscellaneous
#. Driver script ``go`` in each sub-directory.
#. Obtain example data by executing::

     scons --get-scdata

   in ``$SCSRC``.
#. Install `ParaView <http://www.paraview.org/>`_ with::

     $ sudo apt-get install paraview

Flow over Cylinder
++++++++++++++++++

The simplest example: located at ``$SCSRC/examples/euler/hbnt/``.

Objective:

#. Understand the concept of "driving script" (programmable input file).
#. Perform simulation with SOLVCON.

Course:

#. Run the code::

     $ PYTHONPATH=../../.. ./go run

#. Simulation results are stored in the sub-directory ``result/``.  Use
   ParaView to load the VTK files.
#. Code organization:

   - First line: indicating it is a Python script.
   - Second line: indicating encoding of the file.
   - Line 4-18: comments for copyright information.
   - Line 20-27: docstring in the form of `string literal
     <http://docs.python.org/reference/lexical_analysis.html#string-literals>`_.
   - Line 29: module-level import for arrangement decorator.
   - Line 31-99: main body for instantiating the ``Case`` object in the form
     of a Python function/callable; "creation function".
   - Line 101-112: `decorated
     <http://en.wikipedia.org/wiki/Decorator_pattern>`_ arrangement
     (function).
   - Line 114-116: invoking command-line interface of SOLVCON.
#. Customization goes into the creation function:

   - Specify BC: line 54-59.
   - Feed parameter to Case: line 60-64.
#. SOLVCON modules to hack:

   - ``solvcon.boundcond``
   - ``solvcon.case``
   - ``solvcon.solver``

Reflection of Oblique Shock
+++++++++++++++++++++++++++

Located at ``$SCSRC/examples/euler/obrefl/``.

Objective:

- Use the programmability of input file for properties specification.
- Understand the Hook system for custom post-processing.

Question:

- Where is the creation function?

Course:

#. Run and inspect the simulation.
#. Change the flow properties in line 263-275 and see the difference.

   - Utility code is organized as a class in line 52-164, for calculating shock
     properties.
#. How to extend SOLVCON by using Hook, i.e., line 166-244, 318-320.
#. SOLVCON modules to hack:

   - ``solvcon.hook``
   - ``solvcon.kerpak.euler``

The Velocity-Stress Equation Solver
+++++++++++++++++++++++++++++++++++

Located at ``$SCSRC/examples/elastic/grpv/``.

Objective:

- Change the physical model.
- Understand the Anchor system for parallel processing.

Questions:

#. What is the path of the mesh file used in this simulation?
#. What is the equivalent code of line 123-125 in the previous two examples?

Course:

#. Run and inspect the simulation.
#. Note the difference of line 144.  It uses a different calculator to the
   Euler solver.
#. Line 76-89, 135-142 uses the Anchor system to insert source term.
#. Line 35-74 calculate the source value.
#. SOLVCON modules to hack:

   - ``solvcon.anchor``
   - ``solvcon.kerpak.elastic``

Output Control
++++++++++++++

Located at ``$SCSRC/examples/visout/pvtk/``.

Objective:

- Parallel run.
- Specify the variables to output.

Questions:

#. Guess what problem is it?
#. Where is the code for sequential VTK output in legacy format?

Course:

#. Run the simulation in parallel by following the docstring.
#. Inspect the solution.
#. Line 90-102 specifies three kinds of quantities:

   - Negative integer for array.
   - Zero for scalar.
   - Positive value for vector.
#. Try to turn off some of the variables by commenting out.

   - Before rerun the simulation, clean the ``result/`` directory.
#. SOLVCON sub-package to hack:

   - ``solvcon.io``

Other Examples
++++++++++++++

- ``$SCSRC/examples/euler/impl/``: implosion, which takes longer time to
  finish.
- ``$SCSRC/examples/misc/osc/``: tell you how to run on OSC.

Resources
=========

- Mailing list at http://groups.google.com/group/solvcon

  Where I make announcement and you ask questions.
- Blessed repository at https://bitbucket.org/yungyuc/solvcon/wiki/Home

  Wiki (collaborating documentation) is hosted here too.
- API document at http://solvcon.net/

  The up-to-dated API doc is generated nightly.
- Issue tracker at
  https://bitbucket.org/yungyuc/solvcon/issues?status=new&status=open
  
  Hosted with the repository.  Report and discuss bug, improvement, and any
  other issues related to the code.
- Buildbot at http://solvcon.net/buildbot/

  For monitoring the building and testing of the code.
- IRC chat room #solvcon at http://freenode.net/
