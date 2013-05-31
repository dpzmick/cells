ifndef CC
CC=gcc-4.9
endif
OPTS=-O2 -std=c99
OMP=-fopenmp

EXEC=cells
SOURCES=cells.c


all: cells

cells: cells.c
	$(CC) $(OPTS) $(OMP) $(SOURCES) -o $(EXEC)

run: all
	mkdir output
	@./$(EXEC) $(L) $(T)

clean:
	rm $(EXEC)
