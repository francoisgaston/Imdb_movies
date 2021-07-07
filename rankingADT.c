//
// Created by Jose Menta on 06/07/2021.
//

#include "rankingADT.h"
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#define MAX_RANKING 100
#define SCORE(score) ((int)((score)*10))
struct nodeRanking{
    int year;
    char* title;
    size_t cantVotes;
    char score;
    struct nodeRanking* tail;
    struct nodeRanking* prev;
};
typedef struct nodeRanking* TListRanking;
struct rankingCDT{
    TListRanking ranking;
    size_t cant;
    TListRanking last;
    TListRanking curr;
};
rankingADT newRankingADT(){
    return calloc(1,sizeof (struct rankingCDT));
}
static void freeListRanking(TListRanking ranking){
    if(ranking==NULL){
        return;
    }
    freeListRanking(ranking->tail);
    free(ranking->title);
    free(ranking);
}
void freeRankingADT(rankingADT ranking){
    freeListRanking(ranking->ranking);
    free(ranking);
}
static int compareRanking(char score1, long int cantVotes1, char score2, long int cantVotes2){//ordena la lista del renking en forma descendente
    if(score1==score2){
        return cantVotes2-cantVotes1;
    }
    return score2-score1;
}
static void removeLast(rankingADT ranking){//saca el que tiene menor calificacion si ya no hay espacio
    if(ranking->last==NULL){//no se debería llamar a esto en ejecucion, se puede sacar despues
        return;
    }
    TListRanking aux=ranking->last;
    ranking->last=ranking->last->prev;//dejo last uno antes
    if(ranking->last==NULL){//no quedan más elementos
        ranking->ranking=NULL;//es un caso por si se cambia la cantidad máxima, no para este uso tampoco, seguro hay 100 antes ahora
    }
    ranking->last->tail=NULL;//arreglo la lista que queda
    free(aux->title);
    free(aux);
}
static TListRanking addRankingList(TListRanking ranking, TListRanking prev,const TContent * content,TListRanking* aux,int* flag){
    int c;
    if(ranking==NULL|| (c= compareRanking(ranking->score,ranking->cantVotes,SCORE(content->rating),content->numVotes))>0){//tengo que agregar el nuevo nodo
        TListRanking ans= malloc(sizeof (struct nodeRanking));
        if(errno==ENOMEM){
            *flag=ERR;
            return ranking;//dejo la lista como estaba antes
        }
        ans->title= content->ptimaryTitle;
        ans->year=content->startYear; //puede ser negativo, en el caso donde no pasan el año
        ans->cantVotes=content->numVotes;
        ans->score= SCORE(content->rating);//lo guardo como entero para simplificar comparaciones
        ans->tail=ranking;
        ans->prev=prev;
        *flag=1; //tengo que sumar a la cantidad de elementos
        if(ranking==NULL){//es el ultimo nodo en la lista, me lo guardo por si lo tengo que eliminar
            *aux=ans;
        }else{
            ranking->prev=ans;
        }
        return ans;
    }
    if(c==0){//no deberia haber repetidos, pero por las dudas dejo el caso
        return ranking;
    }
    ranking->tail= addRankingList(ranking->tail,ranking,content,aux,flag);
    return ranking;
}
int addRanking(rankingADT ranking, const TContent* content){
    if(ranking->cant==MAX_RANKING && ranking->last->score> SCORE(content->rating)){
        //si ya no tengo espacio y sé que todos los que estan son mejores
        //este caso da eficiencia, si no recorro toda la lista y seguro despues lo tengo que eliminar
        free(content->ptimaryTitle);
        return NOT_ERR; //ni intento agregarlo, seguro lo voy a sacar despues. no hubo error igual
    }
    TListRanking aux=NULL; //para ver si tengo que cambiar el last
    int flag=0;
    ranking->ranking = addRankingList(ranking->ranking,NULL, content,&aux,&flag);
    if(flag==ERR){
    //ya no hago más, ahora despues se tiene que eliminar el TAD
        return ERR;
    }
    if(aux!=NULL){//encontre un nuevo ultimo nodo
        ranking->last=aux;
    }
    ranking->cant+=flag;//ya sé que es o 0 o 1
    if(ranking->cant>MAX_RANKING){//si me paso de los 100, tengo que eliminar el peor
        removeLast(ranking);
        //se encarga de liberar el nodo y el char* del titulo que se paso en el front
        ranking->cant--;
    }
    return flag;//si flag=1, entonces se uso el puntero al titulo y no lo tengo que liberar
}
void toBeginRankingADT(rankingADT ranking){
    ranking->curr=ranking->ranking;
}
int hasNextRankingADT(rankingADT ranking){
    return ranking->curr!=NULL;
}
tQ4 nextRankingADT(rankingADT ranking){
    assert(hasNextRankingADT(ranking));
    tQ4 ans;
    TListRanking curr=ranking->curr;
    ans.startYear=curr->year;
    ans.averageRating=curr->score/10.0;
    ans.numVotes=curr->cantVotes;
    ans.primaryTitle=curr->title;
    ranking->curr=ranking->curr->tail;
    return ans;
}
