==================================
I Can Now Write PSTricks in Sphinx
==================================

:date: 2015/4/9

Thanks to `Pstake <http://pstake.readthedocs.org/>`_, now I can turn
PSTricks code embedded in Sphinx to an image in HTML:

.. code-block:: rst

  .. pstake::
    :alt: PSTricks for a hexahedron
    :align: center
    :width: 50%
    :scale: 50%

    \begin{pspicture}(-1.5,-0.9)(1.5,1.9)
    \psset{xMin=0,yMin=0,zMin=0,xunit=2.4cm,yunit=1.6cm,Alpha=60,Beta=30}

    \pstThreeDNode(0.6,-0.2,0){0}
    \pstThreeDDot(0.6,-0.2,0)
    \pstThreeDPut(0.6,-0.2,-0.2){0}
    \pstThreeDNode(0.4,0.5,0){1}
    \pstThreeDDot(0.4,0.5,0)
    \pstThreeDPut(0.4,0.6,0){1}
    \pstThreeDNode(-0.5,0.2,0){2}
    \pstThreeDDot(-0.5,0.2,0)
    \pstThreeDPut(-0.5,0.3,0){2}
    \pstThreeDNode(0,-0.3,0){3}
    \pstThreeDDot(0,-0.3,0)
    \pstThreeDPut(0,-0.4,0){3}

    \pstThreeDNode(0.6,-0.2,1){4}
    \pstThreeDDot(0.6,-0.2,1)
    \pstThreeDPut(0.6,-0.2,1.2){4}
    \pstThreeDNode(0.3,0.45,1){5}
    \pstThreeDDot(0.3,0.45,1)
    \pstThreeDPut(0.3,0.45,1.2){5}
    \pstThreeDNode(-0.5,0.2,1){6}
    \pstThreeDDot(-0.5,0.2,1)
    \pstThreeDPut(-0.5,0.3,1){6}
    \pstThreeDNode(0,-0.3,1){7}
    \pstThreeDDot(0,-0.3,1)
    \pstThreeDPut(0,-0.4,1){7}

    \ncline{0}{1}
    \ncline{1}{2}
    \ncline[linestyle=dotted]{2}{3}
    \ncline[linestyle=dotted]{3}{0}

    \ncline{0}{4}
    \ncline{1}{5}
    \ncline{2}{6}
    \ncline[linestyle=dotted]{3}{7}

    \ncline{4}{5}
    \ncline{5}{6}
    \ncline{6}{7}
    \ncline{7}{4}

    \end{pspicture}

.. pstake::
  :alt: PSTricks for a hexahedron
  :align: center
  :width: 50%
  :scale: 50%

  \begin{pspicture}(-1.5,-0.9)(1.5,1.9)
  \psset{xMin=0,yMin=0,zMin=0,xunit=2.4cm,yunit=1.6cm,Alpha=60,Beta=30}

  \pstThreeDNode(0.6,-0.2,0){0}
  \pstThreeDDot(0.6,-0.2,0)
  \pstThreeDPut(0.6,-0.2,-0.2){0}
  \pstThreeDNode(0.4,0.5,0){1}
  \pstThreeDDot(0.4,0.5,0)
  \pstThreeDPut(0.4,0.6,0){1}
  \pstThreeDNode(-0.5,0.2,0){2}
  \pstThreeDDot(-0.5,0.2,0)
  \pstThreeDPut(-0.5,0.3,0){2}
  \pstThreeDNode(0,-0.3,0){3}
  \pstThreeDDot(0,-0.3,0)
  \pstThreeDPut(0,-0.4,0){3}

  \pstThreeDNode(0.6,-0.2,1){4}
  \pstThreeDDot(0.6,-0.2,1)
  \pstThreeDPut(0.6,-0.2,1.2){4}
  \pstThreeDNode(0.3,0.45,1){5}
  \pstThreeDDot(0.3,0.45,1)
  \pstThreeDPut(0.3,0.45,1.2){5}
  \pstThreeDNode(-0.5,0.2,1){6}
  \pstThreeDDot(-0.5,0.2,1)
  \pstThreeDPut(-0.5,0.3,1){6}
  \pstThreeDNode(0,-0.3,1){7}
  \pstThreeDDot(0,-0.3,1)
  \pstThreeDPut(0,-0.4,1){7}

  \ncline{0}{1}
  \ncline{1}{2}
  \ncline[linestyle=dotted]{2}{3}
  \ncline[linestyle=dotted]{3}{0}

  \ncline{0}{4}
  \ncline{1}{5}
  \ncline{2}{6}
  \ncline[linestyle=dotted]{3}{7}

  \ncline{4}{5}
  \ncline{5}{6}
  \ncline{6}{7}
  \ncline{7}{4}

  \end{pspicture}

The commands can also be included from a separate file.  See Pstake_ for more
information.

To use Pstake_ with Sphinx (after pip installation: ``pip install pstake``), in
your ``conf.py``, enable it as an extension, like:

.. code-block:: python

  extensions.append('pstake')
