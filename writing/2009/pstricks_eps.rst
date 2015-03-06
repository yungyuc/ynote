==============================
Export EPS from LaTeX pstricks
==============================

pstricks_ is a very powerful tool to draw high quality schematics.

.. _pstricks: http://tug.org/PSTricks/main.cgi/

To export pstricks drawing into eps file, usually you need to make use of 
``pst-eps`` macro package and ``TeXtoEPS`` environment, and write a ``.tex``
file like

.. code-block:: latex

  \documentclass[letterpaper,dvips]{article}
  \usepackage{pst-all}
  \usepackage{pst-3dplot}
  \usepackage{pst-eps}
  \begin{document}
  \pagestyle{empty}
  \begin{TeXtoEPS}
  \psset{unit=0.7}
  \begin{pspicture}(-4.5,-4.75)(5.25,5.5)
    \psset{linewidth=.5pt}
    \pstThreeDLine[linecolor=green,arrows=->](-3,0,0)(5,0,0)
    \pstThreeDPut(5.5,0,0){$X$}
    \pstThreeDLine[linecolor=green,arrows=->](0,-1,0)(0,5,0)
    \pstThreeDPut(0,5.5,0){$Y$}
    \pstThreeDLine[linecolor=green,arrows=->](0,0,-.5)(0,0,5)
    \pstThreeDPut(0,0,5.5){$Z$}
    \pstThreeDCoor[linecolor=red,RotY=-60,RotZ=150,xMin=0,xMax=6,yMin=0,yMax=4.5,zMin=0,zMax=2]
    \psset{linewidth=1pt}
    \pstThreeDBox[linecolor=black,linestyle=solid,RotY=-60,RotZ=150](0,0,0)(5,0,0)(0,1,0)(0,0,1)
    \pstThreeDBox[linecolor=blue,linestyle=solid,RotY=60,RotZ=-30](0,-1,0)(5,0,0)(0,1,0)(0,0,1)
    \pstThreeDLine[linewidth=3pt,linecolor=black,arrows=<-,RotY=60,RotZ=-30](3,-.5,.5)(5,-.5,.5)
    \pstThreeDPut[RotY=60,RotZ=-30](5.5,-.5,.5){$\vec{v}$}
    \psset{linewidth=.5pt}
    \pstThreeDNode[SphericalCoor](2,150,0){A}
    \pstThreeDNode[SphericalCoor](2,150,60){B}
    \ncarc[arcangle=30]{<-}{B}{A}
    \pstThreeDPut[SphericalCoor](2.5,150,30){$\phi$}
    \pstThreeDNode[SphericalCoor](1.5,0,0){A}
    \pstThreeDNode[SphericalCoor](1.5,150,0){B}
    \ncarc[arcangle=60]{<-}{B}{A}
    \pstThreeDPut[SphericalCoor](2.5,75,0){$\theta$}
    \pstThreeDLine[SphericalCoor,linestyle=dashed](0,0,0)(5,150,0)
  \end{pspicture}
  \end{TeXtoEPS}
  \end{document}

Then, run ``latex`` and ``dvips`` in sequence::

  $ latex draw.tex; dvips draw.dvi -E -o draw.eps

It will give you the eps file with correct bounding box.

To convert the generated eps file to png, invoke imagemagick_::

  $ convert -density 300 draw.eps draw.png

.. _imagemagick: http://www.imagemagick.org/script/index.php

Now it's the happy ending.
