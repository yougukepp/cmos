all:
	xelatex doc.tex
	bibtex doc
	xelatex doc.tex
	xelatex doc.tex
	evince doc.pdf &
	rm -rf *.aux *.idx *.log *.toc *.out *.bbl *.blg
clean:
	rm -rf *.aux *.idx *.log *.pdf *.toc *.out *.bbl *.blg

