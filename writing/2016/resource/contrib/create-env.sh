#!/bin/bash
ENVNAME=ynote_bweak
conda create -n ${ENVNAME} --no-default-packages -y -c conda-forge \
  python pip boost \
  pillow sphinx sphinx_rtd_theme
source activate ${ENVNAME}
pip install https://github.com/pybind/pybind11/archive/master.zip
