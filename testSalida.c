#include <stdio.h>
#include <stdlib.h>

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
    tGenres vector[2];
} tYear;

typedef struct ImdbADT{
    tYear vector[2];
    tQ4 vector2[2];
}ImdbADT;

void Query123(ImdbADT Imdb);
void Query4(ImdbADT Imdb);
void showData(ImdbADT Imdb);


int main(){
    ImdbADT Imdb={{{2021, "pepe", 12, 1.8, 5, "jose", 13, 2.5, 8,
                           {{"drama", 4},{"CF", 45}}},
                                           {20212, "peoe", 12, 1.8, 5, "jose", 13, 2.5, 8,
                                                   {{"drama", 4},{"CF", 45}}}},
                  {{2023, "jose", 34, 3.6},{2023, "jose", 34, 3.6}}};
    showData(Imdb);
}


void showData(ImdbADT Imdb){
    Query123(Imdb);
    Query4(Imdb);
}

void Query123(ImdbADT Imdb){
    FILE * q1, * q2, * q3;
    q1 = fopen("../query1.txt", "wt");
    q2 = fopen("../query2.txt", "wt");
    q3 = fopen("../query3.txt", "wt");
    if (q1 == NULL || q2 == NULL || q3 == NULL) { //esto creo que no deberia ir porque siempre se puede crear
        printf("No se pudo crear el archivo del Querry 4\n");
        return;
    }
    int i=0;
    while(i!=2){
        tYear aux = Imdb.vector[i++];
        fprintf(q1, "%d;%d;%d;\n", aux.startYear, aux.dimMovie, aux.dimSerie);
        fprintf(q3, "%d;%s;%ld;%.1f;%s;%ld;%.1f;\n", aux.startYear,
                aux.primaryTitleMovie, aux.numVotesMovie, aux.averageRatingMovie,
                aux.primaryTitleSerie, aux.numVotesSerie, aux.averageRatingSerie);
        int j=0;
        while(j!=2){
            tGenres aux2 = aux.vector[j++];
            fprintf(q2, "%d;%s;%d;\n", aux.startYear, aux2.genres, aux2.cant);
        }
    }
    fclose(q1);
    fclose(q2);
    fclose(q3);
}

void Query4(ImdbADT Imdb) {
    FILE * archivo;
    archivo = fopen("../query4.txt", "wt");
    if (archivo == NULL) { //esto creo que no deberia ir porque siempre se puede crear
        printf("No se pudo crear el archivo del Querry 4\n");
        return;
    }
    int i=0;
    while(i!=2){
        tQ4 aux = Imdb.vector2[i++];
        fprintf(archivo, "%d;%s;%ld;%.1f;\n", aux.startYear, aux.primaryTitle, aux.numVotes, aux.averageRating);
    }
    fclose(archivo);
}



