============================================
Example Code (C++ and Computer Architecture)
============================================

.. literalinclude:: code/types.cpp
  :name: nsd-cpp-example-types-cpp
  :caption:
    Show the size of fundamental integer types
    (:download:`types.cpp <code/types.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. literalinclude:: code/cstdint.cpp
  :name: nsd-cpp-example-cstdint-cpp
  :caption:
    Show the size of fixed-width integer types
    (:download:`cstdint.cpp <code/cstdint.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. literalinclude:: code/arrays.cpp
  :name: nsd-cpp-example-arrays-cpp
  :caption:
    Index arrays using integers (:download:`arrays.cpp <code/arrays.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. literalinclude:: code/class.cpp
  :name: nsd-cpp-example-class
  :caption:
    C++ classes (:download:`class.cpp <code/class.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-cpp-example-class-build
  :caption:
    Execution results of :ref:`class.cpp <nsd-cpp-example-class>`

  $ g++ class.cpp -o class --std=c++11
  $ ./class
  PointClass and PointStruct has the same size: true
  pntc.getX() = 1002, pntc.getY() = 2004
  pnts.m_x = 1007, pnts.m_y = 1009

.. add 10 blank lines to avoid messing up with the vim modeline.












.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
