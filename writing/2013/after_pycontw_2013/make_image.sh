#!/bin/sh

montage \
  david_beazley.jpg \
  jan_decaluwe.jpg \
  pyconapac2013.jpg \
  allen_downey.jpg \
  david_cramer.jpg \
  lottery.jpg -tile 2x3 -geometry +0+0 agenda.jpg
convert agenda.jpg -resize 512 agenda.jpg

montage \
  night_market.jpg \
  food.jpg \
  musicpy.jpg \
  strong.jpg \
  people.jpg \
  raspberrypi.jpg \
  -tile 2x3 -geometry +0+0 reception.jpg
convert reception.jpg -resize 512 reception.jpg
