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
    Build :ref:`03_ownership.cpp <nsd-smart-example-own>`.

  $ g++ 03_ownership.cpp -o 03_ownership -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror -DCRASHME

.. add 10 blank lines to avoid messing up with the vim modeline.











.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
