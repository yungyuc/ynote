================================
Example Code (Matrix Operations)
================================

.. literalinclude:: code/pod01_vector.cpp
  :name: nsd-matrix-example-pod01-vector
  :caption:
    Use a simple C array for vector (:download:`pod01_vector.cpp
    <code/pod01_vector.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :caption: Build ":ref:`nsd-matrix-example-pod01-vector`"

  $ g++ pod01_vector.cpp -o pod01_vector -std=c++17 -O3 -g -m64

.. literalinclude:: code/pod02_matrix_auto.cpp
  :name: nsd-matrix-example-pod02-matrix-auto
  :caption:
    Use a 2D array on stack for a square matrix
    (:download:`pod02_matrix_auto.cpp <code/pod02_matrix_auto.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :caption: Build ":ref:`nsd-matrix-example-pod02-matrix-auto`"

  $ g++ pod02_matrix_auto.cpp -o pod02_matrix_auto -std=c++17 -O3 -g -m64

.. literalinclude:: code/pod_bad_matrix.cpp
  :name: nsd-matrix-example-pod-bad-matrix
  :caption:
    C++ does not support variable-length arrays (:download:`pod_bad_matrix.cpp
    <code/pod_bad_matrix.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. literalinclude:: code/pod03_matrix_rowmajor.cpp
  :name: nsd-matrix-example-pod03-matrix-rowmajor
  :caption:
    Row-majored 2D array (:download:`pod03_matrix_rowmajor.cpp
    <code/pod03_matrix_rowmajor.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :caption: Build ":ref:`nsd-matrix-example-pod03-matrix-rowmajor`"

  $ g++ pod03_matrix_rowmajor.cpp -o pod03_matrix_rowmajor -std=c++17 -O3 -g -m64

.. literalinclude:: code/pod04_matrix_colmajor.cpp
  :name: nsd-matrix-example-pod04-matrix-colmajor
  :caption:
    Column-majored 2D array (:download:`pod04_matrix_colmajor.cpp
    <code/pod04_matrix_colmajor.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :caption: Build ":ref:`nsd-matrix-example-pod04-matrix-colmajor`"

  $ g++ pod04_matrix_colmajor.cpp -o pod04_matrix_colmajor -std=c++17 -O3 -g -m64

.. literalinclude:: code/ma01_matrix_class.cpp
  :name: nsd-matrix-example-ma01-matrix-class
  :caption:
    Skeleton implementation for a C++ matrix class
    (:download:`ma01_matrix_class.cpp <code/ma01_matrix_class.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :caption: Build ":ref:`nsd-matrix-example-ma01-matrix-class`"

  $ g++ ma01_matrix_class.cpp -o ma01_matrix_class -std=c++17 -O3 -g -m64

.. literalinclude:: code/ma02_matrix_vector.cpp
  :name: nsd-matrix-example-ma02-matrix-vector
  :caption:
    Skeleton implementation for a C++ matrix class
    (:download:`ma02_matrix_vector.cpp <code/ma02_matrix_vector.cpp>`)
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :caption: Build ":ref:`nsd-matrix-example-ma02-matrix-vector`"

  $ g++ ma02_matrix_vector.cpp -o ma02_matrix_vector -std=c++17 -O3 -g -m64

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
