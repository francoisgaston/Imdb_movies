#include <stdio.h>
#include "imdbADT.h"

int main(int argQty, char * file[]) {
    imdbADT imdb = newImdb();
    if(imdb == NULL){
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
    readInput(argQty, file[], imdb)
    showData(imdb);
    freeImdbADT(imdb);
    return 0;
}
