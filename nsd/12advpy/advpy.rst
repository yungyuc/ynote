===============
Advanced Python
===============

Iterator
========

While processing data in memory, they are iterated one by one.  Assume we have
10 elements in a list.

.. code-block:: pycon

  >>> data = list(range(10))
  >>> print(data, type(data))

Python uses the :ref:`iterator protocol <python:typeiter>` to get one element a
time:

.. code-block:: python

  class ListIterator:

      def __init__(self, data, offset=0):
          self.data = data
          self.it = None
          self.offset = offset

      def __iter__(self):
          return self

      def __next__(self):
          if None is self.it:
              self.it = 0
          elif self.it >= len(self.data)-1:
              raise StopIteration
          else:
              self.it += 1
          return self.data[self.it] + self.offset

.. code-block:: pycon

  list_iterator = ListIterator(data)

The ``for ... in ...`` construct applies to the iterator object.  Every time
the construct needs the next element, ``ListIterator.__next__()`` is called:

.. code-block:: pycon

  >>> for i in list_iterator:
  >>>     print(i)
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9

List comprehension:

.. code-block:: pycon

  >>> print([value+100 for value in data])
  [100, 101, 102, 103, 104, 105, 106, 107, 108, 109]
  >>> print(list_iterator)
  <__main__.ListIterator object at 0x10cfaebd0>
  >>> print(dir(list_iterator))
  ['__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__',
  '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__',
  '__init_subclass__', '__iter__', '__le__', '__lt__', '__module__', '__ne__',
  '__new__', '__next__', '__reduce__', '__reduce_ex__', '__repr__',
  '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__',
  'data', 'it', 'offset']
  
Of course, you don't really need to write your own ``ListIterator`` for
iterating a list, because Python builds in an iterator already:

.. code-block:: pycon

  >>> list_iterator2 = iter(data)
  >>> for i in list_iterator2:
  >>>     print(i)
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9
  >>> print(list_iterator2)
  <list_iterator object at 0x10cfb2990>
  >>> print(dir(list_iterator2))
  ['__class__', '__delattr__', '__dir__', '__doc__', '__eq__', '__format__',
  '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__',
  '__init_subclass__', '__iter__', '__le__', '__length_hint__', '__lt__',
  '__ne__', '__new__', '__next__', '__reduce__', '__reduce_ex__', '__repr__',
  '__setattr__', '__setstate__', '__sizeof__', '__str__', '__subclasshook__']

The built-in iterator is created by calling
:py:meth:`python:container.__iter__` method on the container object
(:py:func:`python:iter` simply does it for you):

.. code-block:: pycon

  >>> list_iterator3 = data.__iter__()
  >>> print(list_iterator3)
  <list_iterator object at 0x10cfbab90>

And the ``for ... in ...`` construct actually knows about the :ref:`iterator
protocol <python:typeiter>`:

.. code-block:: pycon

  >>> for i in data:
  >>>     print(i)
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9

List Comprehension
++++++++++++++++++

List comprehension is the construct ``[... for ... in ...]``.  Python borrowed
the syntax of list comprehension from other languages, e.g., Haskell, and it
follows the :ref:`iterator protocol <python:typeiter>`.  It is very convenient.
For example, the above ``for`` loop can be replaced by a one-liner:

.. code-block:: pycon

  >>> print("\n".join([str(i) for i in data]))
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9

Generator
+++++++++

.. code-block:: python

  def list_generator(input_data):
      for i in input_data:
          yield i

.. code-block:: pycon

  >>> generator = list_generator(data)
  >>> print(generator)
  <generator object list_generator at 0x10cf756d0>
  >>> print(dir(generator))
  ['__class__', '__del__', '__delattr__', '__dir__', '__doc__', '__eq__',
  '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__',
  '__init_subclass__', '__iter__', '__le__', '__lt__', '__name__', '__ne__',
  '__new__', '__next__', '__qualname__', '__reduce__', '__reduce_ex__',
  '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__',
  'close', 'gi_code', 'gi_frame', 'gi_running', 'gi_yieldfrom', 'send',
  'throw']
  >>> for i in list_generator(data):
  >>>     print(i)
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9

