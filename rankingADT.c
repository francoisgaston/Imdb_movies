//
//
//

#include "rankingADT.h"
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#define MAX_RANKING 100
#define SCORE_TO_RATING(score) ((score)/10.0)
/*
 * Estrucrura de la lista. Se usa una lista doble para tener el puntero a last y poder eliminar el ultimo y actualizar
 * el last de manera eficiente. Esto le saca operaciones al caso donde hay que sacar de la lista a la pelícual con el menor ratitig
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
//funcion para comparar 2 nodos en funcion de su rating y su cantidad de votos en caso de que el primero sea el mismo
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
    if(ranking->last==NULL){
        return;
    }
    TListRanking aux=ranking->last;
    ranking->last=ranking->last->prev;//deja last uno antes
    if(ranking->last==NULL){//no quedan más elementos, en el caso donde MAX_RANKING es 1
        ranking->ranking=NULL;//es un caso por si se cambia la cantidad máxima, no para este uso pues seguro hay 100 antes.
    }
    ranking->last->tail=NULL;//arregla la lista que queda
    free(aux->primaryTitle);
    free(aux);
}
//agrega un nodo a la lista usando comapre para ordenar los nodos. Deja en *aux el nodo que agrego si este es el ultimo de la lista
//usa de TContent solo los contenidos para el TAD, el resto no es desreferenciado.
static TListRanking addRankingList(TListRanking ranking, TListRanking prev,const TContent * content,TListRanking* aux,int* flag){
    int c;
    if(ranking==NULL|| (c= compareRanking(ranking->score, ranking->numVotes, score(content->averageRating), content->numVotes)) > 0){//tengo que agregar el nuevo nodo
        TListRanking ans= malloc(sizeof (struct nodeRanking));
        if(ans==NULL || errno==ENOMEM){
        //si no se pudo reservar memoria para el nodo, se deja a la lista como estaba y se idica con el flag
            free(ans);
            *flag=ERR;
            return ranking;
        }
        ans->primaryTitle= content->primaryTitle;//copia el puntero a primaryTitle
        ans->year=content->startYear;
        ans->numVotes=content->numVotes;
        ans->score= score(content->averageRating);//se guarda como entero para simplificar comparaciones
        ans->tail=ranking;
        ans->prev=prev;
        *flag=1; //tiene que sumar a la cantidad de elementos
        if(ranking==NULL){//es el ultimo nodo en la lista, lo guarda por si se tiene que eliminar
            *aux=ans;
        }else{//arregla el anterior del próximo nodo
            ranking->prev=ans;
        }
        return ans;
    }
    if(c==0){//no deberia haber repetidos, pero por las dudas dejo el caso. Se queda la película que se envió antes al TAD, no se agrega repetida
        free(content->primaryTitle);//no se usa el titulo que pasaron
        return ranking;
    }
    ranking->tail= addRankingList(ranking->tail,ranking,content,aux,flag);
    return ranking;
}
int addRanking(rankingADT ranking, const TContent* content){
    if(ranking->cant==MAX_RANKING && compareRanking(score(content->averageRating),content->numVotes,ranking->last->score,ranking->last->numVotes)>0){
        //si ya no hay espacio y seguro todos los que estan son mejores (pues el nuevo iría despúes del ultimo nodo, que es el peor)
        //si ranking->cant==MAX_RANKING, seguro last no es NULL (no tiene sentido que MAX_RANKING sea 0)
        free(content->primaryTitle);//no se va a usar el título
        return OK;
    }
    TListRanking aux=NULL; //para ver si tiene que cambiar el last
    int flag=0;
    ranking->ranking = addRankingList(ranking->ranking,NULL, content,&aux,&flag);
    if(flag==ERR){
    //Si hubo un error al agregar el nodo, se avisa al front que se debe liberar la estructura y se deja como estaba antes de intentar agregarlo
        return ERR;
    }
    if(aux!=NULL){//encontro un nuevo ultimo nodo
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
