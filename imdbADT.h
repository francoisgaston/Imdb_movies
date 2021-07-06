//
// Created by Jose Menta on 05/07/2021.
//

#ifndef TPE_FINAL_PI_IMDBADT_H
#define TPE_FINAL_PI_IMDBADT_H
#include <stddef.h>
//flags para pasar el tipo del elemento sin pasar el string
#define MOV 1
#define SER 2
//estructura para pasar contenido al TAD
typedef struct TContent{
    char titleType;
    char* ptimaryTitle;
    size_t startYear;
    char* genre;
    double rating;
    size_t numVotes;
}TContent;
//estructuras auxiliares para los iteradores
//estructura para iterar en el top 100 de peliculas
typedef struct Querry4 {
    int startYear;
    char * primaryTitle;
    long numVotes;
    float averageRating;
} tQ4;
//estructura para iterar en los géneros de cada año
typedef struct genres{
    char * genres;
    int cant;
} tGenres;
//estructura para iterar en la lista de años
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
    tGenres * current;//esto debería ser un vector o un puntero? No puedo pasar el tipo del puntero al front
} tYear;

typedef struct imdbCDT* imdbADT;
imdbADT newImdb(void);
void add(imdbADT imdb, struct TContent content);
void freeImdb(imdbADT imdb);
void toBeginYear(imdbADT imdb);
int hasNextYear(imdbADT imdb);
tYear nextYear(imdbADT imdb);
void goToNextYear(imdbADT imdb);
int hasNextGenres(imdbADT imdb); //se ubica en el año donde esta el iterador de years, no le paso otra cosa
tGenres nextGenres(imdbADT);
void toBeginRanking(imdbADT imdb);
int hasNextRanking(imdbADT imdb);
tQ4 nextRanking(imdbADT imdb);

#endif //TPE_FINAL_PI_IMDBADT_H
