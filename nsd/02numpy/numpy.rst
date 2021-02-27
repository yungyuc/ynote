================
Python and Numpy
================

Python is easy to use and popular among scientists and engineers for its
simplicity.  It is suitable for almost every task, and the versatility makes is
one of the best tool as the programming interface for numerical applications.

Contents in this chapter:

.. contents::
  :local:
  :depth: 1

Organize Python Code
====================

To help the discussions for Python code organization, we make a simple
categorization:

* One file containing Python code as a **script**.
* One Python file is a ":term:`module <python:module>`".
* One directory containing Python files (satisfying some rules) is a
  ":term:`package <python:package>`".
* "Module" is usually used in a loose way to refer to things that may be
  imported by Python :ref:`import <python:import>` statement.  Then a "module"
  can mean (the strictly defined) module or a package.

Contents in this section:

.. contents::
  :local:
  :depth: 1

How a Python Script Works
+++++++++++++++++++++++++

A script is a text file that the program loader sends to an engine (usually
interpreter) to execute with the content.  We usually write scripts for
automating repetitive work, and they should be short for quick implementation.
Assume that we have a simple task: count the number of lines in a file.  Here
is a simple script to do it:

.. literalinclude:: code/step0.py
  :name: nsd-numpy-example-step0
  :caption:
    A Python script that counts lines in a file (:download:`step0.py
    <code/step0.py>`)
  :language: python
  :linenos:

Running a script requires the executable permission to be set.  Otherwise, the
script file cannot be executed in the shell as a command:

.. code-block:: console

  $ ls -al step0.py      # executable bit is not set
  -rw-r--r--  1 yungyuc  staff  574 Apr  7 22:18 step0.py

  $ ./step0.py pstake.py # can't run without permission
  -bash: ./step0.py: Permission denied

.. code-block:: console

  $ chmod a+x step0.py   # set the executable bit

  $ ls -al step0.py      # executable bit is set
  -rwxr-xr-x  1 yungyuc  staff  574 Apr  7 22:18 step0.py*

  $ ./step0.py pstake.py # properly it runs
  811 lines in pstake.py

If we do not need to run the script like an executable, it can be run by
explicitly calling Python:

.. code-block:: console

  $ chmod u-x step0.py

  $ python3 step0.py pstake.py
  811 lines in pstake.py

Python Script Shebang
---------------------

The leading line in a script that starts with ``#!`` is called the *shebang*.
It tells the program loader which executable to run for the script.  For a
Python script, `it is recommended
<https://www.python.org/dev/peps/pep-0394/#for-python-script-publishers>`__ to
use ``/usr/bin/env python3`` in the shebang.  It allows the interactive shell
to search for the Python binary configured by the user.

.. code-block:: python

  #!/usr/bin/env python3
  # ...

Modeline
--------

At the end of the script we have the `vim modeline magic
<https://vim.fandom.com/wiki/Modeline_magic>`__:

.. code-block:: python

  ...
  # vim: set ff=unix ft=python et sw=4 ts=4 sts=4:

A script runs without it, but it may be used to specify how the file should be
formatted in the editor.  Vim runs the command in the modeline when reading a
file having it.  The modeline is useful to specify how the source code is
formatted and provide the minimal information about the coding convention.  The
modeline should not contain commands that changes the global or cosmetic
behaviors, e.g., show or hide the line numbers.  The above example does the
following:

``ff=unix``
  Use ``<LF>`` to for the end-of-line character.

``ft=python``
  Use Python syntax highlighting.

``et``
  Expand tab characters.

``sw=4``
  Number of spaces for each step of indent.

``ts=4``
  Number of spaces for a tab.

``sts=4``
  Number of spaces for a tab during editing.

Emacs has a similar feature called `file variable
<https://www.gnu.org/software/emacs/manual/html_node/emacs/Specifying-File-Variables.html>`__:

.. code-block:: lisp

  ...
  ;; -*- mode: Lisp; fill-column: 75; comment-column: 50; -*-

One-Liner
---------

Python executable supports the ``-c`` argument for one-liner.  The content of
the script is passed from the command line.  It's called one-liner because it
usually only takes one line.