Generator Expression
++++++++++++++++++++

A more convenient way of creating a generator is to use the generator
expression ``(... for ... in ...)``.  Note this looks like the list
comprehension ``[... for ... in ...]``, but uses parentheses to replace the
brackets.

.. code-block:: pycon

  >>> generator2 = (i for i in data)
  >>> print(generator2)
  <generator object <genexpr> at 0x10cfce1d0>
  >>> print(dir(generator2))
  ['__class__', '__del__', '__delattr__', '__dir__', '__doc__', '__eq__',
  '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__',
  '__init_subclass__', '__iter__', '__le__', '__lt__', '__name__', '__ne__',
  '__new__', '__next__', '__qualname__', '__reduce__', '__reduce_ex__',
  '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__',
  'close', 'gi_code', 'gi_frame', 'gi_running', 'gi_yieldfrom', 'send',
  'throw']
  >>> for i in generator2:
  >>>     print(i)
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9

By using the generator expression, the data printing one-liner can drop the
brackets:

.. code-block:: pycon

  >>> print("\n".join(str(i) for i in data))
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9
  >>> # Compare with the the list comprehension:
  >>> print("\n".join( [ str(i) for i in data ] ))
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9

Python Stack Frame
==================

(C)Python uses a stack-based interpreter.  We are allowed to peek all the
previous stack frames:

.. code-block:: python

  import traceback

  def f1():
      traceback.print_stack()

  def f2():
      f1()

  def f3():
      f2()

.. code-block:: pycon

  >>> f3()
    File "<stdin>", line 1, in <module>
    File "<stdin>", line 2, in f3
    File "<stdin>", line 2, in f2
    File "<stdin>", line 2, in f1

``frame`` object
++++++++++++++++

We can get the ``frame`` object of the current stack frame using
:py:func:`python:inspect.currentframe`:

.. code-block:: pycon

  >>> import inspect
  >>> f = inspect.currentframe()

A ``frame`` object has the following attributes:

* Namespace:
  * ``f_builtins``: builtin namespace seen by this frame
  * ``f_globals``: global namespace seen by this frame
  * ``f_locals``: local namespace seen by this frame
