//
// Created by Jose Menta on 06/07/2021.
//

#include "rankingADT.h"
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#define MAX_RANKING 100
#define SCORE_TO_RATING(score) ((score)/10.0)
/*
 * Estrucrura de la lista. Se uso una lista doble para tener el puntero a last y poder eliminar el ultimo y actualizar
 * el last en O(1). Esto le saca operaciones al caso donde hay que sacar de la lista a la pelícual con el menor ratitig
 * por ser más de 100
 */
struct nodeRanking{
    int year;
    char* primaryTitle;
    size_t numVotes;
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
//libera todos los recursos de la lista, incluyendo el string en heap que tiene el título
static void freeListRanking(TListRanking ranking){
    if(ranking==NULL){
        return;
    }
    freeListRanking(ranking->tail);
    free(ranking->primaryTitle);
    free(ranking);
}
void freeRankingADT(rankingADT ranking){
    freeListRanking(ranking->ranking);
    free(ranking);
}
//funcion para comparar 2 nodos en funcion de su raiting y su cantidad de votos en caso de que el primero sea el mismo
static int compareRanking(char score1, long int numVotes1, char score2, long int numVotes2){
    if(score1==score2){
        return numVotes2 - numVotes1;
    }
    return score2-score1;
}
static char score(double rating){
    return (char)(floor(rating*10+0.5));
}
//saca el que tiene menor calificacion si ya no hay espacio y actualiza el puntero a last
static void removeLast(rankingADT ranking){
    if(ranking->last==NULL){//no se debería llamar a esto en ejecucion, se puede sacar despues
        return;
    }
    TListRanking aux=ranking->last;
    ranking->last=ranking->last->prev;//dejo last uno antes
    if(ranking->last==NULL){//no quedan más elementos
        ranking->ranking=NULL;//es un caso por si se cambia la cantidad máxima, no para este uso tampoco, seguro hay 100 antes.
    }
    ranking->last->tail=NULL;//arreglo la lista que queda
    free(aux->primaryTitle);
    free(aux);
}
//agrega un nodo a la lista usando comapre para ordenar los nodos. Deja en *aux el nodo que agrego si este es el ultimo de la lista
//usa de TContent solo los contenidos para el TAD, el resto no es resreferenciado
static TListRanking addRankingList(TListRanking ranking, TListRanking prev,const TContent * content,TListRanking* aux,int* flag){
    int c;
    if(ranking==NULL|| (c= compareRanking(ranking->score, ranking->numVotes, score(content->averageRating), content->numVotes)) > 0){//tengo que agregar el nuevo nodo
        TListRanking ans= malloc(sizeof (struct nodeRanking));
        if(errno==ENOMEM){
        //si no se pudo reservar memoria para el nodo, se deja a la lista como estaba y se idica con el flag
            *flag=ERR;
            return ranking;
        }
        ans->primaryTitle= content->primaryTitle;//copia el puntero a primaryTitle
        ans->year=content->startYear; //puede ser negativo, en el caso donde no pasan el año. Lo considero ahora porque no se ordenan por año
        ans->numVotes=content->numVotes;
        ans->score= score(content->averageRating);//se guardacomo entero para simplificar comparaciones
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
        free(content->primaryTitle);//no se usa el titulo que pasaron
        return ranking;
    }
    ranking->tail= addRankingList(ranking->tail,ranking,content,aux,flag);
    return ranking;
}
int addRanking(rankingADT ranking, const TContent* content){
    if(ranking->cant==MAX_RANKING && ranking->last->score> score(content->averageRating)){
        //si ya no hay espacio y seguro todos los que estan son mejores
        //si ranking->cant==MAX_RANKING, seguro last no es NULL
        free(content->primaryTitle);//no se va a usar el título
        return OK; //ni intento agregarlo, seguro lo voy a sacar despues. no hubo error igual
    }
    TListRanking aux=NULL; //para ver si tengo que cambiar el last
    int flag=0;
    ranking->ranking = addRankingList(ranking->ranking,NULL, content,&aux,&flag);
    if(flag==ERR){
    //Si hubo un error al agregar el nodo, se avisa al front que se debe liberar la estructura y se deja como estaba antes de intentar agregarlo
        return ERR;
    }
    if(aux!=NULL){//encontre un nuevo ultimo nodo
        ranking->last=aux;
    }
    ranking->cant+=flag;//ya sé que es o 0 o 1
    if(ranking->cant>MAX_RANKING){//si se pasa de los 100, tiene que eliminar el peor
        removeLast(ranking);
        //se encarga de liberar el nodo y el char* del titulo que se paso en el front
        ranking->cant--;
    }
    return flag;
}
void toBeginRankingADT(rankingADT ranking){
    ranking->curr=ranking->ranking;
}
int hasNextRankingADT(rankingADT ranking){
    return ranking->curr!=NULL;
}
TRanking nextRankingADT(rankingADT ranking){
    assert(hasNextRankingADT(ranking));
    TRanking ans;
    TListRanking curr=ranking->curr;
    ans.startYear=curr->year;
    ans.averageRating= SCORE_TO_RATING(curr->score);
    ans.numVotes=curr->numVotes;
    ans.primaryTitle=curr->primaryTitle;
    ranking->curr=ranking->curr->tail;
    return ans;
}