One-liners are convenient for code that is only run once.  Quick to write but
hard to read.

.. code-block:: console

  $ python3 -c 'print(len(open("pstake.py").readlines()), "lines")'
  811 lines

Make a Module
+++++++++++++

A :term:`Python module <python:module>` is pretty much a Python source file.  A
Python script is also a file containing Python code.  They differ in the way
the code is invoked.  A script is used as a command.  A module is imported as a
library.

Can a script be used as a module?  It depends on how it is written.  For
example, the script :ref:`step0.py <nsd-numpy-example-step0>` is not suitable
for being imported as a module:

.. code-block:: console

  $ python3 -c 'import step0'
  missing file name

The :ref:`import <python:import>` statement runs the Python source code and
puts the results in the module namespace.  However, the code in :ref:`step0.py
<nsd-numpy-example-step0>` simply does the work without leaving anything useful
in the module to be imported.

We should modify the script to be make it suitable for a module:

.. literalinclude:: code/step1.py
  :name: nsd-numpy-example-step1
  :caption:
    A Python module file (:download:`step1.py <code/step1.py>`)
  :language: python
  :linenos:

Because a module is not supposed to be run as a command, the file no longer has
a shebang.  In addition to that, we made two changes.  First, the module file
factors out the line-counting code from the script example:

.. code-block:: python

  if os.path.exists(fname):
      with open(fname) as fobj:
          lines = fobj.readlines()
      sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
  else:
      sys.stdout.write('{} not found\n'.format(fname))

into a function:

.. code-block:: python

  def count_line(fname):
      if os.path.exists(fname):
          with open(fname) as fobj:
              lines = fobj.readlines()
          sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
      else:
          sys.stdout.write('{} not found\n'.format(fname))

Second, the rest of the code is moved into an ``if`` test:

.. code-block:: python

  # This tests whether the code is evaluated as a script.
  if __name__ == '__main__':
      if len(sys.argv) < 2:
          sys.stdout.write('missing file name\n')
      elif len(sys.argv) > 2:
          sys.stdout.write('only one argument is allowed\n')
      else:
          count_line(sys.argv[1])

.. note::

  :py:mod:`__main__ <python:__main__>` is the name of the scope in which the
  top-level code execute in Python.

After the change, :py:mod:`!step1` acts like a module.  When it is imported,
nothing happens in the calling site:

.. code-block:: console

  $ python3 -c 'import step1'

To run the code defined in the :py:mod:`!step1` module, the function
:py:func:`!count_line` should be explicitly called:

.. code-block:: console

  $ python3 -c 'import step1 ; step1.count_line("pstake.py")'
  811 lines in pstake.py

While modifying the file, we keep the capability to run the file as a script:

.. code-block:: console

  $ python3 step0.py pstake.py
  811 lines in pstake.py
  $ python3 step1.py pstake.py
  811 lines in pstake.py

Move Everything inside Function
-------------------------------

We can improve the module by factoring out all code to functions.  See the
following example:

.. literalinclude:: code/step2.py
  :name: nsd-numpy-example-step2
  :caption:
    Move all code to functions (:download:`step2.py <code/step2.py>`)
  :language: python
  :linenos:

It moves the argument-processing code to the function :py:func:`!main`:

.. code-block:: python

  def main():
      if len(sys.argv) < 2:
          sys.stdout.write('missing file name\n')
      elif len(sys.argv) > 2:
          sys.stdout.write('only one argument is allowed\n')
      else:
          count_line(sys.argv[1])


  # This tests whether the file is evaluated as a script.
  if __name__ == '__main__':
      main()

:ref:`step1.py <nsd-numpy-example-step1>` and :ref:`step2.py
<nsd-numpy-example-step2>` have the same behavior, except :ref:`step2.py
<nsd-numpy-example-step2>` has a :py:func:`!main` function, which allows it to
behave like a script:

.. code-block:: console

  $ # only import the module
  $ python3 -c 'import step2'
  $ # import and then run the new main function
  $ python3 -c 'import step2 ; step2.main()' pstake.py
  811 lines in pstake.py

