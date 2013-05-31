CC=gcc-4.9
OPTS=-O2 -std=c99
OMP=-fopenmp
CILK=-fcilkplus -lcilkrts

EXEC=cells
SOURCES=cells.c


all: cells-omp

cells-omp: cells.c
	$(CC) $(OPTS) $(OMP) -Dopenmp $(SOURCES) -o $(EXEC)

cells-cilk: cells.c
	$(CC) $(OPTS) $(CILK) -Dcilk $(SOURCES) -o $(EXEC)

run: cells.c cells
	@echo Make usage: make run R=r L=l T=t
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
