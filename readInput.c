#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imdbADT.h"

enum content{PTITLE=1, SYEAR, GENRE = 4, ARATE, NVOT};

//VA O NO VA?
static void checkMemory(void * text){
    if(errno == ENOMEM){
        free(text);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
}

static void checkMemGenre(TContent * aux, int idx){
    if(errno == ENOMEM) {
        for (int i = 0; i < idx; i++)
            free(aux->genre[i]);
        free(aux->genre);
        free(aux->primaryTitle);
        free(aux);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
}

//QUITAR DESAUX
static char * getLine(FILE * text) {
    unsigned int i = 0;
    int c;
    char * desc = NULL;
    while( (c = fgetc(text)) != '\n' && c != EOF){
        if((i % BLOCK) == 0){
            desc = realloc(desc, (i + BLOCK) * sizeof(char));
            //errno == ENOMEM (PREGUNTAR)
            checkMemory(desc);
        }
        desc[i++] = c;
    }
    if(desc != NULL){
        desc = realloc(desc, (i+1) * sizeof(char));
        checkMemory(desc);
        desc[i] = 0;
    }
    return desc;
}
static char * copy(char * source){
    char * ans = malloc((strlen(source)+1) * sizeof(char));
    checkMemory(ans);
    strcpy(ans, source);
    return ans;
}

static void copy_genres(TContent * aux , char * line){
    aux->genre = NULL;
    char * token = strtok(line, ",");
    int i = 0;
    for(; token != NULL ; i++,token = strtok(NULL, ",")){
        if((i % BLOCK) == 0) {
            aux->genre = realloc(aux->genre, (BLOCK + i) * sizeof(char *));
            checkMemGenre(aux->genre,BLOCK+i);
        }
        aux->genre[i] = copy(token);
    }
    aux->genre = realloc(aux->genre, (i+1) * sizeof(char *));
    checkMemGenre(aux->genre,i+1);
    aux->genre[i] = NULL;
}

//VERIFICAR EL /N QUE PUEDE APARECER PREGUNTAR.
//enum para sumar punticos.
static int getTokens(char * line, TContent * aux){
    char * token = strtok(line, ";");
    if(!strcmp(token, "movie")){
        aux->titleType = MOV;
    }else if(!strcmp(token, "tvSeries")){
        aux->titleType = SER;
    }else{
        return FALSE;
    }
    char counter = 0;
    while((token = strtok(NULL, ";")) != NULL){ 
        switch(++counter){
            case PTITLE:
                aux->primaryTitle = copy(token);
                break;
            case SYEAR:
                aux->startYear = (!strcmp(token, "\\N")) ? INVALID : atoi(token);
                counter++;
                strtok(NULL, ";"); //Porque sino me quedo en el TOKEN de FIN DE ANIO
                break;  //agregar aca el INVALID
            case GENRE: //si falla el malloc del genero liberar los demas
                copy_genres(aux,token);
                break;
            case ARATE:
                aux->averageRating = atof(token);
                break;
            case NVOT:
                aux->numVotes = atoi(token);
                return TRUE;
            default:
                break;
        }
    }
    return TRUE;
}

//PASAR SOLO UNA ESTRUCTURA
//NO LIBERAR LOS CHAR * DE LA ESTRUCTURA
void readInput(int argQty, char * file[],imdbADT imdb){
    if(argQty != 2){
        fprintf(stderr, "CANTIDAD DE ARGUMENTOS INVALIDA!");
        exit(1);
    }
    FILE * text = fopen(file[1], "r");  
    TContent * aux = malloc(sizeof(TContent));
    checkMemory(aux);
    char * s = getLine(text);
    free(s);
    while((s = getLine(text)) != NULL){
        if(getTokens(s, aux)){
            //asignar al adt SOLAMENTE en este caso
            if(add(imdb, aux) == ERR){
                freeImdb(imdb);
                fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
                exit(1);
            }
        }
        free(s);
    }
    //SOLAMENTE LIBERO LA ESTRUCTURA. LOS CHAR * NO
    free(aux);
    fclose(text);
    free(s);
}