Run Module as Script
--------------------

If there is a module, Python allows to run the module as a script without
knowing where the module file is.  The functionality is supported with the
``-m`` argument.

.. code-block:: console

  $ python3 -m step1 pstake.py
  811 lines in pstake.py

Make a Package
++++++++++++++

When the code grows to a point, you may need a directory to house it.  Let's
use our simple example to show how to make a package:

* ``step3/``

  * :download:`__init__.py <code/step3/__init__.py>`
  * :download:`__main__.py <code/step3/__main__.py>`
  * :download:`_core.py <code/step3/_core.py>`

No file in the package version :py:mod:`!step3` can be run as a script.

.. code-block:: console

  $ # The package __init__.py doesn't work like a module.
  $ python3 step3/__init__.py numpy.ipynb
  Traceback (most recent call last):
    File "step3/__init__.py", line 11, in <module>
      from ._core import count_line
  ImportError: attempted relative import with no known parent package

Everything else remains working, including the ``-m`` option of Python
executable.

.. code-block:: console

  $ python3 -m step3 numpy.ipynb
  1512 lines in numpy.ipynb
  $ python3 -c 'import step3 ; step3.main()' numpy.ipynb
  1512 lines in numpy.ipynb

A Real Useful Script
++++++++++++++++++++

Here is a real-world example (:download:`pstake.py <code/pstake.py>`) for how
to write a useful script: convert `pstricks
<http://tug.org/PSTricks/main.cgi/>`__ to an image file.

.. code-block:: tex
  :caption: Example PSTricks TeX file :download:`code/cce.tex`.

  \psset{unit=2cm}
  \begin{pspicture}(-3,-.5)(3,1.5)
    \psset{linewidth=1pt}
    \psset{linecolor=black}
    \psframe(-1,0)(1,1)
    \psline[linestyle=dashed](0,0)(0,1)
    \psframe[linestyle=dotted,linecolor=blue](-0.95,0.05)(-0.05,0.95)
    \rput[t](-0.5,0.9){$\mathrm{CE}_-$}
    \psframe[linestyle=dotted,linecolor=blue](0.05,0.05)(0.95,0.95)
    \rput[t](0.5,0.9){$\mathrm{CE}_+$}
    \psframe[linestyle=dotted,linecolor=red](-1.05,-0.05)(1.05,1.05)
    \rput[tr](-1.1,0.9){$\mathrm{CE}$}
    \psdots[dotstyle=*](0,1)(-1,1)(-1,0)(0,0)(1,0)(1,1)(0,1)
    \uput{0.1}[u](0,1){A $(x_j,t^n)$}
    \uput{0.1}[ul](-1,1){B}
    \uput{0.1}[dl](-1,0){$(x_{j-\frac{1}{2}},t^{n-\frac{1}{2}})$ C}
    \uput{0.1}[d](0,0){D}
    \uput{0.1}[dr](1,0){E $(x_{j+\frac{1}{2}},t^{n-\frac{1}{2}})$}
    \uput{0.1}[ur](1,1){F}
    \psline[linestyle=dashed](0,0)(0,1)
  \end{pspicture}

.. code-block:: console

  $ rm -f cce.png
  $ ./pstake.py cce.tex cce.png 2>&1 > /dev/null

.. figure:: image/cce.png
  :align: center

Numpy for Array-Centric Code
============================

* Arrays are the best container to manage homogeneous data.
* The `numpy <http://www.numpy.org/>`__ library provides everything we need for
  arrays in Python.
* Arrays use contiguous memory, sequences don't.

.. code-block:: pycon

  >>> # Make a list (one type of Python sequence) of integers.
  >>> lst = [1, 1, 2, 3, 5]
  >>> print('A list:', lst)
  A list: [1, 1, 2, 3, 5]
  >>> # Import the numpy library. It's a universal convention to alias it to "np".
  >>> import numpy as np
  >>> # Make an array from the sequence.
  >>> array = np.array(lst)
  >>> print('An array:', np.array(array))
  An array: [1 1 2 3 5]

