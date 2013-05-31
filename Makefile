ifndef CC
CC=gcc-4.9
endif
OPTS=-O2 -std=c99

EXEC=cells
SOURCES=cells.c


all: cells

cells: cells.c
	$(CC) $(OPTS) -fopenmp $(SOURCES) -o $(EXEC)

cells-cilk: cells.c
	$(CC) $(OPTS) -fcilkplus -lcilkrts $(SOURCES) -o $(EXEC) 

run: all
	rm -r output
	mkdir output
	@./$(EXEC) $(L) $(T)

clean:
	rm $(EXEC)
