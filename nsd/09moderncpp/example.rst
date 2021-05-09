========================
Example Code: Modern C++
========================

.. literalinclude:: code/03_elision/01_copy.cpp
  :name: nsd-moderncpp-example-copy
  :caption:
    Manipulate data using a raw pointer (:download:`01_copy.cpp
    <code/03_elision/01_copy.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-copy-build
  :caption:
    Build :ref:`01_copy.cpp <nsd-moderncpp-example-copy>`.

  $ g++ 01_copy.cpp -o 01_copy -std=c++17 -g -O3
  $ g++ 01_copy.cpp -o 01_copy -std=c++17 -g -O0

.. literalinclude:: code/03_elision/02_move.cpp
  :name: nsd-moderncpp-example-move
  :caption:
    The interaction between movement and copy elision (:download:`02_move.cpp
    <code/03_elision/02_move.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-move-build
  :caption:
    Build :ref:`02_move.cpp <nsd-moderncpp-example-move>`.

  $ g++ 02_move.cpp -o 02_move -std=c++17 -g -O3
  $ g++ 02_move.cpp -o 02_move -std=c++17 -g -O3 -DFORCEMOVE

.. literalinclude:: code/03_elision/03_accumulate.cpp
  :name: nsd-moderncpp-example-accumulate
  :caption:
    Concatenate containers (:download:`03_accumulate.cpp
    <code/03_elision/03_accumulate.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-accumulate-build
  :caption:
    Build :ref:`03_accumulate.cpp <nsd-moderncpp-example-accumulate>`.

  $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DOTYPE=1
  $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DMOVENOEXCEPT -DOTYPE=1
  $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DMOVENOEXCEPT -DOTYPE=2
  $ g++ 03_accumulate.cpp -o 03_accumulate -std=c++17 -g -O3 -DMOVENOEXCEPT -DOTYPE=3

.. literalinclude:: code/04_template/01_factory.cpp
  :name: nsd-moderncpp-example-factory
  :caption:
    Variadic template for factory function (:download:`01_factory.cpp
    <code/04_template/01_factory.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-factory-build
  :caption:
    Build :ref:`01_factory.cpp <nsd-moderncpp-example-factory>`.

  $ g++ 01_factory.cpp -o 01_factory -std=c++17 -g -O3 -DUSE_CREATE
  $ g++ 01_factory.cpp -o 01_factory -std=c++17 -g -O3
  $ g++ 01_factory.cpp -o 01_factory -std=c++17 -g -O3 -DSHOW_PERFECT_FORWARD

.. literalinclude:: code/05_lambda/01_lambda.cpp
  :name: nsd-moderncpp-example-lambda
  :caption:
    C++ lambda (:download:`01_lambda.cpp <code/05_lambda/01_lambda.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-lambda-build
  :caption:
    Build :ref:`01_lambda.cpp <nsd-moderncpp-example-lambda>`.

  $ g++ 01_lambda.cpp -o 01_lambda -std=c++17 -g -O3

.. literalinclude:: code/05_lambda/02_stored.cpp
  :name: nsd-moderncpp-example-stored
  :caption:
    Store a lambda in a variable (:download:`02_stored.cpp
    <code/05_lambda/02_stored.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-stored-build
  :caption:
    Build :ref:`02_stored.cpp <nsd-moderncpp-example-stored>`.

  $ g++ 02_stored.cpp -o 02_stored -std=c++17 -g -O3
  $ g++ 02_stored.cpp -o 02_stored -std=c++17 -g -O3 -DSHOW_DIFF

.. literalinclude:: code/05_lambda/02_stored_demangle.cpp
  :name: nsd-moderncpp-example-stored-demangle
  :caption:
    Store a lambda in a variable, demangled version
    (:download:`02_stored_demangle.cpp
    <code/05_lambda/02_stored_demangle.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-stored-demangle-build
  :caption:
    Build :ref:`02_stored_demangle.cpp <nsd-moderncpp-example-stored-demangle>`.

  $ g++ 02_stored_demangle.cpp -o 02_stored_demangle -std=c++17 -g -O3

.. literalinclude:: code/05_lambda/03_closure.cpp
  :name: nsd-moderncpp-example-closure
  :caption:
    Closure example (:download:`03_closure.cpp
    <code/05_lambda/03_closure.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-moderncpp-example-closure-build
  :caption:
    Build :ref:`03_closure.cpp <nsd-moderncpp-example-closure>`.

  $ g++ 03_closure.cpp -o 03_closure -std=c++17 -g -O3 -DWRONG_CAPTURE
  $ g++ 03_closure.cpp -o 03_closure -std=c++17 -g -O3

.. add 10 blank lines to avoid messing up with the vim modeline.











.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
