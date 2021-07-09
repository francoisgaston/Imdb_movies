#include <stdio.h>
#include "imdbADT.h"

static void Query123(imdbADT imdb){
    //genero los archivos
    FILE * q1 = fopen("./query1.csv", "wt");
    FILE * q2 = fopen("./query2.csv", "wt");
    FILE * q3 = fopen("./query3.csv", "wt");
    if (q1 == NULL || q2 == NULL || q3 == NULL) {
        printf("No se pudo crear un archivo\n");
        return;
    }
    //imprimo headers
    fprintf(q1, "year;films;series\n");
    fprintf(q2, "year;genre;films\n");
    fprintf(q3, "startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie\n");
    //imprimo ADT
    toBeginYear(imdb);
    while(hasNextYear(imdb)){
        TYear aux = nextYear(imdb);
        fprintf(q1, "%d;%ld;%ld\n", aux.startYear, aux.dimMovie, aux.dimSerie);
        fprintf(q3, "%d;%s;%ld;%.1f;%s;%ld;%.1f\n", aux.startYear,
                (aux.primaryTitleMovie!=NULL)? aux.primaryTitleMovie : "-",
                aux.numVotesMovie, aux.averageRatingMovie,
                (aux.primaryTitleSerie!=NULL)? aux.primaryTitleSerie : "-",
                aux.numVotesSerie, aux.averageRatingSerie);
        while(hasNextGenres(imdb)){
            TGenre aux2 = nextGenres(imdb);
            fprintf(q2, "%d;%s;%ld\n", aux.startYear, aux2.genre, aux2.cant);
        }
        goToNextYear(imdb); //cambio el año y el iterador de generos
    }
    fclose(q1);
    fclose(q2);
    fclose(q3);
}

static void Query4(imdbADT Imdb) {
    FILE * archivo = fopen("./query4.csv", "wt");
    if (archivo == NULL) {
        printf("No se pudo crear un archivo\n");
        return;
    }
    fprintf(archivo, "startYear;primaryTitle;numVotes;averageRating\n");
    toBeginRanking(Imdb);
    while(hasNextRanking(Imdb)){
        TRanking aux = nextRanking(Imdb);
        if(aux.startYear == INVALID)
            fprintf(archivo, "-;%s;%ld;%.1f\n", aux.primaryTitle, aux.numVotes, aux.averageRating);
        else
            fprintf(archivo, "%d;%s;%ld;%.1f\n",aux.startYear, aux.primaryTitle, aux.numVotes, aux.averageRating);
    }
    fclose(archivo);
}

void showData(imdbADT Imdb) {
    Query123(Imdb);
    Query4(Imdb);
}
