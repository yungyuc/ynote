==========================================
Example Code: Ownership and Smart Pointers
==========================================

.. literalinclude:: code/01_pointer/01_raw_pointer.cpp
  :name: nsd-smart-example-raw
  :caption:
    Manipulate data using a raw pointer (:download:`01_raw_pointer.cpp
    <code/01_pointer/01_raw_pointer.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-raw-build
  :caption:
    Build :ref:`01_raw_pointer.cpp <nsd-smart-example-raw>`.

  $ g++ 01_raw_pointer.cpp -o 01_raw_pointer -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. literalinclude:: code/01_pointer/02_reference.cpp
  :name: nsd-smart-example-ref
  :caption:
    Manipulate data using a reference (:download:`02_reference.cpp
    <code/01_pointer/02_reference.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-ref-build
  :caption:
    Build :ref:`02_reference.cpp <nsd-smart-example-ref>`.

  $ g++ 02_reference.cpp -o 02_reference -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. literalinclude:: code/01_pointer/03_ownership.cpp
  :name: nsd-smart-example-own
  :caption:
    The concept of ownership (:download:`03_ownership.cpp
    <code/01_pointer/03_ownership.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-own-build
  :caption:
    Build :ref:`03_ownership.cpp <nsd-smart-example-own>`.

  $ g++ 03_ownership.cpp -o 03_ownership -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. code-block:: console
  :name: nsd-smart-example-own-crash
  :caption:
    Build :ref:`03_ownership.cpp <nsd-smart-example-own>` but turn on the
    crashing macro.

  $ g++ 03_ownership.cpp -o 03_ownership -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCRASHME

.. literalinclude:: code/01_pointer/04_unique.cpp
  :name: nsd-smart-example-unique
  :caption:
    Use unique pointer (:download:`04_unique.cpp
    <code/01_pointer/04_unique.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-unique-build
  :caption:
    Build :ref:`04_unique.cpp <nsd-smart-example-unique>`.

  $ g++ 04_unique.cpp -o 04_unique -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. code-block:: console
  :name: nsd-smart-example-unique-build-crash
  :caption:
    Build :ref:`04_unique.cpp <nsd-smart-example-unique>` but turn on the
    crashing macro.

  $ g++ 04_unique.cpp -o 04_unique -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCOPYNOWORK

.. literalinclude:: code/01_pointer/05_shared.cpp
  :name: nsd-smart-example-shared
  :caption:
    Use shared pointer (:download:`05_shared.cpp
    <code/01_pointer/05_shared.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-shared-build
  :caption:
    Build :ref:`05_shared.cpp <nsd-smart-example-shared>`.

  $ g++ 05_shared.cpp -o 05_shared -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. literalinclude:: code/02_shared/01_fully.cpp
  :name: nsd-smart-example-fully
  :caption:
    Fully manage data object in shared pointer (:download:`01_fully.cpp
    <code/02_shared/01_fully.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-fully-build
  :caption:
    Build :ref:`01_fully.cpp <nsd-smart-example-fully>`.

  $ g++ 01_fully.cpp -o 01_fully -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. code-block:: console
  :name: nsd-smart-example-fully-ctor
  :caption:
    Build :ref:`01_fully.cpp <nsd-smart-example-fully>` with constructor error.

  $ g++ 01_fully.cpp -o 01_fully -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCTORNOWORK

.. code-block:: console
  :name: nsd-smart-example-fully-make
  :caption:
    Build :ref:`01_fully.cpp <nsd-smart-example-fully>` with ``make_shared``
    error.

  $ g++ 01_fully.cpp -o 01_fully -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DMAKENOWORK

.. literalinclude:: code/02_shared/02_duplicate.cpp
  :name: nsd-smart-example-duplicate
  :caption:
    Duplicate the ownership anywhere (:download:`02_duplicate.cpp
    <code/02_shared/02_duplicate.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-duplicate-build
  :caption:
    Build :ref:`02_duplicate.cpp <nsd-smart-example-duplicate>` (it contains
    the double-free bug).

  $ g++ 02_duplicate.cpp -o 02_duplicate -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. literalinclude:: code/02_shared/03_fromthis.cpp
  :name: nsd-smart-example-fromthis
  :caption:
    Enable shared pointer from this (:download:`03_fromthis.cpp
    <code/02_shared/03_fromthis.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-fromthis-build
  :caption:
    Build :ref:`03_fromthis.cpp <nsd-smart-example-fromthis>`.

  $ g++ 03_fromthis.cpp -o 03_fromthis -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. literalinclude:: code/02_shared/04_cyclic.cpp
  :name: nsd-smart-example-cyclic
  :caption:
    The problem of circular reference of shared pointer
    (:download:`04_cyclic.cpp <code/02_shared/04_cyclic.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-cyclic-build
  :caption:
    Build :ref:`04_cyclic.cpp <nsd-smart-example-cyclic>`.

  $ g++ 04_cyclic.cpp -o 04_cyclic -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. literalinclude:: code/02_shared/05_weak.cpp
  :name: nsd-smart-example-weak
  :caption:
    Use weak pointers to break circular reference (:download:`05_weak.cpp
    <code/02_shared/05_weak.cpp>`).
  :language: cpp
  :linenos:
  :end-before: // vim: set

.. code-block:: console
  :name: nsd-smart-example-weak-build
  :caption:
    Build :ref:`05_weak.cpp <nsd-smart-example-cyclic>`.

  $ g++ 05_weak.cpp -o 05_weak -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror

.. add 10 blank lines to avoid messing up with the vim modeline.











.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
