//
// Created by Jose Menta on 05/07/2021.
//
//hay que arreglar como termina si hay error en malloc. Ver de separar el TAD en 2 distintos para que quede más manejable
#include "imdbADT.h"
#include "yearsADT.h"
#include "rankingADT.h"
#include <stdlib.h>
#include <errno.h>
#define MIN_VOTES 100000
struct imdbCDT{
    yearsADT years;
    rankingADT ranking;
};
void freeImdb(imdbADT imdb){
    freeYearsADT(imdb->years);
    freeRankingADT(imdb->ranking);
    free(imdb);
}

imdbADT newImdb(void){//libera automáticamente si no pudo reservar algo, y devuelve NULL
    imdbADT ans= calloc(1,sizeof(struct imdbCDT));
    if(errno==ENOMEM){
        return NULL;
        //usar errno en front, preguntar si se toca despues
    }
    ans->years=newYearsADT();
    if(errno==ENOMEM){
        free(ans);
        return NULL;
    }
    ans->ranking=newRankingADT();
    if(errno==ENOMEM){
        freeYearsADT(ans->years);
        free(ans);
        return NULL;
    }
    return ans;
}
int add(imdbADT imdb, const TContent*  content){
    int flag1=OK, flag2=OK;
    if(content->startYear!=INVALID){ //si o si necesito que tenga un año para este query
        flag1=addYear(imdb->years, content);
    }
    if(flag1!=ERR) {
        if (content->titleType == MOV && content->numVotes >= MIN_VOTES) {//intento agregarlo a los mejores 100
            flag2=addRanking(imdb->ranking, content);//ranking se va a quedar o liberar el char* que manda el front
        }
    }
    if(flag1==ERR || flag2==ERR){
        return ERR;
    }
    return OK;
}
//iteradores para el front
void toBeginYear(imdbADT imdb){
    toBeginYearsADT(imdb->years);
}
int hasNextYear(imdbADT imdb){
    return hasNextYearsADT(imdb->years);
}
int hasNextGenres(imdbADT imdb){
    return hasNextGenresYearsADT(imdb->years);
}
tYear nextYear(imdbADT imdb){//no se mueve al proximo, porque necesita iterar por los generos antes
    return nextYearADT(imdb->years);
}
tGenres nextGenres(imdbADT imdb){//esta funcion avanza automáticamente al siguiente
    return nextGenresYearsADT(imdb->years);
}
void goToNextYear(imdbADT imdb){
    goToNextYearsADT(imdb->years);
}
void toBeginRanking(imdbADT imdb){
    toBeginRankingADT(imdb->ranking);
}
int hasNextRanking(imdbADT imdb){
    return hasNextRankingADT(imdb->ranking);
}
tQ4 nextRanking(imdbADT imdb){
    return nextRankingADT(imdb->ranking);
}