Key Meta-Data
+++++++++++++

.. code-block:: pycon

  >>> array = np.array([[0, 1, 2], [3, 4, 5]])
  >>> print("shape:", array.shape)
  shape: (2, 3)
  >>> print("size:", array.size)
  size: 6
  >>> print("nbytes:", array.nbytes)
  nbytes: 48
  >>> print("itemsize:", array.itemsize)
  itemsize: 8
  >>> print("dtype:", array.dtype)
  dtype: int64

Data Type
+++++++++

The numpy array is of type :py:class:`numpy:numpy.ndarray`.  It has a property
:py:attr:`~numpy:numpy.ndarray.dtype` for the data type the array uses:

.. code-block:: pycon

  >>> print(type(array))
  <class 'numpy.ndarray'>
  >>> print(array.dtype)
  int64

:py:func:`numpy:numpy.array` is the most basic constructor (factor function)
for :py:class:`~numpy:numpy.ndarray`.  It detects the types in the input
sequence data and choose the appropriate :py:class:`~numpy:numpy.dtype` for the
constructed array.

.. code-block:: pycon

  >>> array1 = np.array([1, 1, 2, 3, 5]) # only integer
  >>> print("only int:", array1, type(array1), array1.dtype)
  only int: [1 1 2 3 5] <class 'numpy.ndarray'> int64
  >>> array2 = np.array([1.0, 1.0, 2.0, 3.0, 5.0]) # only real
  >>> print("only real:", array2, type(array2), array2.dtype)
  only real: [1. 1. 2. 3. 5.] <class 'numpy.ndarray'> float64
  >>> array3 = np.array([1, 1, 2, 3, 5.0]) # integer and real
  >>> print("int and real:", array3, type(array3), array3.dtype)
  int and real: [1. 1. 2. 3. 5.] <class 'numpy.ndarray'> float64

* A Python list doesn't know the type it contains, but an array does.
* The type information allows numpy to process the array data using
  pre-compiled C code.

Construction
++++++++++++

Numpy provides a lot of helpers to construct arrays (see
:doc:`numpy:reference/routines.array-creation`).  The 3 most common
constructors are :py:func:`numpy:numpy.empty`, :py:func:`numpy:numpy.zeros`,
and :py:func:`numpy:numpy.ones`:

.. code-block:: pycon

  >>> empty_array = np.empty(4)
  >>> print("It will contain garbage, but it doesn't waste time to initialize:", empty_array)
  It will contain garbage, but it doesn't waste time to initialize: [9.26744491e+242 3.74168445e+233 1.94950106e-057 3.47526968e-309]
  >>> zeroed_array = np.zeros(4)
  >>> print("The contents are cleared with zeros:", zeroed_array)
  The contents are cleared with zeros: [0. 0. 0. 0.]
  >>> unity_array = np.ones(4)
  >>> print("Instead of zeros, fill it with ones:", unity_array)
  Instead of zeros, fill it with ones: [1. 1. 1. 1.]
  >>> print("All of their data types are float64 (double-precision floating-point):",
  ...       empty_array.dtype, zeroed_array.dtype, unity_array.dtype)
  All of their data types are float64 (double-precision floating-point): float64 float64 float64

:py:func:`numpy:numpy.full` is a shorthand for :py:func:`~numpy:numpy.empty`
and :py:meth:`numpy:numpy.ndarray.fill`:

.. code-block:: pycon

  >>> empty_array = np.empty(4)
  >>> empty_array.fill(7)
  >>> print("Create an empty array and fill the value:", empty_array)
  Create an empty array and fill the value: [7. 7. 7. 7.]
  >>> filled_array = np.full(4, 7)
  >>> print("Build an array populated with an arbitrary value:", filled_array)
  Build an array populated with an arbitrary value: [7 7 7 7]
  >>> filled_real_array = np.full(4, 7.0)
  >>> print("Build an array populated with an arbitrary real value:", filled_real_array)
  Build an array populated with an arbitrary real value: [7. 7. 7. 7.]

:py:func:`numpy:numpy.arange` builds a monotonically increasing array:

