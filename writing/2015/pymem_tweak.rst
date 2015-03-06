=================================
Tweaking Python Memory Management
=================================

:date: 2015/3/6

Linux glibc `often hesitates to return memory to the OS
<http://bugs.python.org/issue11849>`__.  The mechanism is `well explained
<http://bugs.python.org/issue11849#msg134992>`__ in the issue tracker
discussions.  As such, CPython 3.3 changed to use :c:func:`mmap` to allocate
arenas: https://hg.python.org/cpython/rev/f8a697bc3ca8/ and
https://hg.python.org/cpython/rev/e7aa72e6aad4/.  The enhancement was later
back-ported to 2.7: https://hg.python.org/cpython/rev/4e43e5b3f7fc/.

Alternatively, to resolve the wild glibc, one can use another memory allocator.
Guys at ESN tried `tcmalloc
<http://goog-perftools.sourceforge.net/doc/tcmalloc.html>`__ and `found it is
useful <http://pushingtheweb.com/2010/06/python-and-tcmalloc/>`__.  For
debugging memory issues tcmalloc looks like a good tool, as it offers `quite
nice heap analyzer
<http://goog-perftools.sourceforge.net/doc/tcmalloc.html>`__.
