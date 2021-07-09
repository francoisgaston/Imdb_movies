#include <stdio.h>
#include "imdbADT.h"
#include "front.h"
#include <stdlib.h>
int main(int argQty, char * file[]) {
    if(argQty != 2){ //Verifica si la cantidad de argumentos al ejecutar es correcta.
        fprintf(stderr, "CANTIDAD DE ARGUMENTOS INVALIDA!\n");
        exit(1);
    }
    imdbADT imdb = newImdb();
    if(imdb == NULL){
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n");
        exit(1);
    }
    readInput(file, imdb);
    showData(imdb);
    freeImdb(imdb);
    return 0;
}