* Other:
  * ``f_back``: next outer frame object (this frame's caller)
  * ``f_code``: code object being executed in this frame
  * ``f_lasti``: index of last attempted instruction in bytecode
  * ``f_lineno``: current line number in Python source code

.. code-block:: pycon

  >>> print([k for k in dir(f) if not k.startswith('__')])
  ['clear', 'f_back', 'f_builtins', 'f_code', 'f_globals', 'f_lasti',
  'f_lineno', 'f_locals', 'f_trace', 'f_trace_lines', 'f_trace_opcodes']

We can learn many things about the frame in the object.  For example, we can
take a look in the builtin namespace:

.. code-block:: pycon

  >>> print(f.f_builtins.keys())
  dict_keys(['__name__', '__doc__', '__package__', '__loader__', '__spec__',
  '__build_class__', '__import__', 'abs', 'all', 'any', 'ascii', 'bin',
  'breakpoint', 'callable', 'chr', 'compile', 'delattr', 'dir', 'divmod',
  'eval', 'exec', 'format', 'getattr', 'globals', 'hasattr', 'hash', 'hex',
  'id', 'input', 'isinstance', 'issubclass', 'iter', 'len', 'locals', 'max',
  'min', 'next', 'oct', 'ord', 'pow', 'print', 'repr', 'round', 'setattr',
  'sorted', 'sum', 'vars', 'None', 'Ellipsis', 'NotImplemented', 'False',
  'True', 'bool', 'memoryview', 'bytearray', 'bytes', 'classmethod', 'complex',
  'dict', 'enumerate', 'filter', 'float', 'frozenset', 'property', 'int',
  'list', 'map', 'object', 'range', 'reversed', 'set', 'slice', 'staticmethod',
  'str', 'super', 'tuple', 'type', 'zip', '__debug__', 'BaseException',
  'Exception', 'TypeError', 'StopAsyncIteration', 'StopIteration',
  'GeneratorExit', 'SystemExit', 'KeyboardInterrupt', 'ImportError',
  'ModuleNotFoundError', 'OSError', 'EnvironmentError', 'IOError', 'EOFError',
  'RuntimeError', 'RecursionError', 'NotImplementedError', 'NameError',
  'UnboundLocalError', 'AttributeError', 'SyntaxError', 'IndentationError',
  'TabError', 'LookupError', 'IndexError', 'KeyError', 'ValueError',
  'UnicodeError', 'UnicodeEncodeError', 'UnicodeDecodeError',
  'UnicodeTranslateError', 'AssertionError', 'ArithmeticError',
  'FloatingPointError', 'OverflowError', 'ZeroDivisionError', 'SystemError',
  'ReferenceError', 'MemoryError', 'BufferError', 'Warning', 'UserWarning',
  'DeprecationWarning', 'PendingDeprecationWarning', 'SyntaxWarning',
  'RuntimeWarning', 'FutureWarning', 'ImportWarning', 'UnicodeWarning',
  'BytesWarning', 'ResourceWarning', 'ConnectionError', 'BlockingIOError',
  'BrokenPipeError', 'ChildProcessError', 'ConnectionAbortedError',
  'ConnectionRefusedError', 'ConnectionResetError', 'FileExistsError',
  'FileNotFoundError', 'IsADirectoryError', 'NotADirectoryError',
  'InterruptedError', 'PermissionError', 'ProcessLookupError', 'TimeoutError',
  'open', 'copyright', 'credits', 'license', 'help', '__IPYTHON__', 'display',
  'get_ipython'])

A mysterious ``code`` object:

.. code-block::

  >>> print(f.f_code)
  <code object <module> at 0x10d0d1810, file "<ipython-input-26-dac680851f0c>",
  line 3>

Because a ``frame`` object holds everything a construct uses, after finishing
using the `frame` object, make sure to break the reference to it.  If we don't
do it, it may take long time for the interpreter to break the reference for
you.

An example script for showing stack frame:

.. code-block::

  >>> f.clear()
  >>> del f

.. literalinclude:: code/showframe.py
  :language: python
  :linenos:
  :end-before: # [end example]

.. code-block:: console

  $ ./showframe.py
  frame #0:
    FrameInfo(frame=<frame at 0x7f8d4c31fdc0, file './showframe.py', line 8, code main>, filename='./showframe.py', lineno=7, function='main', code_context=['    for it, fi in enumerate(inspect.stack()):\n'], index=0)

  frame #1:
    FrameInfo(frame=<frame at 0x104762450, file './showframe.py', line 11, code <module>>, filename='./showframe.py', lineno=11, function='<module>', code_context=['    main()\n'], index=0)

Customizing Module Import with :py:data:`python:sys.meta_path`
==============================================================

Python :doc:`importlib <python:library/importlib>` allows high degree of
freedom in customizing how modules are imported.  Here I will use an example to
load a module, :py:mod:`!onemod`, locating in an alternate directory,
``altdir/``, and ask Python to load it from the non-standard location.

.. code-block:: pycon

  >>> # Bookkeeping code: keep the original meta_path.
  >>> old_meta_path = sys.meta_path[:]

.. >>> #sys.meta_path = old_meta_path[:-1]

:py:mod:`python:importlib` provides many facilities.  The theme in this example
is :py:data:`python:sys.meta_path`.  It defines a list of
:py:class:`python:importlib.abc.MetaPathFinder` objects for customizing the
import process.

.. code-block:: pycon

  >>> sys.meta_path = old_meta_path
  >>> print(sys.meta_path)
  [<class '_frozen_importlib.BuiltinImporter'>, <class
  '_frozen_importlib.FrozenImporter'>, <class
  '_frozen_importlib_external.PathFinder'>]

At this point, the :py:mod:`!onemod` cannot be imported, because ``altdir/`` is
not in :py:data:`python:sys.path`:

.. code-block:: pycon

  >>> import onemod
  Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
  ModuleNotFoundError: No module named 'onemod'

In normal Python code, you will be asked to modify :py:data:`python:sys.path`
to include ``altdir/`` for correctly import :py:mod:`!onemod`.  Here we will
use :py:class:`~python:importlib.abc.MetaPathFinder`.  Subclass the abstract
base class (ABC) and override the
:py:meth:`~python:importlib.abc.MetaPathFinder.find_spec` method, to tell it to
load the :py:mod:`!onemod` module at the place we specify.

For our path finder to work, we need to properly set up a
:py:class:`python:importlib.machinery.ModuleSpec`, and create a
:py:class:`python:importlib.machinery.SourceFileLoader` object for it.

.. code-block:: python
  :linenos:

  import os
  import importlib.abc
  import importlib.machinery

  class MyMetaPathFinder(importlib.abc.MetaPathFinder):
      def find_spec(self, fullname, path, target=None):
          if fullname == 'onemod':
              print('DEBUG: fullname: {} , path: {} , target: {}'.format(fullname, path, target))
              fpath = os.path.abspath('altdir/onemod.py')
              loader = importlib.machinery.SourceFileLoader('onemod', fpath)
              return importlib.machinery.ModuleSpec(fullname, loader, origin=fpath)
          else:
              return None

.. code-block:: pycon

  >>> sys.meta_path = old_meta_path + [MyMetaPathFinder()]
  >>> print(sys.meta_path)
  [<class '_frozen_importlib.BuiltinImporter'>, <class
  '_frozen_importlib.FrozenImporter'>, <class
  '_frozen_importlib_external.PathFinder'>, <__main__.MyMetaPathFinder object
  at 0x10117b850>]

With the meta path finder inserted, :py:mod:`!onemod` can be imported:

.. code-block:: pycon

  >>> import onemod
  DEBUG: fullname: onemod , path: None , target: None
  >>> print("show content in onemod module:", onemod.content)
  show content in onemod module: string in onemod

It only deals with :py:`python:onemod`.  To test, ask it to load a module that
does not exist:

.. code-block:: pycon

  >>> import one_non_existing_module
  Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
  ModuleNotFoundError: No module named 'one_non_existing_module'

See the module we loaded.  Compare it with a "normal module".

.. code-block:: pycon

  >>> import re
  >>> print('onemod:', onemod)
  onemod: <module 'onemod' (/Users/yungyuc/work/web/ynote/nsd/12advpy/code/altdir/onemod.py)>
  >>> print('re:', re)
  re: <module 're' from '/Users/yungyuc/hack/usr/opt39_210210/lib/python3.9/re.py'>

The module objects have an important field :py:data:`!__spec__`, which is the
:py:class:`~python:importlib.machinery.ModuleSpec` we created:

.. code-block:: pycon

  >>> print('onemod.__spec__:', onemod.__spec__)
  onemod.__spec__: ModuleSpec(name='onemod',
  loader=<_frozen_importlib_external.SourceFileLoader object at 0x10117bd30>,
  origin='/Users/yungyuc/work/web/ynote/nsd/12advpy/code/altdir/onemod.py')
  >>> print('re.__spec__:', re.__spec__)
  re.__spec__: ModuleSpec(name='re',
  loader=<_frozen_importlib_external.SourceFileLoader object at 0x1010b4fa0>,
  origin='/Users/yungyuc/hack/usr/opt39_210210/lib/python3.9/re.py')

Descriptor
==========

The :doc:`descriptor protocol <python:howto/descriptor>` allows us to route
attribute access to anywhere outside the class.

.. code-block:: python
  :linenos:

  class ClsAccessor:
      """Routing access to all instance attributes to the descriptor object."""
      def __init__(self, name):
          self._name = name
          self._val = None
      def __get__(self, obj, objtype):
          print('On object {} , retrieve: {}'.format(obj, self._name))
          return self._val
      def __set__(self, obj, val):
          print('On object {} , update: {}'.format(obj, self._name))
          self._val = val

  class MyClass:
      x = ClsAccessor('x')

See the message printed while getting the attribute :py:attr:`!x`:

.. code-block:: pycon

  >>> o = MyClass()
  >>> print(o.x)
  On object <__main__.MyClass object at 0x1011c02b0> , retrieve: x
  None

Setting the attribute also shows a message:

.. code-block:: pycon

  >>> o.x = 10
  On object <__main__.MyClass object at 0x1011c02b0> , update: x
  >>> print(o.x)
  On object <__main__.MyClass object at 0x1011c02b0> , retrieve: x
  10

Because the attribute value is kept in the descriptor, and the descriptor is
kept in the :py:class:`!class` object, attributes of all instances of
:py:class:`!MyClass` share the same value.

.. code-block:: pycon

  >>> o2 = MyClass()
  >>> print(o2.x) # Not None!
  On object <__main__.MyClass object at 0x1011c02e0> , retrieve: x
  10
  >>> o2.x = 100
  On object <__main__.MyClass object at 0x1011c02e0> , update: x
  >>> print(o.x)
  On object <__main__.MyClass object at 0x1011c02b0> , retrieve: x
  100

Keep Data on the Instance
+++++++++++++++++++++++++

Having all instances sharing the attribute value isn't always desirable.
Descriptor protocol allows to bind the values to the instance too.

.. code-block:: python

  class InsAccessor:
      """Routing access to all instance attributes to alternate names on the instance."""
      def __init__(self, name):
          self._name = name
      def __get__(self, obj, objtype):
          print('On object {} , instance retrieve: {}'.format(obj, self._name))
          varname = '_acs' + self._name
          if not hasattr(obj, varname):
              setattr(obj, varname, None)
          return getattr(obj, varname)
      def __set__(self, obj, val):
          print('On object {} , instance update: {}'.format(obj, self._name))
          varname = '_acs' + self._name
          return setattr(obj, varname, val)

  class MyClass2:
      x = InsAccessor('x')

Create an instance to test the descriptor.

.. code-block:: pycon

  >>> mo = MyClass2()
  >>> print(mo.x)
  On object <__main__.MyClass2 object at 0x101190250> , instance retrieve: x
  None
  >>> mo.x = 10
  On object <__main__.MyClass2 object at 0x101190250> , instance update: x
  >>> print(mo.x)
  On object <__main__.MyClass2 object at 0x101190250> , instance retrieve: x
  10

In a new instance, the value uses the initial value:

.. code-block:: pycon

  >>> mo2 = MyClass2()
  >>> print(mo2.x)
  On object <__main__.MyClass2 object at 0x101190a90> , instance retrieve: x
  None

Metaclass
=========

Python class is also an object.

.. code-block:: python

  class ClassIsObject:
      pass

.. code-block:: pycon

  >>> print(ClassIsObject)
  <class '__main__.ClassIsObject'>
  >>> print(ClassIsObject.__dict__)
  {'__module__': '__main__',
   '__dict__': <attribute '__dict__' of 'ClassIsObject' objects>,
   '__weakref__': <attribute '__weakref__' of 'ClassIsObject' objects>,
   '__doc__': None}
  >>> isinstance(ClassIsObject, object)
  True
  >>> isinstance(ClassIsObject, type)
  True
  >>> isinstance(type, object)
  True

:ref:`Metaclasses <python:metaclasses>` allow programmers to customize class creation.

.. code-block:: python

  class AutoAccessor:
      """Routing access to all instance attributes to alternate names on the instance."""
      def __init__(self):
          self.name = None
      def __get__(self, obj, objtype):
          print('On object {} , auto retrieve: {}'.format(obj, self.name))
          varname = '_acs' + self.name
          if not hasattr(obj, varname):
              setattr(obj, varname, None)
          return getattr(obj, varname)
      def __set__(self, obj, val):
          print('On object {} , auto update: {}'.format(obj, self.name))
          varname = '_acs' + self.name
          return setattr(obj, varname, val)

  class AutoAccessorMeta(type):
      def __new__(cls, name, bases, namespace):
          print('DEBUG before names:', name)
          print('DEBUG before bases:', bases)
          print('DEBUG before namespace:', namespace)
          for k, v in namespace.items():
              if isinstance(v, AutoAccessor):
                  v.name = k
          # Create the class object for MyAutoClass.
          newcls = super(AutoAccessorMeta, cls).__new__(cls, name, bases, namespace)
          print('DEBUG after names:', name)
          print('DEBUG after bases:', bases)
          print('DEBUG after namespace:', namespace)
          return newcls

We will use the descriptor to test the metaclass.  The new descriptor class
:py:class:`!AutoAccessor` doesn't take the attribute name in the constructor.
Instead, :py:class:`!AutoAccessorMeta` assigns the correct attribute name.

.. code-block:: pycon

  >>> class MyAutoClassDefault(metaclass=type):
  ...     x = AutoAccessor()
  ...
  >>> class MyAutoClass(metaclass=AutoAccessorMeta):
  ...     x = AutoAccessor()  # Note: no name is given.
  ...
  DEBUG before names: MyAutoClass
  DEBUG before bases: ()
  DEBUG before namespace: {'__module__': '__main__',
  '__qualname__': 'MyAutoClass',
  'x': <__main__.AutoAccessor object at 0x10117bcd0>}
  DEBUG after names: MyAutoClass
  DEBUG after bases: ()
  DEBUG after namespace: {'__module__': '__main__',
  '__qualname__': 'MyAutoClass',
  'x': <__main__.AutoAccessor object at 0x10117bcd0>}
  >>> ao = MyAutoClass()
  >>> print(ao.x)
  On object <__main__.MyAutoClass object at 0x101190460> , auto retrieve: x
  None
  >>> ao.x = 10
  On object <__main__.MyAutoClass object at 0x101190460> , auto update: x
  >>> print(ao.x)
  On object <__main__.MyAutoClass object at 0x101190460> , auto retrieve: x
  10
  >>> print(ao._acsx)
  10

Type Introspection and :py:mod:`Abstract Base Class (abc) <python:abc>`
=======================================================================

.. code-block:: pycon

  >>> class MyBaseClass:
  ...     pass
  ...
  >>> class MyDerivedClass(MyBaseClass):
  ...     pass
  ...
  >>> base = MyBaseClass()
  >>> derived = MyDerivedClass()
  >>> print('base {} MyBaseClass'.format('is' if isinstance(base, MyBaseClass) else 'is not'))
  base is MyBaseClass
  >>> print('base {} MyDerivedClass'.format('is' if isinstance(base, MyDerivedClass) else 'is not'))
  base is not MyDerivedClass
  >>> print('derived {} MyBaseClass'.format('is' if isinstance(derived, MyBaseClass) else 'is not'))
  derived is MyBaseClass
  >>> print('derived {} MyDerivedClass'.format('is' if isinstance(derived, MyDerivedClass) else 'is not'))
  derived is MyDerivedClass

Method Resolution Order (mro)
+++++++++++++++++++++++++++++

Python uses the "C3" algorithm to determine the [method resolution order
(MRO)](https://www.python.org/download/releases/2.3/mro/) [1]_.

.. code-block:: python

  class A:
      def process(self):
          print('A process()')

  class B(A):
      def process(self):
          print('B process()')
          super(B, self).process()

  class C(A):
      def process(self):
          print('C process()')
          super(C, self).process()

  class D(B, C):
      pass

.. code-block:: pycon

  >>> print(D.__mro__)
  (<class '__main__.D'>, <class '__main__.B'>, <class '__main__.C'>, <class '__main__.A'>, <class 'object'>)
  >>> obj = D()
  >>> obj.process()
  B process()
  C process()
  A process()

Change the order in the inheritance declaration and the MRO changes
accordingly.

.. code-block:: pycon

  >>> class D(C, B):
  ...     pass
  ...
  >>> print(D.__mro__)
  (<class '__main__.D'>, <class '__main__.C'>, <class '__main__.B'>, <class '__main__.A'>, <class 'object'>)

Example: Multiple-Level Inheritance
-----------------------------------

.. code-block:: python

  O = object
  class F(O): pass
  class E(O): pass
  class D(O): pass
  class C(D, F): pass
  class B(D, E): pass
  class A(B, C): pass

.. code-block:: pycon

  >>> print(A.__mro__)
  (<class '__main__.A'>, <class '__main__.B'>, <class '__main__.C'>, <class '__main__.D'>, <class '__main__.E'>, <class '__main__.F'>, <class 'object'>)
  >>> print(B.__mro__)
  (<class '__main__.B'>, <class '__main__.D'>, <class '__main__.E'>, <class 'object'>)
  >>> print(C.__mro__)
  (<class '__main__.C'>, <class '__main__.D'>, <class '__main__.F'>, <class 'object'>)
  >>> print(D.__mro__)
  (<class '__main__.D'>, <class 'object'>)
  >>> print(E.__mro__)
  (<class '__main__.E'>, <class 'object'>)
  >>> print(F.__mro__)
  (<class '__main__.F'>, <class 'object'>)

.. code-block:: pycon

  >>> a = A()
  >>> print('a {} A'.format('is' if isinstance(a, A) else 'is not'))
  a is A
  >>> print('a {} B'.format('is' if isinstance(a, B) else 'is not'))
  a is B
  >>> print('a {} C'.format('is' if isinstance(a, C) else 'is not'))
  a is C
  >>> print('a {} D'.format('is' if isinstance(a, D) else 'is not'))
  a is D
  >>> print('a {} E'.format('is' if isinstance(a, E) else 'is not'))
  a is E
  >>> print('a {} F'.format('is' if isinstance(a, F) else 'is not'))
  a is F

:py:mod:`Abstract Base Class (abc) <python:abc>`
++++++++++++++++++++++++++++++++++++++++++++++++

Python :py:doc:`abstract base class (abc) <python:library/abc>` provides the
capabilities to overload :py:func:`python:isinstance` and
:py:func:`python:issubclass`, and define abstract methods.

We can use :py:meth:`python:abc.ABCMeta.register` method to ask a class
:py:class:`!MyABC` that is not in a inheritance chain of another class
:py:class:`!A` to be a "virtual" base class of the latter.

.. code-block:: python

  import abc

  class MyABC(metaclass=abc.ABCMeta):
      pass

As we know, :py:class:`!A` is not a subclass of :py:class:`!MyABC`:

.. code-block:: pycon

  >>> print('A {} a subclass of MyABC'.format('is' if issubclass(A, MyABC) else 'is not'))
  A is not a subclass of MyABC

But once we ":py:meth:`register <python:abc.ABCMeta.register>`" :py:class:`!MyABC` to be
a virtual base class of :py:class:`!A`, we will see :py:class:`!A` becomes
subclass of :py:class:`!MyABC`:

.. code-block:: pycon

  >>> MyABC.register(A)
  <class '__main__.A'>
  >>>
  >>> print('A {} a subclass of MyABC'.format('is' if issubclass(A, MyABC) else 'is not'))
  A is a subclass of MyABC

Abstract Method
+++++++++++++++

Using :py:mod:`python:abc`, we can add abstract methods to an class (making it
abstract).

.. code-block:: python

  class AbstractClass(metaclass=abc.ABCMeta):
      @abc.abstractmethod
      def process(self):
          pass

An abstract class cannot be instantiated:

.. code-block:: pycon

  >>> a = AbstractClass()
  Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
  TypeError: Can't instantiate abstract class AbstractClass with abstract method process

In a derived class, the abstract method needs to be overridden

.. code-block:: python

  class GoodConcreteClass(AbstractClass):
      def process(self):
          print('GoodConcreteClass process')

Then the good concrete class can run.

.. code-block:: pycon

  >>> g = GoodConcreteClass()
  >>> g.process()
  GoodConcreteClass process

If the abstract method is not overridden

.. code-block:: python

  class BadConcreteClass(AbstractClass):
      pass

the derived class cannot run.

.. code-block:: pycon

  >>> b = BadConcreteClass()
  Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
  TypeError: Can't instantiate abstract class BadConcreteClass with abstract method process

References
==========

.. [1] K. Barrett, B. Cassels, P. Haahr, D. A. Moon, K. Playford, and P. T.
   Withington, “A monotonic superclass linearization for Dylan,” SIGPLAN Not.,
   vol. 31, no. 10, pp. 69–82, Oct. 1996, doi: 10.1145/236338.236343.
   https://dl.acm.org/doi/10.1145/236338.236343.

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
