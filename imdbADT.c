//
// Created by Jose Menta on 05/07/2021.
//

#include "imdbADT.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#define MAX_RANKING 100
#define MIN_VOTES 100000
//estructura para ordenar a las películas por año
struct nodeGenre{
    char* genre;
    size_t count;
    struct nodeGenre* tail;
};
typedef struct nodeGenre* TListGenre;
struct movie{//estructura para guardar los datos relevantes de una película/serie
    char* title;
    size_t cantVotes;
    char score;//vamos a guardar la calificacion como un entero, ocupa menos espacio y es más fácil para comparar
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
//estructura para las mejores 100 películas (lista doble)
struct nodeRanking{
    int year;
    char* title;
    size_t cantVotes;
    char score;
    struct nodeRanking* tail;
    struct nodeRanking* prev;
};
typedef struct nodeRanking* TListRanking;
struct imdbCDT{
    //para la lista con los años
    TListYear years;
    TListYear currYears;
    TListGenre currGenre; //iterador de los generos del año donde estoy iterando
    //para la lista con las mejores 100 películas
    TListRanking ranking;
    size_t sizeRanking;
    TListRanking lastRanking;
    TListRanking currRanking;
};

//funciones auxiliares para years
static int addYear(imdbADT imdb, char * title, char type, int startYear, char* genre, char score, size_t cantVotes);
static TListYear addYearList(TListYear listYear, imdbADT imdb,char* title, char type, int startYear, char* genre, char score, size_t cantVotes,int* flag);
static TListYear newYearNode(imdbADT imdbAdt,int startYear, char* genre);
static void updateMoviesSeries(TListYear ans, char* title, char type, char score, size_t cantVotes,int* flag);
static TListGenre addGenre( TListGenre listGenre,imdbADT imdb, char *genre);
static void freeListGenre(TListGenre genre);
static void freeListYear(TListYear year);

//funciones auxiliares para ranking
static int compareRanking(char score1, size_t cantVotes1, char score2, size_t cantVotes2);
static int addRanking(imdbADT imdb, int year, char* title, size_t cantVotes, char score);
static TListRanking addRankingList(imdbADT imdb, TListRanking ranking, TListRanking prev, int year, char* title, size_t cantVotes, char score,TListRanking* aux,int* flag);
static void freeListRanking(TListRanking ranking);
static void removeLast(imdbADT imdb);


void freeImdb(imdbADT imdb){
    freeListYear(imdb->years);
    freeListRanking(imdb->ranking);
    free(imdb);
}

imdbADT newImdb(void){
    imdbADT ans= calloc(1,sizeof(struct imdbCDT));
    if(errno==ENOMEM){//termino la ejecucion, no tengo que liberar más memoria poque no reservé nada
        exit(1);
    }
    return ans;
}
void add(imdbADT imdb, TContent content){
    int add1,add2;
    char scoreUsed=content.rating*10;//se guarda como un entero para facilitar las comaparaciones y ocupar menos espacio
    //flags para ver si me guardé el título en alguna parte, si no lo tengo que liberar porque freeImdb no lo va a poder liberar despues
    add1=addYear(imdb,content.ptimaryTitle,content.titleType,content.startYear,content.genre,scoreUsed,content.numVotes);
    if(content.titleType==MOV && content.numVotes>=MIN_VOTES){//lo agrego a los mejores 100
        add2=addRanking(imdb,content.startYear,content.ptimaryTitle,content.numVotes,scoreUsed);
    }
    if(!add1 && !add2){
        free(content.ptimaryTitle);
    }
}
//iteradores para el front
void toBeginYear(imdbADT imdb){
    imdb->currYears=imdb->years;
    if(imdb->years!=NULL) {
        imdb->currGenre = imdb->currYears->genres;
    }
}
int hasNextyear(imdbADT imdb){
    return imdb->currYears!=NULL;
}
int hasNextGenres(imdbADT imdb){
    return imdb->currGenre!=NULL;
}
tYear nextYear(imdbADT imdb){//no se mueve al proximo, porque necesita iterar por los generos antes
    assert(hasNextyear(imdb));
    tYear ans;
    TListYear curr=imdb->currYears;
    ans.startYear=curr->year;
    ans.averageRatingMovie=curr->bestMovie.score;
    ans.averageRatingSerie=curr->bestSerie.score;
    ans.dimMovie=curr->cantMovies;
    ans.dimSerie=curr->cantSeries;
    ans.numVotesMovie=curr->bestMovie.cantVotes;
    ans.numVotesSerie=curr->bestSerie.cantVotes;
    ans.primaryTitleMovie=curr->bestMovie.title;//no tiene que liberar/cambiar ese puntero
    ans.primaryTitleSerie=curr->bestSerie.title;
    //no avanza al siguiente para poder iterar por los generos de este año
    return ans;
}
tGenres nextGenres(imdbADT imdb){//esta funcion avanza automáticamente al siguiente
    assert(hasNextGenres(imdb));
    tGenres ans;
    ans.genres=imdb->currGenre->genre;
    ans.cant=imdb->currGenre->count;
    imdb->currGenre=imdb->currGenre->tail;
    return ans;
}
void goToNextYear(imdbADT imdb){
    assert(hasNextyear(imdb));
    imdb->currYears=imdb->currYears->tail;
    if(imdb->currYears!=NULL){//setea el iterador por los géneros del año
        imdb->currGenre=imdb->currYears->genres;
    }
}
void toBeginRanking(imdbADT imdb){
    imdb->currRanking=imdb->ranking;
}
int hasNextRanking(imdbADT imdb){
    return imdb->currRanking!=NULL;
}
tQ4 nextRanking(imdbADT imdb){
    assert(hasNextRanking(imdb));
    tQ4 ans;
    TListRanking curr=imdb->currRanking;
    ans.startYear=curr->year;
    ans.averageRating=curr->score/(float)10;
    ans.numVotes=curr->cantVotes;
    ans.primaryTitle=curr->title;
    imdb->currRanking=imdb->currRanking->tail;
    return ans;
}
static void checkMemory(imdbADT imdb){//funcion que usan todas las funciones que reservan memoria
    if(errno==ENOMEM){//tengo que liberar todos los recursos del TAD
        freeImdb(imdb);
        exit(1);
    }
}

//funciones auxiliares para years
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
static TListGenre addGenre( TListGenre listGenre,imdbADT imdb, char *genre){
    int c;
    if(listGenre==NULL || (c= strcmp(listGenre->genre,genre))>0){
        TListGenre ans= malloc(sizeof (struct nodeGenre));
        checkMemory(imdb);
        ans->genre= genre;
        strcpy(ans->genre,genre);
        ans->count=1;
        ans->tail=listGenre;
        return ans;
    }
    if(c==0){
        free(genre);//tengo que liberar el puntero que me pasaron poruqe no lo voy a usar
        listGenre->count++;
        return listGenre;
    }
    listGenre->tail= addGenre(listGenre->tail, imdb,genre);
    return listGenre;
}
static void updateMoviesSeries(TListYear ans, char* title, char type, char score, size_t cantVotes,int* flag){
    if(type==MOV){
        if(ans->bestMovie.title==NULL || ans->bestMovie.score<score){
            free(ans->bestMovie.title);//tengo que liberar el puntero que ya no uso
            ans->bestMovie.title=title;//ya me pasan un puntero en zona dinámica
            *flag=1;//aviso que no liberen el titulo
            ans->bestMovie.score=score;
            ans->bestMovie.cantVotes=cantVotes;
        }
        ans->cantMovies++;
    }else{//es una serie
        if(ans->bestSerie.title==NULL || ans->bestSerie.score<score){
            free(ans->bestSerie.title);
            ans->bestSerie.title=title;
            *flag=1;//no liberen el titulo
            ans->bestSerie.score=score;
            ans->bestSerie.cantVotes=cantVotes;
        }else{
            free(title);
        }
        ans->cantSeries++;
    }
}
static TListYear newYearNode(imdbADT imdb,int startYear, char* genre){
    TListYear ans=malloc(sizeof(struct nodeYear));
    checkMemory(imdb);
    ans->year=startYear;
    ans->cantMovies=0;
    ans->cantSeries=0;
    ans->bestMovie=(struct movie){NULL,0,0};
    ans->bestSerie=(struct movie){NULL,0,0};
    ans->genres=addGenre(ans->genres,imdb,genre);
    return ans;
}
static TListYear addYearList(TListYear listYear, imdbADT imdb,char* title, char type, int startYear, char* genre, char score, size_t cantVotes, int* flag){
    if(listYear==NULL || listYear->year<startYear){//tengo que agregar un nuevo nodo
        TListYear ans= newYearNode(imdb,startYear,genre);
        updateMoviesSeries(ans,title,type,score,cantVotes,flag);
        ans->tail=listYear;
        return ans;
    }
    if(listYear->year==startYear){
        updateMoviesSeries(listYear,title,type,score,cantVotes,flag);
        listYear->genres= addGenre(listYear->genres,imdb,genre);
        return listYear;
    }
    listYear->tail= addYearList(listYear->tail,imdb,title,type,startYear,genre,score,cantVotes,flag);
    return listYear;
}
static int addYear(imdbADT imdb, char * title, char type, int startYear, char* genre, char score, size_t cantVotes){
    int flag=0; //como default, lo tengo que elimiar
    imdb->years= addYearList(imdb->years,imdb,title,type,startYear,genre,score,cantVotes,&flag);
    return flag;
}


//funciones auxiliares para ranking
static int compareRanking(char score1, size_t cantVotes1, char score2, size_t cantVotes2){//ordena la lista del renking en forma descendente
    if(score1==score2){
        return cantVotes2-cantVotes1;
    }
    return score2-score1;
}

static void freeListRanking(TListRanking ranking){
    if(ranking==NULL){
        return;
    }
    freeListRanking(ranking->tail);
    free(ranking->title);
    free(ranking);
}
static TListRanking addRankingList(imdbADT imdb, TListRanking ranking, TListRanking prev, int year, char* title, size_t cantVotes, char score,TListRanking* aux,int* flag){
    int c;
    if(ranking==NULL|| (c= compareRanking(ranking->score,ranking->cantVotes,score,cantVotes))>0){//tengo que agregar el nuevo nodo
        TListRanking ans= malloc(sizeof (struct nodeRanking));
        checkMemory(imdb);
        ans->title= title;
        strcpy(ans->title,title);
        ans->year=year;
        *flag=1;
        ans->cantVotes=cantVotes;
        ans->score=score;
        ans->tail=ranking;
        ans->prev=prev;
        if(ranking==NULL){//es el ultimo nodo en la lista, me lo guardo por si lo tengo que eliminar
            *aux=ans;
        }
        return ans;
    }
    if(c==0){//no deberia haber repetidos, pero por las dudas dejo el caso
        return ranking;
    }
    ranking->tail= addRankingList(imdb,ranking->tail,ranking,year,title,cantVotes,score,aux, flag);
    return ranking;
}
static int addRanking(imdbADT imdb, int year, char* title, size_t cantVotes, char score){
    if(imdb->sizeRanking==MAX_RANKING && imdb->lastRanking->score>score){//si ya no tengo espacio y sé que todos los que estan son mejores
        return 0; //ni intento agregarlo, seguro lo voy a sacar despues
    }
    TListRanking aux=NULL; //para ver si tengo que cambiar el last
    int flag=0;
    imdb->ranking = addRankingList(imdb, imdb->ranking, NULL, year, title, cantVotes, score, &aux, &flag);
    if(aux!=NULL){//encontre un nuevo ultimo nodo
        imdb->lastRanking=aux;
    }
    imdb->sizeRanking+=flag;
    if(imdb->sizeRanking>MAX_RANKING){
        removeLast(imdb);
        imdb->sizeRanking--;
    }
    return flag;//si flag=1, entonces se uso el puntero al titulo y no lo tengo que liberar
}
static void removeLast(imdbADT imdb){//saca el que tiene menor calificacion si ya no hay espacio
    if(imdb->lastRanking==NULL){
        return;
    }
    TListRanking aux=imdb->lastRanking;
    imdb->lastRanking=imdb->lastRanking->prev;
    if(imdb->lastRanking==NULL){//no quedan más elementos
        imdb->ranking=NULL;//es un caso por si se cambia la cantidad máxima, no para este uso
    }
    imdb->lastRanking->tail=NULL;//no deberia ver el caso donde el anterior es NULL, se supone que hay 100 atras
    free(aux->title);
    free(aux);
}
