ifndef CC
CC=gcc-4.9
endif
OPTS=-O2 -std=c99
OMP=-fopenmp
CILK=-fcilkplus -lcilkrts

EXEC=cells
SOURCES=cells.c


all: cells

cells: cells.c
	$(CC) $(OPTS) $(CILK) $(SOURCES) -o $(EXEC)

run: all
	@./$(EXEC) $(R) $(L) $(T)

run_fav: cells cells.c
	./$(EXEC) 30 10000 5000

image:
	pnmtopng output.pbm > output.png
	rm output.pbm

view: image
	open output.png

clean:
	rm $(EXEC)
	rm output.*
