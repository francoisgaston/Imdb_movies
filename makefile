FLAGS = -Wall -pedantic -std=c99 -fsanitize=address

LIBRARIES = -lm

FILES = main.c imdbADT.c rankingADT.c readInput.c yearsADT.c print.c

EXE_FILE = imdb

COMPILER = gcc

RUN = @$(COMPILER) $(FLAGS) $(FILES) -o $(EXE_FILE) $(LIBRARIES)

imdb:
	$(RUN)
	@echo "Nombre de archivo ejecutable: $(EXE_FILE)"
