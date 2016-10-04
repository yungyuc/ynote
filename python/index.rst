:orphan:

======================
Introduction to Python
======================

.. contents::

.. _Python: http://www.python.org/
.. _Mercurial: http://mercurial.selenic.com/
.. _SCons: http://scons.org/
.. _Numpy: http://numpy.scipy.org/
.. _Scipy: http://www.scipy.org/
.. _Matplotlib: http://matplotlib.sourceforge.net/

Python_ is an easy-to-use, multi-paradigm programming language.  It is dynamic,
i.e., dynamically typed, and very productive.  It is described as
"batteries-included" for the abundant functionalities provided in its standard
library.  Python is especially suitable to high-performance computing (HPC)
since one of its design goals is to provide easy interfaces to C.

Python is important to the tool chain of my research work on computational
science.  I use many utilities and libraries built on Python:

- Distributed source control management: Mercurial_ (hg).
- Building system: SCons_.
- Multi-dimensional array: Numpy_.
- Scientific extension: Scipy_.
- Data analysis/visualization library: Matplotlib_ (mpl).

Version Control
===============

Research works need to be written out.  Modern research works usually involve a
team.  It is very common for a group of researchers to write papers together.
How can a group work on the manuscript effectively and efficiently?

The issue can be solved by using established tools in software engineering:
*source control management (SCM)*/*version control system (VCS)*.  Programmers
use the system to track computer code, while we can use it to track
manuscripts.  Mercurial_/hg is one of the most advanced SCM/VCS, and is very
suitable for research works.

To take advantage of hg or any other SCM/VCS, plain text files should be used,
i.e., **Tex/Latex** files.  SCM/VCS works best for plain text files.  SCM/VCS
usually cannot handle the complicated document formats, e.g., .doc, .docx,
.otf, etc.

hg can be installed on Debian using::

  $ apt-get install mercurial

For Windows users, go to http://tortoisehg.bitbucket.org/ to download the
latest version of TortoiseHg, which is an Explorer extension for hg.  Excellent
documentation is provided in the `HgBook
<http://hgbook.red-bean.com/read/behind-the-scenes.html>`_ and the `Mercurial
Wiki <http://mercurial.selenic.com/wiki/UnderstandingMercurial>`_.

Build System
============

In additional to texts, a research paper needs many things such as tables,
figures, references, etc.  They do not aggregate naturally.  A build system can
help organize those resources.  SCons_ can be used to automate the building
process of the document from various resources including Latex files, figures,
raw data, etc.  You can install it in Debian using::

  $ apt-get install scons

For Windows, download the latest installer at
http://www.scons.org/download.php.  Rich documentation can be found at
http://www.scons.org/documentation.php.

Numerical Analysis
==================

Even if you are not a numerical analyst, there's still a chance that you need
to crunch some numbers.  If you are not lucky enough to get the task done
solely with the Python standard library, you need help from high-performance,
easy-to-use numerical tools such as Numpy_ and Scipy_.

The word "high-performance" here is relative.  It is unlikely that a Python
program can outperforme C or FORTRAN for number-crunching.  What Numpy does is
to provide a nice set of functionalities to use pre-compiled C code for array
manipulation.  You can write high-level Python code to do what you want.  For
example::

  >>> from numpy import array
  >>> a = array([[0, 1], [2, 3]], dtype='float64')
  >>> b = a ** 2
  >>> print a+b
  [[  0.   2.]
   [  6.  12.]]

Numpy allows you to manipulate arrays pretty much in the same way that Matlab®
allows you to do (with different syntax).

Numpy was built as the core component of Scipy, which includes much more
external libraries for numerical/scientific analysis, e.g., ODE integration,
equation solving, interpolation, Fourier transform, linear algebra, statistics,
etc.

To install Numpy and/or Scipy in Debian::

  $ apt-get install python-numpy python-scipy

Windows users can download corresponding binaries at
http://www.scipy.org/Download.  Comprehensive documentation for Numpy and Scipy
can be found at http://docs.scipy.org/doc/numpy/ and
http://docs.scipy.org/doc/scipy/reference/, respectively.

Visualization
=============

Easy visualization of calculated data is one of the reasons that commercial
tools such as Matlab® are so popular among scientists and engineers.  Using
Matplotlib_, the de facto plotting toolkit in Python, you can do most of the
things that Matlab® can do with more flexibility.  You can install Matplotlib
in Debian using::

  $ apt-get install python-matplotlib

Windows binaries are available at
http://sourceforge.net/projects/matplotlib/files/.
