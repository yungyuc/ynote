=================
Memory Management
=================

:date: 2015/6/17, 2015/6/26

Existing POSIX Memory Managers
==============================

Dynamic Storage Allocation: A Survey and Critical Review:
http://www.cs.northwestern.edu/~pdinda/icsclass/doc/dsa.pdf

A collection of memory allocator implementations:
https://github.com/emeryberger/Malloc-Implementations

Surveyed memory managers:

- dlmalloc (Doug Lea's malloc): http://g.oswego.edu/dl/html/malloc.html
  
  This malloc implementation led to many successors including ptmalloc2.
- ptmalloc2 (glibc malloc):
  http://sourceware.org/git/?p=glibc.git;a=blob;f=malloc/malloc.c;h=452f036387e0b5699e8f0fa33ed027abf066115e;hb=HEAD
- tcmalloc (thread-caching malloc in Google performance tools):
  http://gperftools.googlecode.com/git/doc/tcmalloc.html
- OSX malloc:
  http://www.opensource.apple.com/source/libmalloc/libmalloc-53.1.1/

Other notes:

- Ulrich Drepper wrote about statistical features of malloc() in glibc 2.10:
  http://udrepper.livejournal.com/20948.html

Enhancement to the New Python Memeory Allocator (for calloc)
=============================================================

On 2014/4/14, Nathaniel Smith proposed to add calloc() to the new Python memory
manager (PEP-445):
https://mail.python.org/pipermail/python-dev/2014-April/133985.html .  The
`issue 21233 <http://bugs.python.org/issue21233>`__ was then `opened by
Nathaniel
<https://mail.python.org/pipermail/python-dev/2014-April/134026.html>`__.
Victor Stinner implemented it and the additional API is planned to be included
into Python 3.5.

A related discussion thread in the numpy-discussion mailing list:
http://mail.scipy.org/pipermail/numpy-discussion/2014-April/069935.html (on
2014/4/15).

Later on, Antoine Pitrou inquired why PyDataMem\_\* APIs still uses vanilla
malloc/calloc.  Nathaniel `responded
<http://mail.scipy.org/pipermail/numpy-discussion/2015-January/072070.html>`__
and pointed to `a related NumPy GitHub issue
<https://github.com/numpy/numpy/issues/4663>`__.

A related discussion on Python bug track about aligned memory allocation:
http://bugs.python.org/issue18835 .  There is also a related NumPy issue:
https://github.com/numpy/numpy/issues/5312 .
