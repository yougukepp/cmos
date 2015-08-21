all:
	xelatex main.tex
	bibtex main
	xelatex main.tex
	xelatex main.tex
	evince main.pdf &
	rm -rf *.aux *.idx *.log *.toc *.out *.bbl *.blg *.lot *.lof
clean:
	rm -rf *.aux *.idx *.log *.toc *.out *.bbl *.blg *.lot *.lof *.pdf

