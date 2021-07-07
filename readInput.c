#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imdbADT.h"

enum content{PTITLE=1, SYEAR, GENRE = 4, ARATE, NVOT};

//VA O NO VA?
void checkMemory(void * text){
    if(errno == ENOMEM){
        free(text);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
}

//QUITAR DESAUX
char * getLine(FILE * text) {
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
//VERIFICAR EL /N QUE PUEDE APARECER PREGUNTAR.
//enum para sumar punticos.
int getTokens(char * line, TContent * aux){
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
                aux->startYear = atoi(token);
                counter++;
                strtok(NULL, ";"); //Porque sino me quedo en el TOKEN de FIN DE ANIO
                break;  //agregar aca el INVALID
            case GENRE: //si falla el malloc del genero liberar los demas
                aux->genre = copy(token);
                break;
            case ARATE:
                aux->rating = atof(token);
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
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
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