.. code-block:: pycon

  >>> ranged_array = np.arange(4)
  >>> print("Build an array with range:", ranged_array)
  Build an array with range: [0 1 2 3]
  >>> ranged_real_array = np.arange(4.0)
  >>> print("Build with real range:", ranged_real_array)
  Build with real range: [0. 1. 2. 3.]

:py:func:`numpy:numpy.linspace` returns an array whose elements are evenly
placed in a closed interval:

.. code-block:: pycon

  >>> linear_array = np.linspace(11, 13, num=6)
  >>> print("Create an equally-spaced array with 6 elements:", linear_array)
  Create an equally-spaced array with 6 elements: [11.  11.4 11.8 12.2 12.6 13. ]

Multi-dimensional arrays
++++++++++++++++++++++++

Multi-dimensional arrays are the building-block of matrices and linear algebra.
Much more useful than one-dimensional arrays.

Create multi-dimensional arrays by stacking 1D:

.. code-block:: pycon

  >>> ranged_array = np.arange(10)
  >>> print("A 1D array:", ranged_array)
  A 1D array: [0 1 2 3 4 5 6 7 8 9]
  >>> hstack_array = np.hstack([ranged_array, ranged_array])
  >>> print("Horizontally stacked array:", hstack_array)
  Horizontally stacked array: [0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9]
  >>> vstack_array = np.vstack([ranged_array, ranged_array+100])
  >>> print("Vertically stacked array:", vstack_array)
  Vertically stacked array: [[  0   1   2   3   4   5   6   7   8   9]
   [100 101 102 103 104 105 106 107 108 109]]


:py:class:`~numpy:numpy.ndarray` by default is row-majoring ("C"-style):

.. math::

  A = \left(\begin{array}{ccc}
    a_{00} & a_{01} & a_{02} \\
    a_{10} & a_{11} & a_{12}
  \end{array}\right)
  = \left(\begin{array}{ccc}
    0 & 1 & 2 \\
    3 & 4 & 5
  \end{array}\right)

.. code-block:: pycon

  >>> original_array = np.arange(6)
  >>> print("original 1D array:", original_array)
  original 1D array: [0 1 2 3 4 5]

.. code-block:: pycon

  >>> print("reshaped 2D array:\n%s" % original_array.reshape((2,3)))
  reshaped 2D array:
  [[0 1 2]
   [3 4 5]]

Column-majoring ("F"-style):

.. code-block:: pycon

  >>> print("reshaped 2D array:\n%s" % original_array.reshape((2,3), order='f'))
  reshaped 2D array:
  [[0 2 4]
   [1 3 5]]

Example for 3D arrays:

.. code-block:: pycon

  >>> original_array = np.arange(24)
  >>> print("original 1D array:\n%s" % original_array)
  original 1D array:
  [ 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23]

.. code-block:: pycon

  >>> reshaped_array = original_array.reshape((2,3,4))
  >>> print("reshaped 3D array:\n%s" % reshaped_array)
  reshaped 3D array:
  [[[ 0  1  2  3]
    [ 4  5  6  7]
    [ 8  9 10 11]]

   [[12 13 14 15]
    [16 17 18 19]
    [20 21 22 23]]]

For multi-dimensional arrays, operations can be done along any of the axes.

For summing the above array of shape (2, 3, 4) along the 0-th axis, the
calculation is:

.. math::

  a_{jk} = \sum_{i=0}^1a_{ijk} ,\; j=0, 1, 2; \; k=0, 1, 2, 3

The resulting array has shape (3, 4).

.. code-block:: pycon

  >>> print("Summation along 0th axis:\n%s" % reshaped_array.sum(axis=0))
  Summation along 0th axis:
  [[12 14 16 18]
   [20 22 24 26]
   [28 30 32 34]]

For summing the 1-st axis, the calculation is:

.. math::
  a_{ik} = \sum_{j=0}^2a_{ijk} ,\; i=0, 1; \; k=0, 1, 2, 3

The resulting array has shape (2, 4).

.. code-block:: pycon

  >>> print("Summation along 1st axis:\n%s" % reshaped_array.sum(axis=1))
  Summation along 1st axis:
  [[12 15 18 21]
   [48 51 54 57]]

