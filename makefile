FLAGS = -Wall -pedantic -std=c99 -lm -fsanitize=address

FILES = main.c imdbADT.c rankingADT.c readInput.c yearsADT.c print.c

EXE_FILE = imdb

COMPILER = gcc

TEST_FILE = test.csv

DATASET_FILE = imdbtitles.csv

RUN = @$(COMPILER) $(FLAGS) $(FILES) -o $(EXE_FILE)

imdb:
	$(RUN)
	@echo "Nombre de archivo ejecutable: $(EXE_FILE)"

remove:
	@rm query1.csv query2.csv query3.csv query4.csv $(EXE_FILE)
	@echo "Querys y $(EXE_FILE) borrados"

test:
	$(RUN)
	@./$(EXE_FILE) $(TEST_FILE)
	@echo "test listo"

run:
	$(RUN)
	@./$(EXE_FILE) $(DATASET_FILE)
	@echo "Proceso listo"
