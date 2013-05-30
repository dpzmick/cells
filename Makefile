all: cells

cells: cells.c
	gcc -fopenmp cells.c -o cells -O2 -std=c99

run: cells.c
	./cells

view:
	convert output.pbm output.jpg
	rm output.pbm
	open output.jpg

clean:
	rm cells
	rm output.*
