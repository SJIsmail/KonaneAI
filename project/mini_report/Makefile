.PHONY: all data

all: mini_report.pdf

%.pdf: %.tex data
	latexmk -xelatex -quiet "$*" || { cat "$*.log"; exit 1; }

clean:
	latexmk -C
