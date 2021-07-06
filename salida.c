#include <stdio.h>
#include <stdio.h>

//no darle bola a los nombres porque estoy esperando el .h de jose
//titleType;primaryTitle;startYear;endYear;genres;averageRating;numVotes;runtimeMinutes
typedef struct Querry4 {
    int startYear;
    char * primaryTitle;
    long numVotes;
    float averageRating;
} tQ4;

typedef struct genres{
    char * genres;
    int cant;
} tGenres;

typedef struct year{
    int startYear;
    char * primaryTitleMovie;
    long numVotesMovie;
    float averageRatingMovie;
    int dimMovie;
    char * primaryTitleSerie;
    long numVotesSerie;
    float averageRatingSerie;
    int dimSerie;
    tGenres * current;
} tYear;


void Query123(ImdbADT Imdb);
void Query4(ImdbADT Imdb);


void showData(ImdbADT Imdb) {
    Query123(Imdb);
    Query4(Imdb);
}

void Query123(ImdbADT Imdb){
    FILE * q1, * q2, * q3;
    q1 = fopen("../query1.csv", "wt");
    q2 = fopen("../query2.csv", "wt");
    q3 = fopen("../query3.csv", "wt");
    if (q1 == NULL || q2 == NULL || q3 == NULL) { //esto creo que no deberia ir porque siempre se puede crear
        printf("No se pudo crear un archivo\n");
        return;
    }
    while(hasNextYear(Imdb)){
        tYear aux = nextYear(Imdb);
        fprintf(q1, "%d;%d;%d;\n", aux.startYear, aux.dimMovie, aux.dimSerie);
        fprintf(q3, "%d;%s;%ld;%.1f;%s;%ld;%.1f;\n", aux.startYear,
                aux.primaryTitleMovie, aux.numVotesMovie, aux.averageRatingMovie,
                aux.primaryTitleSerie, aux.numVotesSerie, aux.averageRatingSerie);

        while(hasNextGenres(aux)){
            struct structGenres aux2 = nextGenres(aux);
            fprintf(q2, "%d;%s;%d;\n", aux.startYear, aux2.genres, aux2.cant);
        }
        toBeginGenres(aux);
    }
    toBeginYear(Imdb);
    fclose(q1);
    fclose(q2);
    fclose(q3);
}

void Query4(ImdbADT Imdb) {
    FILE * archivo;
    archivo = fopen("../query4.csv", "wt");
    if (archivo == NULL) { //esto creo que no deberia ir porque siempre se puede crear
        printf("No se pudo crear un archivo\n");
        return;
    }
    while(hasNextQ4(Imdb)){
        tQ4 aux = nextQ4(Imdb);
        fprintf(archivo, "%d;%s;%ld;%.1f;\n", aux.startYear, aux.primaryTitle, aux.numVotes, aux.averageRating);
    }
    toBeginQ4(Imdb);
    fclose(archivo);
}
