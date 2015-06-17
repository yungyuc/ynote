=========================
Survey of Memory Managers
=========================

:date: 2015/6/17

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