Selection: Extract Sub-Array
++++++++++++++++++++++++++++

There are 3 ways to create sub-arrays:

1. Slicing
2. Integer indexing
3. Boolean indexing

Slicing
+++++++

The array created from slicing shares the buffer of the original one:

.. code-block:: pycon

  >>> array = np.arange(10)
  >>> print("This is the original array:", array)
  This is the original array: [0 1 2 3 4 5 6 7 8 9]
  >>>
  >>> sub_array = array[:5]
  >>> print("This is the sub-array:", sub_array)
  This is the sub-array: [0 1 2 3 4]
  >>>
  >>> sub_array[:] = np.arange(4, -1, -1)
  >>> print("The sub-array is changed:", sub_array)
  The sub-array is changed: [4 3 2 1 0]
  >>>
  >>> print("And the original array is changed too (!):", array)
  And the original array is changed too (!): [4 3 2 1 0 5 6 7 8 9]

New buffer can be created by copying the returned array:

.. code-block:: pycon

  >>> array = np.arange(10.0)
  >>> print("Recreate the original array to show how to avoid this:", array)
  Recreate the original array to show how to avoid this: [0. 1. 2. 3. 4. 5. 6. 7. 8. 9.]
  >>>
  >>> # Make a copy from the slice.
  >>> sub_array = array[:5].copy()
  >>> sub_array[:] = np.arange(4, -1, -1)
  >>> print("The sub-array is changed, again:", sub_array)
  The sub-array is changed, again: [4. 3. 2. 1. 0.]
  >>> print("But original array remains the same:", array)
  But original array remains the same: [0. 1. 2. 3. 4. 5. 6. 7. 8. 9.]

Slice one dimension in a multi-dimensional array:

.. code-block:: pycon

  >>> array = np.arange(24).reshape((2,3,4))
  >>> print("orignal:\n%s" % array)
  orignal:
  [[[ 0  1  2  3]
    [ 4  5  6  7]
    [ 8  9 10 11]]

   [[12 13 14 15]
    [16 17 18 19]
    [20 21 22 23]]]
  >>> array[:,1,3] = np.arange(300,302)
  >>> print("find 300, 301:\n%s" % array)
  find 300, 301:
  [[[  0   1   2   3]
    [  4   5   6 300]
    [  8   9  10  11]]

   [[ 12  13  14  15]
    [ 16  17  18 301]
    [ 20  21  22  23]]]

Slice two dimensions in a multi-dimensional array:

.. code-block:: pycon

  >>> array = np.arange(24).reshape((2,3,4))
  >>> print("orignal:\n%s" % array)
  orignal:
  [[[ 0  1  2  3]
    [ 4  5  6  7]
    [ 8  9 10 11]]

   [[12 13 14 15]
    [16 17 18 19]
    [20 21 22 23]]]
  >>> array[:,0,:] = np.arange(200,208).reshape((2,4))
  >>> print("find the number [200,208):\n%s" % array)
  find the number [200,208):
  [[[200 201 202 203]
    [  4   5   6   7]
    [  8   9  10  11]]

   [[204 205 206 207]
    [ 16  17  18  19]
    [ 20  21  22  23]]]

Integer Indexing
++++++++++++++++

.. code-block:: pycon

  >>> array = np.arange(100, 106)
  >>> slct = np.array([1, 3])
  >>> print("select by indice 1, 3:", array[slct])
  select by indice 1, 3: [101 103]
  >>> slct = np.array([0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5])
  >>> print("new array is bigger than the old one:", array[slct])
  new array is bigger than the old one: [100 100 101 101 102 102 103 103 104 104 105 105]
  >>> array2 = array.reshape((2,3))
  >>> slct = np.array([1])
  >>> print("select by indice 1:", array2[slct])
  select by indice 1: [[103 104 105]]
  >>> slct = np.array([[0,0], [0,1], [1,2]])
  >>> print("select by indice (0,0), (0,1), (1,2):", array2[slct[:,0], slct[:,1]],
  ...       "using", slct)
  select by indice (0,0), (0,1), (1,2): [100 101 105] using [[0 0]
   [0 1]
   [1 2]]

