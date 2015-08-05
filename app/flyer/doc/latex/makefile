all:
	xelatex 姿态融合算法.tex
	bibtex 姿态融合算法
	xelatex 姿态融合算法.tex
	xelatex 姿态融合算法.tex
	evince 姿态融合算法.pdf &
	rm -rf *.aux *.idx *.log *.toc *.out *.bbl *.blg
clean:
	rm -rf *.aux *.idx *.log *.pdf *.toc *.out *.bbl *.blg

