=====================
SOLVCON Outlook, 2014
=====================

:date: 2014/1/21

I like to code and engineer for continuum-based first-principle simulations.
Although numerical simulations inherently have limitation, to use computers to
get to the details and insights is fascinating, especially when the problems
are otherwise difficult to be studied.  A table, some pencils, a laptop, and
data centers are all we need.  Oh, and we don't need to physically touch a data
center (or we aren't allowed to do so :)  It's amazing to be able to see so
much with so little.

But it doesn't mean the simulations are simple, although the building blocks
are really just mathematical formulations and computer code.  Remember devils
are in the details.  Correct and accountable simulation results are as
challenging as physical experiments, but call for a different skill set.  We
have to realize that doing the simulations means making special computer
software, of which the prerequisite is the "domain knowledge" that takes years
of study.

The point is that we are making software.  For scientific research, there is an
impression that the efforts on software should be minimized.  Many researchers
don't have long-term thinking about their code.  That causes problems.  A
computational scientist can be sure about nothing other than the formations and
the code.  If we don't program with discipline, our simulations won't have
desired credibility.  From empirical equations, semi-analytical simulations,
one-dimensional simulations to three-dimensional, massively-parallelized
simulations, the ever-increasing computing power drove the code to be more and
more complex, and we simply need to work like professional programmers to
properly organize our calculations.

Better Engineering for Science
==============================

More engineering practices should be brought into the picture.  We need to
construct the simulation software with discipline, but engineering software is
pretty different to other engineering.  In software, there's no "real thing"
that can be verified by the Mother Nature.  Many of us may not be familiar with
the skills and tools to be employed.

The goal of SOLVCON is to let a researcher focus on developing research code
that uses the CESE method.  From the engineering perspective, collaboration is
good and it's not reasonable to ask a single person to understand every detail
from the choice of design patterns to the development of constitutive models.
Ultimately, we hope everyone who's interested in SOLVCON's applicable areas can
find a comfortable place to contribute.

But it is unclear that how many and what roles there should be for a good team.
How people should collaborate?  How should the responsibility from different
roles to overlap?  For a usual research team in a university, we don't need to
answer these questions, because the team is more or less constrained by how
academia is working.  SOLVCON is an open-source project.  It enjoys a high
degree of freedom and the only significant constraint is resources.  We can
make different try.

Scope is still needed for a project.  The basis of SOLVCON is (i) unstructured
meshes (Voronoi-based) of mixed elements, (ii) the CESE method, and (iii)
hybrid implementation of Python and C.  SOLVCON is open-sourced with a
permissive license so close-source applications are OK.  On that basis we want
to learn how to do good computational science with good software practices.

Documenting
===========

The first thing to consider is communication, which is important for both
science and software.  Open and critical discussions make ideas to grow into
solutions.  In the software development communities many systems and services
have been developed to facilitate communication in development teams.
Documenting is the most prominent system, for all we have are just code and
formulations.

Formulations are what common software projects don't have.  There are some
solutions available, and SOLVCON uses Sphinx as the documenting system.  In
this way, the formulations and the simulations will be integrated into the
software's document.  In-progress demonstration can be found at
http://www.solvcon.net/en/latest/app_bulk.html and
http://www.solvcon.net/en/latest/app_linear.html.

One challenge with the traditional research papers is that the they can't carry
code.  For a medium-sized project of more than a thousand lines of code, the
separation between code and passages makes either part difficult to be
maintained.  The SOLVCON documenting system wants to improve this.  By using
"readthedocs" and "BitBucket", producing the cross-referenced document online
is just several clicks away.

Version Control and Building
============================

Although everything about numerical simulations is in computers, we still can
say that we have one tangible asset: source code.  The source code should be
properly managed and can be made to the center of all communication.  In
software development it is called version control, and there are many version
control systems (VCS) and services at our disposal.  SOLVCON uses Mercurial
(hg) with the BitBucket hosting service.  BitBucket also provides a nice issue
tracker and can hook into other online services.

Source code needs to be built before running.  Building the binary code from
source code involves compiling, linking, and organizing the intermediates at
proper places.  It's much more complex than just compiling so that we need a
build system.  SOLVCON uses SCons to build its binary parts.  Standard Python
distutil is used after SCons for making source package.

In addition to building, SOLVCON provides scripts in "ground/" and "soil/"
directories to build third-party software packages, if they are not otherwise
available.  The dependency management is especially important when running
SOLVCON on a supercomputer site, for usually the pre-installed software
packages are not sufficient to run SOLVCON.

Testing
=======

Testing is something can't be taken away from the development cycle.  It is
different from the result verification for an under-development simulation, but
about repeatedly confirming the already verified (or partially verified)
results are still valid.

In SOLVCON there are three levels of testing: the unit testing, functional
testing, and solution testing.  The unit testing is using the standard Python
unit test framework and doctest.  The functional testing is arranged for
interoperation of different modules in the software system.  Solution testing
is responsible for making sure the calculation results remain the same after
when changing the code.  The solution tests are collected in the "examples/"
directory, and scripts of running these solution tests or examples are
provided.

It is important to separate the testing into levels.  We should constantly run
tests to check that we don't break code, because if we do, it takes more time
to fix errors in the future than to avoid errors during development.  Usually,
the more involved tests need more resources and more complex setup.  For
example, all unit tests in SOLVCON can be run with a single "nosetests"
command, and they finish in 10 seconds on my MacBook Air.  In contrast, a
solution test for a solver in SOLVCON takes 10 minutes on a 8-core server.  We
can use unit tests to quickly detect errors, and the expensive solution tests
only need to be invoked at a late phase of development.

A continuous integration (CI) system (Jenkins) is set up at
http://ci.solvcon.net/ to carry out all the tests and code building with every
commit in the source code repository.  If a developer forgets to run the tests,
the CI system will still run them anyway.  Whenever it's possible, we should
set up automation to do repeating tasks and free up developers' or researchers'
time for something more valuable.

Scheduling for Collaboration
============================

To some extent, developing code is like developing formulations.  They both
consumes large chucks of time and are usually a standalone activity that
requires one to get into the "flow" for efficient and quality work.  That is,
we can't be disturbed in the middle of the work flow or we will lose a lot of
productivity.

If we centralize the work to software development, or, in another word, do
computational research like we are developing software, then collaboration
techniques for software can come to help the research.  Because research teams
are usually small (tens of people or less), we should adopt established agile
methodologies like Scrum, Kanban, eXtreme Programming (XP), etc.  These
methodologies are not addressing how to manage the "domain knowledge", which is
our focus of computational science.  We need to adjust them to balance the
software and non-software parts.

Conclusions
===========

This isn't the complete description about what SOLVCON wants to achieve, but
provides an overview for its possible contribution outside its basis.  The
fundamental infrastructure mentioned above is already there.  The project
should be expanded to find out how far it can go.  For now, the use of the CESE
method is a limiting condition for its applications, but also an opportunity to
bring new things into unexplored application areas.  Preliminary applications
developed for supersonic flows, waves in solids, and acoustics can be used as
working models.

I hope to know more people who are interested in this model of developing PDE
solvers or the CESE method, and try a fun approach of doing computational
research together.
