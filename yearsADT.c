//
// Created by Jose Menta on 06/07/2021.
//
//ver si copio los géneros o dejo el puntero que me pasan.
//Ahora los copio para ser constante en la estructura (o copia todos o almacena el puntero)
#include "yearsADT.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#define BLOQUE 10
//cambiar a funcion redondear
#define SCORE_TO_RATING(score) ((score)/10.0)
struct nodeGenre{
    char* genre;
    size_t count;
    struct nodeGenre* tail;
};
typedef struct nodeGenre* TListGenre;
struct movie{//estructura para guardar los datos relevantes de una película/serie
    char* primaryTitle;
    unsigned int len; //longitud del título, se usa para evitar reallocs
    size_t numVotes;
    char score;//vamos a guardar la calificacion como un entero, ocupa menos espacio y es más fácil para comparar
};
struct nodeYear{
    int startYear;
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
//libera los recursos de la lista de géneros
static void freeListGenre(TListGenre genre){
    if(genre==NULL){
        return;
    }
    freeListGenre(genre->tail);
    free(genre->genre);
    free(genre);
}
//libera los recursos de la lista de años
static void freeListYear(TListYear year){
    if(year==NULL){
        return;
    }
    freeListYear(year->tail);
    free(year->bestMovie.primaryTitle);
    free(year->bestSerie.primaryTitle);
    freeListGenre(year->genres);
    free(year);
}
void freeYearsADT(yearsADT years) {
    freeListYear(years->years);
    free(years);
}
static char score(double rating){
    return (char)(floor(rating*10+0.5));
}
//agrega un nodo a la lista de géneros ordenados ascendentemente
static TListGenre addGenre( TListGenre listGenre, char *genre,int* flag){
    int c;
    if(listGenre==NULL || (c= strcmp(listGenre->genre,genre))>0){
        TListGenre ans= malloc(sizeof (struct nodeGenre));
        if(errno==ENOMEM){
        //no fue posible reservar memoria para el nodo. Se deja a la lista como estaba anteriormente y se indica al programa
        //que hubo un error
            *flag=ERR;
            return listGenre;
        }
        ans->genre= genre;//se almacena el puntero que se envia desde el front
        ans->count=1;
        ans->tail=listGenre;
        return ans;
    }
    if(c==0){
        free(genre);
        listGenre->count++;
        return listGenre;
    }
    listGenre->tail= addGenre(listGenre->tail,genre,flag);
    return listGenre;
}
/*
 *Recibe una zona de memoria a partir de la cual copiar el string str y la memoria reservada en la zona anteriormente en el parámetro len.
 *(en realidad la cantidad de espacio reservado si from!=NULL es *len+1, pero no se usa para no hacer un caso especial cuando len=0)
 *Si from==NULL, reserva una nueva zona de memoria. Si no, intenta copiar el string en la zona de memoira reservada anteriormente y
 *agrega memoria de a bloques si se queda sin espacio.
 *Devuelve un char* con el string str, dejando en el parámetro de salida len la longitud del
 *string sin contar el '\0'
*/
static char* copy(char* from, unsigned int* len, const char* str,int* flag){
    char* ans=from;
    unsigned int cap=*len;
    unsigned int indexAns;
    for(indexAns=0;*str!='\0';indexAns++,str++){
        if(indexAns==cap){
            ans= realloc(ans,(indexAns+BLOQUE)*sizeof (char));
            if(errno==ENOMEM){
            //tengo que liberar la estructura, devuelvo NULL para que al liberar la estrucrura no tenga errores al liberar
                free(ans);//revisar si es muy necesario
                *flag=ERR;
                return NULL;
            }
            cap+=BLOQUE;
        }
        ans[indexAns]=*str;
    }
    ans= realloc(ans,(indexAns+1)*sizeof (char));
    if(errno==ENOMEM){
        free(ans);
        *flag=ERR;
        return NULL;
    }
    ans[indexAns]='\0';
    *len=indexAns;
    return ans;
}
/*
 * Recibe un TListYear y actualiza la mejor serie/película en caso de ser la primera o mejor que la que se tenía anteriormente segun su
 * calificación. Se crea una copia del string del título si es necesario con la función copy. Es necesario para su correcto funcionamiento que
 * los TListYear que se pasen no tengan basura. Tabien se actualizan los contadores de películas/series
 */
static void updateMoviesSeries(TListYear ans, char* primaryTitle, char titleType, double averageRating, size_t numVotes, int* flag){
    char scoreToUse= score(averageRating);//lo voy a guardar como un entero
    if(titleType == MOV){
        if(ans->bestMovie.primaryTitle == NULL || ans->bestMovie.numVotes < numVotes){
            ans->bestMovie.primaryTitle= copy(ans->bestMovie.primaryTitle, &ans->bestMovie.len, primaryTitle, flag);
            //si hay un error en la funcion copy, se actualiza el flag correspondiente. Se continúa con la asignacion aca
            //porque ya hay memoria reservada y para evitar casos especiales
            ans->bestMovie.score=scoreToUse;
            ans->bestMovie.numVotes=numVotes;
        }
        ans->cantMovies++;
    }else{//es una serie, otra cosa no se manda al TAD para los queries necesarios
        if(ans->bestSerie.primaryTitle == NULL || ans->bestSerie.numVotes < numVotes){
            ans->bestSerie.primaryTitle= copy(ans->bestSerie.primaryTitle, &ans->bestSerie.len, primaryTitle, flag);
            ans->bestSerie.score=scoreToUse;
            ans->bestSerie.numVotes=numVotes;
        }
        ans->cantSeries++;
    }
}
//agrega un nodo a la lista de años en orden descendente o actualiza la informacion de un año.
static TListYear addYearList(TListYear listYear,const TContent * content, int* flag){
    if(listYear==NULL || listYear->startYear < content->startYear){
        TListYear ans= calloc(1,sizeof (struct nodeYear));
        if(errno==ENOMEM){
        //no fue posible reservar memoria para el nodo. Se deja a la lista como estaba antes y se avisa con el flag
            *flag=ERR;
            return listYear;
        }
        ans->startYear=content->startYear;
        updateMoviesSeries(ans, content->primaryTitle, content->titleType, content->averageRating, content->numVotes, flag);
        if(*flag==ERR){
        //si hubo un error reservando memoria para las películas/series, se libera el nodo reservado anteriormente y se avisa con el flag
            free(ans);
            *flag=ERR;
            return listYear;
        }
        if(content->titleType==MOV) {
            for(int i=0;content->genre[i]!=NULL || *flag==ERR;i++) {
                ans->genres = addGenre(ans->genres, content->genre[i], flag);
            }
        }
        //si el flag queda en ERR despues de esta funcion, igualmente se sigue con la lista de años y despues el front se encarga de liberar la estructura
        //lo que fue reservado antes va a poder liberarse, solo no se pudo reservar para la lista de generos
        //y en ese caso ya se arreglo como para que los otros nodos queden como antes para liberarse con freeYearsADT
        ans->tail=listYear;
        return ans;
    }
    if(listYear->startYear == content->startYear){
        //si ya hay un nodo con esa informacion, agrego los datos de la nueva película/serie
        if(content->titleType==MOV) {
            for(int i=0;content->genre[i]!=NULL && *flag!=ERR;i++){
                listYear->genres = addGenre(listYear->genres, content->genre[i], flag);
            }
        }
        updateMoviesSeries(listYear, content->primaryTitle, content->titleType, content->averageRating, content->numVotes, flag);
        return listYear;
    }
    listYear->tail= addYearList(listYear->tail,content,flag);
    return listYear;
}
int addYear(yearsADT years, const TContent * content){
    int flag=OK;
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
TYear nextYearADT(yearsADT years){
    assert(hasNextYearsADT(years));
    TYear ans;
    TListYear curr=years->currYears;
    ans.startYear=curr->startYear;
    ans.averageRatingMovie= SCORE_TO_RATING(curr->bestMovie.score);
    ans.averageRatingSerie= SCORE_TO_RATING(curr->bestSerie.score);
    ans.dimMovie=curr->cantMovies;
    ans.dimSerie=curr->cantSeries;
    ans.numVotesMovie=curr->bestMovie.numVotes;
    ans.numVotesSerie=curr->bestSerie.numVotes;
    ans.primaryTitleMovie=curr->bestMovie.primaryTitle;//el front no tiene que liberar/cambiar ese puntero
    ans.primaryTitleSerie=curr->bestSerie.primaryTitle;
    //no avanza al siguiente para poder iterar por los generos de este año
    return ans;
}
TGenre nextGenresYearsADT(yearsADT years){
    assert(hasNextGenresYearsADT(years));
    TGenre ans;
    ans.genre=years->currGenre->genre;
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
