all: 3dRotateByZ.pdf

3dRotateByZ.pdf:3dRotateByZ.asy
	asy -render=0 *.asy

draw:all
	evince *.pdf

clean:
	rm -rf *.pdf *.aux *.log

.PHONY : clean

