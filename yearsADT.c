//
// Created by Jose Menta on 06/07/2021.
//

#include "yearsADT.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#define BLOQUE 10
#define SCORE(score) ((int)((score)*10))
struct nodeGenre{
    char* genre;
    size_t count;
    struct nodeGenre* tail;
};
typedef struct nodeGenre* TListGenre;
struct movie{//estructura para guardar los datos relevantes de una película/serie
    char* title;
    unsigned int len;
    size_t cantVotes;
    int score;//vamos a guardar la calificacion como un entero, ocupa menos espacio y es más fácil para comparar
};
struct nodeYear{
    int year;
    struct movie bestMovie;
    struct movie bestSerie;
    size_t cantMovies;
    size_t cantSeries;
    TListGenre genres;
    struct nodeYear* tail;
};
typedef struct nodeYear* TListYear;
struct yearsCDT{
    TListYear years;
    TListYear currYears;
    TListGenre currGenre; //iterador de los generos del año donde estoy iterando
};
yearsADT newYearsADT(void){//donde lo llamo tengo que preguntar si errno=ENOMEM para seguir o terminar
    return calloc(1,sizeof (struct yearsCDT));
}
static void freeListGenre(TListGenre genre){
    if(genre==NULL){
        return;
    }
    freeListGenre(genre->tail);
    free(genre->genre);
    free(genre);
}
static void freeListYear(TListYear year){
    if(year==NULL){
        return;
    }
    freeListYear(year->tail);
    free(year->bestMovie.title);
    free(year->bestSerie.title);
    freeListGenre(year->genres);
    free(year);
}
void freeYearsADT(yearsADT years){
    freeListYear(years->years);
    free(years);
}
static TListGenre addGenre( TListGenre listGenre, char *genre,int* flag){
    int c;
    if(listGenre==NULL || (c= strcmp(listGenre->genre,genre))>0){
        TListGenre ans= malloc(sizeof (struct nodeGenre));
        if(errno==ENOMEM){//no reservo memoria. dejo la estructura como antes y paso un flag
            free(genre);//tengo que liberar el puntero al genero porque seguro no lo uso
            *flag=ERR;
            return listGenre;//termina aca
        }
        ans->genre= genre;
        ans->count=1;
        ans->tail=listGenre;
        return ans;
    }
    if(c==0){
        free(genre);//tengo que liberar el puntero que me pasaron poruqe no lo voy a usar en otro lugar
        listGenre->count++;
        return listGenre;
    }
    listGenre->tail= addGenre(listGenre->tail,genre,flag);
    return listGenre;
}
char* copy(char* from, unsigned int* len, const char* str,int* flag){
    //tuve que hacerlo porque si no copiaba el título aca, despues ver si lo tenia
    //que liberar o no era muy complicado
    char* ans=from;
    unsigned int cap=*len;
    unsigned int indexAns;
    for(indexAns=0;*str!='\0';indexAns++,str++){
        if(indexAns==cap){
            ans= realloc(ans,(indexAns+BLOQUE)*sizeof (char));
            if(errno==ENOMEM){//tengo que liberar la estructura, ans lo dejo porque como no pudo reservar memoria no lo tengo que liberar
                free(ans);
                *flag=ERR;
                return NULL;
            }
            cap+=BLOQUE;
        }
        ans[indexAns]=*str;
    }
    ans= realloc(ans,(indexAns+1)*sizeof (char));
    if(errno==ENOMEM){//tengo que liberar la estructura, ans lo dejo porque como no pudo reservar memoria no lo tengo que liberar
        free(ans);
        *flag=ERR;
        return NULL;
    }
    ans[indexAns]='\0';
    *len=indexAns;
    return ans;
}
static void updateMoviesSeries(TListYear ans, char* title, char type, double score, size_t cantVotes,int* flag){
    int scoreToUse= SCORE(score);//lo voy a guardar como un entero
    if(type==MOV){
        if(ans->bestMovie.title==NULL || ans->bestMovie.score<scoreToUse){
            ans->bestMovie.title= copy(ans->bestMovie.title,&ans->bestMovie.len,title,flag);
            ans->bestMovie.score=scoreToUse;
            ans->bestMovie.cantVotes=cantVotes;
        }
        ans->cantMovies++;
    }else{//es una serie, otra cosa no se manda al TAD
        if(ans->bestSerie.title==NULL || ans->bestSerie.score<scoreToUse){
            ans->bestSerie.title= copy(ans->bestSerie.title,&ans->bestSerie.len,title,flag);
            ans->bestSerie.score=scoreToUse;
            ans->bestSerie.cantVotes=cantVotes;
        }
        ans->cantSeries++;
    }
}
static TListYear addYearList(TListYear listYear,const TContent * content, int* flag){
    if(listYear==NULL || listYear->year<content->startYear){//tengo que agregar un nuevo nodo
        TListYear ans= calloc(1,sizeof (struct nodeYear));//la mayoria de las cosas las necesito en 0
        ans->year=content->startYear;
        if(errno==ENOMEM){//no pude crear el nodo
            *flag=ERR;
            return listYear;//dejo como estaba
        }
        updateMoviesSeries(ans,content->ptimaryTitle,content->titleType,content->rating,content->numVotes,flag);
        if(*flag==ERR){//error actualizando las películas/series
            free(ans);
            return listYear;//dejo como estaba antes
            //ver despues si se puede dejar como en lo de abajo, creo que si poruqe si no puedo copiarlo devuelvo NULL y puedo hacer free de eso
        }
        if(content->titleType==MOV) {//tengo que considerarlo para los generos en Q3
            ans->genres = addGenre(ans->genres, content->genre, flag);
        }
        //si el flag queda en ERR despues de esta funcion, igualmente sigo con eso y despues libero la estructura, lo que se va a hacer despues en el front
        //lo que fue reservado antes va a poder liberarse, solo no se pudo reservar para la lista de generos
        //y en ese caso ya se arreglo como para que los otros nodos queden como antes para liberarse con freeYearsADT
        ans->tail=listYear;
        return ans;
    }
    if(listYear->year==content->startYear){
        if(content->titleType==MOV) {
            listYear->genres= addGenre(listYear->genres,content->genre,flag);
            //nuevamente, si da error el front se encarga de liberar tood
        }
        updateMoviesSeries(listYear, content->ptimaryTitle,content->titleType, content->rating, content->numVotes, flag);
        return listYear;
    }
    listYear->tail= addYearList(listYear->tail,content,flag);
    return listYear;
}
int addYear(yearsADT years, const TContent * content){
    int flag=0;
    years->years= addYearList(years->years,content,&flag);
    return flag;
}
void toBeginYearsADT(yearsADT years){
    years->currYears=years->years;
    if(years->currYears!=NULL){
        years->currGenre=years->currYears->genres;
    }
}
int hasNextYearsADT(yearsADT years){
    return years->currYears!=NULL;
}
int hasNextGenresYearsADT(yearsADT years){
    return years->currGenre!=NULL;
}
tYear nextYearADT(yearsADT years){
    assert(hasNextYearsADT(years));
    tYear ans;
    TListYear curr=years->currYears;
    ans.startYear=curr->year;
    ans.averageRatingMovie=curr->bestMovie.score/10.0;//dicen que es mejor un define aca? Que nombre le ponemos?
    ans.averageRatingSerie=curr->bestSerie.score/10.0;
    ans.dimMovie=curr->cantMovies;
    ans.dimSerie=curr->cantSeries;
    ans.numVotesMovie=curr->bestMovie.cantVotes;
    ans.numVotesSerie=curr->bestSerie.cantVotes;
    ans.primaryTitleMovie=curr->bestMovie.title;//no tiene que liberar/cambiar ese puntero
    ans.primaryTitleSerie=curr->bestSerie.title;
    //no avanza al siguiente para poder iterar por los generos de este año
    return ans;
}
tGenres nextGenresYearsADT(yearsADT years){
    assert(hasNextGenresYearsADT(years));
    tGenres ans;
    ans.genres=years->currGenre->genre;
    ans.cant=years->currGenre->count;
    years->currGenre=years->currGenre->tail;
    return ans;
}
void goToNextYearsADT(yearsADT years){
    assert(hasNextYearsADT(years));
    years->currYears=years->currYears->tail;
    if(years->currYears!=NULL){
        years->currGenre=years->currYears->genres;
    }
}
