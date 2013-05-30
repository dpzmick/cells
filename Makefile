all: cells

cells: cells.c
	gcc-4.9 -fopenmp cells.c -o cells -O2 -std=c99

run: cells.c cells
	@echo Make usage: make run R=r L=l T=t
	@./cells $(R) $(L) $(T)

run_fav: cells cells.c
	./cells 30 10000 5000

image:
	pnmtopng output.pbm > output.png
	rm output.pbm

view: image
	open output.png

clean:
	rm cells
	rm output.*
