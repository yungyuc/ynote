default: ../image/cce.png ../image/mplplot.png

../image/cce.png: cce.tex pstake.py Makefile
	./pstake.py $< $@ 2>&1 > /dev/null

../image/mplplot.png: mplplot.py Makefile
	./$<
