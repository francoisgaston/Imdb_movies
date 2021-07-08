//
// Created by Jose Menta on 06/07/2021.
//

#ifndef TPE_FINAL_PI_STRUCTS_H
#define TPE_FINAL_PI_STRUCTS_H
#include <stddef.h>

#define BLOCK 10
#define ERR 10
#define OK 1
#define INVALID -1


enum typeOfContent{MOV = 1, SER};
enum states{FALSE = 0, TRUE};


//estructura para pasar contenido al TAD
typedef struct TContent{
    char titleType;
    char* primaryTitle;
    int startYear;
    char ** genre;
    double averageRating;
    size_t numVotes;
}TContent;
//estructuras auxiliares para los iteradores
//estructura para iterar en el top 100 de peliculas
typedef struct Query4 {
    int startYear;
    char * primaryTitle;
    size_t numVotes;
    double averageRating;
} TRanking;
//estructura para iterar en los géneros de cada año
typedef struct genres{
    char * genre;
    size_t cant;
} TGenre;
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
} TYear;
#endif //TPE_FINAL_PI_STRUCTS_H
