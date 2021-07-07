#include <stdio.h>
#include "imdbADT.h"


void Query123(imdbADT Imdb);
void Query4(imdbADT Imdb);


void showData(imdbADT Imdb) {
    Query123(Imdb);
    Query4(Imdb);
}

void Query123(imdbADT imdb){
    FILE * q1, * q2, * q3;
    q1 = fopen("../query1.csv", "wt");
    q2 = fopen("../query2.csv", "wt");
    q3 = fopen("../query3.csv", "wt");
    if (q1 == NULL || q2 == NULL || q3 == NULL) {
        printf("No se pudo crear un archivo\n");
        return;
    }
    //headers
    fprintf(q1, "year;films;series");
    fprintf(q2, "year;genre;films");
    fprintf(q3, "startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie");
    //data
    toBeginYear(imdb);
    while(hasNextYear(imdb)){
        tYear aux = nextYear(imdb);
        fprintf(q1, "%d;%ld;%ld;\n", aux.startYear, aux.dimMovie, aux.dimSerie);
        fprintf(q3, "%d;%s;%ld;%.1f;%s;%ld;%.1f;\n", aux.startYear,
                aux.primaryTitleMovie, aux.numVotesMovie, aux.averageRatingMovie,
                aux.primaryTitleSerie, aux.numVotesSerie, aux.averageRatingSerie);
        while(hasNextGenres(imdb)){
            tGenres aux2 = nextGenres(imdb);
            fprintf(q2, "%d;%s;%ld;\n", aux.startYear, aux2.genres, aux2.cant);
        }
        goToNextYear(imdb);
    }
    fclose(q1);
    fclose(q2);
    fclose(q3);
}

void Query4(imdbADT Imdb) {
    FILE * archivo;
    archivo = fopen("../query4.csv", "wt");
    if (archivo == NULL) {
        printf("No se pudo crear un archivo\n");
        return;
    }
    fprintf(archivo, "startYear;primaryTitle;numVotes;averageRating");
    while(hasNextRanking(Imdb)){
        tQ4 aux = nextRanking(Imdb);
        if(aux.startYear == INVALID)
            fprintf(archivo, "Año sin definir;%s;%ld;%.1f;\n", aux.primaryTitle, aux.numVotes, aux.averageRating);
        else
            fprintf(archivo, "%d;%s;%ld;%.1f;\n",aux.startYear, aux.primaryTitle, aux.numVotes, aux.averageRating);
    }
    toBeginRanking(Imdb);
    fclose(archivo);
}
