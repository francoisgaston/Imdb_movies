//
// Created by Jose Menta on 06/07/2021.
//

#ifndef TPE_FINAL_PI_STRUCTS_H
#define TPE_FINAL_PI_STRUCTS_H
#include <stddef.h>
//flags para pasar el tipo del elemento sin pasar el string
#define MOV 1 //ver de pasar a 3 para no tener cosas con el mismo valor
#define SER 2

#define ERR 10
#define OK 1
#define INVALID -1
//estructura para pasar contenido al TAD
typedef struct TContent{
    char titleType;
    char* primaryTitle;
    int startYear;
    char* genre;
    double rating;
    size_t numVotes;
}TContent;
//estructuras auxiliares para los iteradores
//estructura para iterar en el top 100 de peliculas
typedef struct Querry4 {
    int startYear;
    char * primaryTitle;
    size_t numVotes;
    double averageRating;
} tQ4;
//estructura para iterar en los géneros de cada año
typedef struct genres{
    char * genres;
    size_t cant;
} tGenres;
//estructura para iterar en la lista de años
typedef struct year{
    int startYear;
    char * primaryTitleMovie;
    size_t numVotesMovie;
    double averageRatingMovie;
    size_t dimMovie;
    char * primaryTitleSerie;
    size_t numVotesSerie;
    double averageRatingSerie;
    size_t dimSerie;
    tGenres * current;//esto debería ser un vector o un puntero? No puedo pasar el tipo del puntero al front
} tYear;
#endif //TPE_FINAL_PI_STRUCTS_H
