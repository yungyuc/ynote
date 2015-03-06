================
Code for Results
================

:date: 2012/6/24

I'd like to record some of my thoughts on how to improve the quality of
scientific codes.

Programming for scientific problems is difficult.  It is intrinsically a
multi-discipline task.  To develop efficient code to model interesting
phenomena, a researcher needs to know well about his problem, computer
architecture, various programming techniques, and many numerical analysis
skills.  After the code can finally be compiled and run, it needs to be tested,
debugged, and verified for all target machines (and platforms).  Then we know
the results are correct, reproducible, and trustworthy.

For collaboration, there are more things to do.  If the code is intended to be
released to peer researchers, it should be designed with comprehensible API.
If there would be general users who are not expected to touch the internals,
the API must be organized to allow building a user interface and/or a scripting
layer.  And a guide to the software system should be provided.  But perhaps
more importantly, the code should be available to all collaborators, and
potential collaborators.

Unfortunately, things do not always work that well.  It is common to find a
program that can only run on the machine it was developed on.  It is common to
find a program contains 5,000 lines of arbitrarily-indented code in a single
file.  It is common to find a program shipped with no or an out-dated manual,
and users have no clue to run an example problem.  It is so easy for a
scientific code to become unmaintainable, and its source is usually not
publicly available to reveal the fact.

In general, without a clear structure, version control, unit tests, API
documentation, rerunnable examples, no one can be sure that a code really does
what it claims.  Although the developers usually know what they are solving
for, the beauty of their work is covered by the dust of unconvincing
development practice.

Producing monolithic, ill-organized, limited-use code isn't anything that we
numerical analysts feel proud of.  The status quo should really be improved.  I
am going to write down what I think would be helpful for practicing scientific
programming.

Openness
========

Software development is on magic.  It is just an intellectual-intensive
activity involving computers.  When talking about intellectual development, the
importance of openness cannot be overemphasized.  An open society gestated the
modern high-tech civilization that we are enjoying.  An open environment is
needed to enable the mind sharing for software development.

Such an environment should enable collaboration at the source-code level.
Although many efforts have been made to shape software into components, no
software is simple enough to evade source-level reviewing.  Moreover, just like
a normal person weaves his thoughts by using a natural language, a competent
programmer thinks in a programming language.  The most efficient way for
programmers to collaborate is to use (well-written) source code, not diagrams,
slides, reports, or anything else.

Thus, we can conclude that open-source is a straight-forward approach to
respond the need for productive collaboration.  It should be noted that
open-source has nothing to do with free-of-charge software, although many
open-source projects use free for marketing.  The source-level collaboration
enabled by the open-source practice introduces new business models.  In the new
models, software can be developed by using fewer resources.

For a long time, software is regarded as a large chunk of compiled binary that
hides most of its internals and exposes only a limited amount of user
interface.  However, things do not need to be done in this way.  In the past
two decades, the widespread Internet and WWW have added new rules to the game.
The open-source practice has helped new giants like Yahoo, Apple, Google,
Facebook, and many other new-style software companies to win over the past
invincible opponents like Microsoft, which just started to catch up in the
race.

The trend of open-source will arrive everywhere that demands software.  Aided
by the successful marketing actions led by IBM and RedHat, the open-source
Linux operating system defeats SCO UNIX, Microsoft Windows, and many other
close-source competitors in the server and HPC market.  In the Top 500 list of
supercomputers one can rarely spot a system not running Linux.  The tool chain
of scientific computing is also full of other open-source software, e.g., GCC,
MPI (MPICH1/2, MVAPICH1/2, or OpenMPI), LAPACK, FFTW, to name but a few.
Open-source will help our pursuit of productivity in software development.

Hierarchical Software Stack
===========================

