===============
Write Good Code
===============

:date: 2013/5/12

Computational scientists use computers to resolve difficult problems, but many
of us don't know how to write good computer programs.  This is ironic.  It will
be good if we can better engineer our code.

Good code is strong in three factors: (i) correctiveness, (ii) performance, and
(iii) productivity/maintainability.  For scientific applications,
correctiveness is the most important one.  It's pointless to produce unreliable
results in any discipline.  Sometimes erroneous results are even catastrophic,
e.g., `the Excel error
<http://www.huffingtonpost.com/2013/04/16/reinhart-rogoff-austerity-research-errors_n_3094015.html>`__.

It's not easy to write good code, but there are some basic principles to
achieve that:

1. Make your code look consistent and organized, or in another word, beautiful.
   Although beautiful things are not always right, ugly code is usually buggy.
   And inconsistent or unorganized code is definitely difficult to be
   maintained.  Follow a style convention as much as you can.

2. Always do version control.  A useful software project has at least thousands
   of lines of code and takes months to be completed.  The changes need to be
   tracked or we lose some information in the history.  Version control is also
   critically important for efficient working flows and collaboration.

3. Write appropriate, accurate, and abundant documents.  In the settings of
   modern research environment, writing papers is usually a central activity of
   research.  Documenting code should be granted the same value as
   paper-writing.  If we publish a paper that doesn't follow the code, we
   aren't doing a quality work.  The papers we published should be part of the
   code documents, as they support each other.

4. Test as much as possible.  Although testing is the core of code quality,
   itself alone isn't very helpful.  If we don't code with discipline, it's
   difficult to perform tests, and even if we can, the tests aren't effective.
   Tests should be alert to regressive errors in code.  If possible, it's good
   to make tests to be detective to potential flaws.

When I have time I want to write about systems and tools that can help us to
follow these principles in the context of computational sciences.
