===================================
Survey Python Memory Analysis Tools
===================================

:date: 2015/3/4

Debugging memory consumption/leak is very interesting.  It needs a lot of
tools, and this is a note leading to them.  (Conclusion first: use
`pytracemalloc <http://pytracemalloc.readthedocs.org>`_ if you can.)

The available (C)Python memory analysis tools can be categorized into 4 approaches:

1. Use OS facilities.

   This approach doesn't really analyze the memory used by Python.  By
   reporting the memory information that the OS is tracking for the process
   (that is or uses Python), we get the most coarse but generic number.

   Example: `memory_profiler <https://pypi.python.org/pypi/memory_profiler>`__
   (note it's only available in Linux).

2. Use `the gc module <https://docs.python.org/2/library/gc.html>`__.

   It is convenient to use the ``gc`` module, but the drawback is it doesn't
   give a full list of objects in memory.  The (C)Python garbage collector
   `only tracks objects that potentially have cycle reference
   <http://www.arctrix.com/nas/python/gc/>`__ (related information: `BDFL's
   review
   <https://mail.python.org/pipermail/python-dev/2000-March/002385.html>`__).

   Examples: `pympler <https://code.google.com/p/pympler/>`__, `Meliae
   <https://pypi.python.org/pypi/meliae>`__, `objgraph
   <http://mg.pov.lt/objgraph/>`__.

3. Exhaustive traverse of objects.

   The third approach is interesting.  As far as I know, only `Heapy/Guppy-PE
   <http://guppy-pe.sourceforge.net>`__ takes this approach.  Heapy tries to
   traverse all objects from common roots, without changing (patching) the
   CPython low-level API.  The garbage collector is also used for
   complementary.  See http://guppy-pe.sourceforge.net/heapy-thesis.pdf for
   more information.

   However, this way still doesn't guarantee to retrieve all live objects.

4. Enhance (C)Python low-level APIs to monitor all memory allocation.

   It is the most comprehensive solution.  Once we hook into the Python
   allocation APIs we should be able to get all information.  The Python
   ``--with-pydebug`` configuration option works in this way, at a cost of `16
   more bytes (in 64-bit systems) per PyObject
   <https://hg.python.org/cpython/file/e74e2ce81a1c/Include/object.h#l66>`__.
   
   Examples: `PySizer <http://pysizer.8325.org>`__, pytracemalloc_.

   Note that PySizer is old and works quite differently from pytracemalloc.
   `It uses more memory and runs slower than pytracemalloc
   <https://www.python.org/dev/peps/pep-0454/#proposal>`__.  pytracemalloc uses
   the `PEP 445 <https://www.python.org/dev/peps/pep-0445/>`__ and is more
   compact, versatile, and extensible.

   The PEP 445 added a customizable system for changing memory allocator for
   Python.  The allocator can be changed in the runtime.  The PEP is a great
   enhancement.  It allows embedded Python to use the memory allocators from
   the host system in a systematic way.

pytracemalloc is the most complete memory analysis tool that I am aware of.  It
tracks all `the three domains of Python memory allocation
<https://github.com/haypo/pytracemalloc/blob/5f9d0514dff07863ed68c1ed56fbad866c5f1b43/_tracemalloc.c#L930>`__
(``RAW``, ``MEM``, and ``OBJ``, defined in PEP 445).  It can show `memory
statistics
<http://pytracemalloc.readthedocs.org/examples.html#display-the-top-10>`__ and
`traceback
<http://pytracemalloc.readthedocs.org/examples.html#get-the-traceback-of-a-memory-block>`__.
pytracemalloc can readily explain (i) how much memory is used and (ii) where it
is allocated.

On top of it, tools can be built to answer more questions, like who holds what
memory for how long.  Specific knowledge of the system to be analyzed is
required.

pytracemalloc supports 2.6, 2.7, and 3.3 but requires patches,
which are `included in the project
<https://github.com/haypo/pytracemalloc/tree/5f9d0514dff07863ed68c1ed56fbad866c5f1b43/patches>`__.
In Python 3.4 the module is `added to the standard library
<https://docs.python.org/3/library/tracemalloc.html>`__.

`The related discussions on issue tracker
<https://www.python.org/dev/peps/pep-0445/#links>`__ are very informative,
e.g.,

- http://bugs.python.org/issue3329
- http://bugs.python.org/issue18203

This survey is greatly helped by `the list of prior work
<https://www.python.org/dev/peps/pep-0454/#prior-work>`__ compiled by `Victor
Stinner <https://twitter.com/victor_stinner>`__ in the associated `PEP 454
<https://www.python.org/dev/peps/pep-0454/>`_.
