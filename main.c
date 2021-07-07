#include <stdio.h>
#include "imdbADT.h"
#include "front.h"
#include <stdlib.h>
int main(int argQty, char * file[]) {
    imdbADT imdb = newImdb();
    if(imdb == NULL){
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
    readInput(argQty, file, imdb);
    showData(imdb);
    freeImdb(imdb);
    return 0;
}