Boolean Selection
+++++++++++++++++

The Boolean arrays filter wanted or unwanted elements in another array.

.. code-block:: pycon

  >>> less_than_5 = ranged_array < 5
  >>> print("The mask for less than 5:", less_than_5)
  The mask for less than 5: [ True  True  True  True  True False False False False False]
  >>> print("The values that are less than 5", ranged_array[less_than_5])
  The values that are less than 5 [0 1 2 3 4]
  >>>
  >>> all_on_mask = np.ones(10, dtype='bool')
  >>> print("All on mask:", all_on_mask)
  All on mask: [ True  True  True  True  True  True  True  True  True  True]
  >>>
  >>> all_off_mask = np.zeros(10, dtype='bool')
  >>> print("All off mask:", all_off_mask)
  All off mask: [False False False False False False False False False False]

Broadcasting
++++++++++++

:ref:`Broadcasting <numpy:ufuncs.broadcasting>` handles arrays of different
shapes participating in an operation.

1. All input arrays with number of dimension smaller than the input array of largest number of dimension, have 1’s prepended to their shapes.
2. The size in each dimension of the output shape is the maximum of all the input sizes in that dimension.
3. An input can be used in the calculation if its size in a particular dimension either matches the output size in that dimension, or has value exactly 1.
4. If an input has a dimension size of 1 in its shape, the first data entry in that dimension will be used for all calculations along that dimension.

.. code-block:: pycon

  >>> a = np.arange(2); print("a =", a)
  a = [0 1]
  >>> b = np.arange(10,12); print("b =", b)
  b = [10 11]
  >>> print("a+b =", a+b) # good: same shape
  a+b = [10 12]
  >>> c = np.arange(3); print("c =", c)
  c = [0 1 2]
  >>> print(a+c) # bad: different shape
  Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
  ValueError: operands could not be broadcast together with shapes (2,) (3,)

.. code-block:: pycon

  >>> a = np.arange(5,7).reshape((2,1))
  >>> b = np.arange(10,13).reshape((1,3))
  >>> print("a:\n%s, shape=%s" % (a, a.shape))
  a:
  [[5]
   [6]], shape=(2, 1)
  >>> print("b:\n%s, shape=%s" % (b, b.shape))
  b:
  [[10 11 12]], shape=(1, 3)
  >>> r = a*b
  >>> print("a*b:\n%s, shape=%s" % (r, r.shape))
  a*b:
  [[50 55 60]
   [60 66 72]], shape=(2, 3)

.. note::

  Broadcasting is a powerful tool.  It allows to write complex array
  calculation.  The down side is that the code may usually be too complex to
  understand.  Oftentimes element-wise code is much more maintainable than
  broadcasting code.

Python Tools for Numerical Analysis
===================================

There are two equally important activities for software development.  One is to
write code.  We will need to learn some basic concepts to write meaningful
code.

The other is to use code written by other people.  Especially in the early
stage of development, we want to quickly see the results.  We may just use the
results of other software.  We may directly incorporate the foreign (usually,
also called "third-party") software, if the situation allows.  Otherwise, we
can replace the quick prototype in a later phase.

In this lecture, I will introduce 3 useful tools for numerical analysis that
you may use throughout the course and your future work.

Drawing Using Matplotlib
++++++++++++++++++++++++

`Matplotlib <https://matplotlib.org>`__ is a library for 2D plotting.  It can
be used standalone or integrated with Jupyter notebook.

The recipe of (blindly) using matplotlib:

0. Visit the gallery: https://matplotlib.org/gallery/index.html.  Pick the
   category of the plot you want to make.
1. Copy the example code and run.
2. Modify the example to what you want.

Demonstration:
https://matplotlib.org/gallery/lines_bars_and_markers/multicolored_line.html#sphx-glr-gallery-lines-bars-and-markers-multicolored-line-py

