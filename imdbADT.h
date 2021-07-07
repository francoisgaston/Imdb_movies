//
// Created by Jose Menta on 05/07/2021.
//

#ifndef TPE_FINAL_PI_IMDBADT_H
#define TPE_FINAL_PI_IMDBADT_H
#include "structs.h"
#include <stddef.h>
typedef struct imdbCDT* imdbADT;
imdbADT newImdb(void);
int add(imdbADT imdb, const TContent* content);
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
