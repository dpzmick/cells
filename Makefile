all: cells

cells: cells.c
	gcc -fopenmp cells.c -o cells -O2 -std=c99

run: cells.c cells
	@echo Make usage: make run R=r L=l T=t
	@./cells $(R) $(L) $(T)

run_fav: cells cells.c
	./cells 30 100000 5000

view:
	convert output.pbm output.jpg
	rm output.pbm
	open output.jpg

clean:
	rm cells
	rm output.*