.. code-block:: python

  import numpy as np
  import matplotlib.pyplot as plt
  from matplotlib.collections import LineCollection
  from matplotlib.colors import ListedColormap, BoundaryNorm

  x = np.linspace(0, 3 * np.pi, 500)
  y = np.sin(x)
  dydx = np.cos(0.5 * (x[:-1] + x[1:]))  # first derivative

  # Create a set of line segments so that we can color them individually
  # This creates the points as a N x 1 x 2 array so that we can stack points
  # together easily to get the segments. The segments array for line collection
  # needs to be (numlines) x (points per line) x 2 (for x and y)
  points = np.array([x, y]).T.reshape(-1, 1, 2)
  segments = np.concatenate([points[:-1], points[1:]], axis=1)

  fig, axs = plt.subplots(2, 1, sharex=True, sharey=True)

  # Create a continuous norm to map from data points to colors
  norm = plt.Normalize(dydx.min(), dydx.max())
  lc = LineCollection(segments, cmap='viridis', norm=norm)
  # Set the values used for colormapping
  lc.set_array(dydx)
  lc.set_linewidth(2)
  line = axs[0].add_collection(lc)
  fig.colorbar(line, ax=axs[0])

  # Use a boundary norm instead
  cmap = ListedColormap(['r', 'g', 'b'])
  norm = BoundaryNorm([-1, -0.5, 0.5, 1], cmap.N)
  lc = LineCollection(segments, cmap=cmap, norm=norm)
  lc.set_array(dydx)
  lc.set_linewidth(2)
  line = axs[1].add_collection(lc)
  fig.colorbar(line, ax=axs[1])

  axs[0].set_xlim(x.min(), x.max())
  axs[0].set_ylim(-1.1, 1.1)
  plt.show()

.. figure:: image/mplplot.png
  :align: center
  :width: 20em

Linear Algebra with Numpy
+++++++++++++++++++++++++

Numpy provides wrappers for BLAS and LAPACK and can readily be used for solving
linear systems.  For example, consider the system:

.. math::

  3x_1 +  x_2 + 5x_3 &= 9 \\
   x_1 + 2x_2 +  x_3 &= 8 \\
  4x_1 + 3x_2 +  x_3 &= 2

.. code-block:: pycon

  >>> a = np.array([[3,1,5], [1,2,1], [4,3,1]])
  >>> b = np.array([9,8,2])
  >>> x = np.linalg.solve(a, b)
  >>> print(x)
  [-3.4  4.2  3. ]
  >>> print(np.dot(a, x))
  [9. 8. 2.]

See also :doc:`numpy:references/routines.linalg`.

Package Managers
++++++++++++++++

To write code we need a runtime environment that has the dependency software
installed.  Although manually building all the dependencies from source is
sometimes unavoidable, it's too time-consuming to do it always.

Usually we will use a package manager to help.  A package manager provides
recipes for building package from source, and also pre-built binary packages.
It defines the dependencies between the packages.  For example, for scipy to
work, numpy needs to be installed beforehand.  A package manager should allow
automatic installation of numpy when you request scipy.

In the numerical analysis world, `conda <https://conda.io/>`__ is one of the
most versatile package manager that we will use.  There are two major sources
of packages:

* Anaconda: https://www.anaconda.com, an open and commercially maintained
  package repository.
* Conda-forge: https://conda-forge.org, a community maintained package
  repository.

In addition to conda, `pip <https://pip.pypa.io/>`__ is another popular choice.
pip is the package installer for Python.  You can use pip to install packages
from the `Python Package Index <https://pypi.org/`__ and other indexes.

Exercises
=========

1. List all primitive types supported by :py:class:`numpy:numpy.ndarray` on
   x86-64.
2. Port "``step0.py``" to use bash.
3. Modify the script "``step0.py``" so that it reads the environment variable
   named "``PYTHON_BIN``" that specifies the location of the Python executable
   for the script.  Hint: play a trick (or tricks) using bash, and note it's
   possible to write no-op command in bash.

References
==========

.. [1] [Broadcasting arrays in
   Numpy](https://eli.thegreenplace.net/2015/broadcasting-arrays-in-numpy/) by
   Eli Bendersky

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