Scientific computing needs a hierarchical software stack.  One reason to do so
is to hide the complexity of the computing problem.  On the one hand,
engineering or scientific problems involve a huge amount of computation and
need low-level access to the raw metal for ultimate runtime efficiency.  On the
other hand, the complex mathematical expressions that describe the technical
problems encourage practitioners to employ high-level programming techniques to
write comprehensible code.  The polarized demand would shape the computing
software into multiple abstraction layer.

Another equally important reason for a hierarchical structure is the protection
of sensitive information.  From the security point of view, it is ideal to
open-source the fundamental computing platform as a framework, while protecting
business secrets in a lower-level, compiled layer.

In general, an easy-to-use, high-performance system for scientific computing
cannot be productively built by using an overarching tool or language.
Conventional approaches are either too costly (in terms of labor and money) or
unable to deliver (the runtime performance).  But perhaps the most serious
issue with the conventional approaches is the lack of outlook.  Bleeding-edge
hardware of heterogeneous architecture and the ever-increasing gap between the
floating-point efficiency and the memory operation efficiency call for
renovated software.  Developing monolithic code does not address the new needs.

In fact, vendors of scientific software have already recognized the need, and
structured their product into layers.  They either implement a proprietary
scripting language or use an existing one, to provide an easy-to-use platform
for automation and/or parametric study.  The scripting layer is built on top of
its core framework which realizes fundamental numerics.  One can find this
design in leading companies in almost all major application fields such as
MATLAB (general numerics), ANSYS (computational mechanics, fluid dynamics,
electromagnetics), 3DS/ABAQUS (computational mechanics), Autodesk (computer
aided design), and Synopsys (electronic design automation), to name but a few.

Although multiple abstraction layers have been materialized in these
proprietary software products, there are still many barriers preventing users
to gain higher productivity by accessing the internals.  In this regard, I
think the proprietary approach will be challenged by the users who persistently
demand more and more flexibility and productivity.  Many people would be drawn
to the open-source approach.

Prototype as Product
====================

Based on the hierarchical design and the open-source practice, a new breed of
scientific software can be produced.  In the past decade, new paradigms of
developing software emerged from the need to rapidly develop web applications.
Contrary to scientific software that would be used for decades, web
applications usually live for only months if not weeks.  As a result, web
developers tend to use any means to shorten the time to market, and become the
most active supporters of the open-source practice.  They routinely use
high-level languages for prototyping and the turn the prototype directly into a
product.

This prototype-as-product approach is promoted by recognizing two facts: (i)
Developers are expensive and (ii) computers are relatively cheap.  Prototyping
is not a new concept, but in the past, a high-level prototype is usually
reworked into a product, after the proof-of-concept, by using a different and
lower-level language.  The hasty pace of the web world broke the boundary
between prototypes and products to squeeze all the productivity out of the
workflow.  Optimization is postponed to a later stage of the development.

Although the experience of the web world cannot be equivalently copied into
scientific computing, the spirit should be borrowed.  What practitioners of
scientific computing care is the turn-around time to results, which includes
both development and running time of the software.  The new paradigm can
deliver reliable code in a shorter time than old approaches can.

As long as the performance hotspot in a prototype can be properly optimized, a
prototype-turned product can deliver ideal runtime performance.  It is not
uncommon for an optimized prototype system to exhibit the same runtime
performance as a system implemented by using solely a low-level language like
C, C++, or Fortran.  For example, some new projects have been developed by
combining Python and a low-level language, e.g., FEniCS/DOLFIN, FiPy, Kestrel,
and SOLVCON.  The hybrid approach is also favored by the heterogeneous
architecture, which intrinsically requires the developers to use multiple
programming languages.

Final Words
===========

Scientific computing has driven the development of digital computers from the
very beginning.  Computers are invented to solve technical problems, as the
infamous ENIAC, the first general-purpose electronic computer, born in 1946,
developed to calculated artillery firing tables.  In the era that hand-held
devices delivering the computing power unimaginable from a decade ago, the need
for scientific computing will continue to fuel the development in all aspects
of computation.  And we should write better code to do it.
