FLAGS = -Wall -pedantic -std=c99 -fsanitize=address

LIBRARIES = -lm

FILES = main.c imdbADT.c rankingADT.c readInput.c yearsADT.c print.c

EXE_FILE = imdb

COMPILER = gcc

TEST_FILE = test.csv

DATASET_FILE = imdbv3.csv

RUN = @$(COMPILER) $(FLAGS) $(FILES) -o $(EXE_FILE) $(LIBRARIES)

imdb:
	$(RUN)
	@echo "Nombre de archivo ejecutable: $(EXE_FILE)"

remove:
	@rm query?.csv $(EXE_FILE)
	@echo "Querys y $(EXE_FILE) borrados"

run:
	$(RUN)
	@./$(EXE_FILE) $(DATASET_FILE)
	@echo "Proceso listo"
