//
//
//
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
    if(ans == NULL || errno==ENOMEM){
        return NULL;
    }
    ans->years=newYearsADT();
    if(ans->years == NULL || errno==ENOMEM){
        free(ans);
        return NULL;
    }
    ans->ranking=newRankingADT();
    if(ans->ranking == NULL || errno==ENOMEM){
        freeYearsADT(ans->years);
        free(ans);
        return NULL;
    }
    return ans;
}
int add(imdbADT imdb, const TContent*  content){
    int flag1=OK, flag2=OK;
    if(content->startYear!=INVALID){
    //como en el caso del yearsADT necesita tener un año, si en el csv no había un año no lo agrega como dato
        flag1=addYear(imdb->years, content);
    }else{
        for(int i=0;content->genre[i]!=NULL;i++){
        //se tienen que liberar los géneros, ya que no se utilizarán
            free(content->genre[i]);
        }
    }
    //si se pudo agregar en yearsADT, intenta agregarlo en rankingADT
   if ( flag1!= ERR && content->titleType == MOV && content->numVotes >= MIN_VOTES) {//si cumple con los requisitos
        flag2=addRanking(imdb->ranking, content);//ranking se va a quedar o liberar el char* que manda el front
    }else{
        free(content->primaryTitle); //como no se va a liberar en rankingADT, se tiene que liberar ahora
    }

    if(flag1==ERR || flag2==ERR){ //si hubo error en alguna, se avisa que termine la ejecucion y se libere la estructura
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
TYear nextYear(imdbADT imdb){
    return nextYearADT(imdb->years);
}
TGenre nextGenres(imdbADT imdb){
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
TRanking nextRanking(imdbADT imdb){
    return nextRankingADT(imdb->ranking);
}
