#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

enum content{TITLE = 0, PTITLE, SYEAR, GENRE = 4, ARATE, NVOT};
enum typeOfContent{MOVIE = 1, TVSERIES};
enum states{FALSE = 0, TRUE};
#define ERRORARG 1 //a definir (puede ser que quede mejor en estados)
#define BLOCK 10

typedef struct TContent{
    char utility;
    char titleType;
    char * primaryTitle;
    unsigned int startYear;
    char * genre;
    double averageRating;
    size_t numVotes;
}TContent;

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
char * copy(char * source){
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
        aux->titleType = MOVIE;
    }else if(!strcmp(token, "tvSeries")){
        aux->titleType = TVSERIES;
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
                token = strtok(NULL, ";"); //Porque sino me quedo en el TOKEN de FIN DE ANIO
                break;
            case GENRE:
                aux->genre = copy(token);
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
int main(int argQty, char * file[]){
    if(argQty != 2){
        printf("Cantidad de argumentos ingresados es incorrecto\n");
        return ERRORARG;
    }
    FILE * text = fopen(file[1], "r");  
    TContent * aux = malloc(sizeof(TContent));
    checkMemory(aux);
    char * s = getLine(text);
    free(s);
    while((s = getLine(text)) != NULL){
        if(getTokens(s, aux)){
            //asignar al adt SOLAMENTE en este caso
            /*if(add(imdb, aux) == ERRORDETERMINADOPORJOSE){
                //llamo a free del adt
                //mensaje de error
                //exit
            }
            */
            //printf("%s\n",aux->primaryTitle);
            //printf("%s\n",aux->primaryTitle);
            free(aux->primaryTitle);
            free(aux->genre);
        }
        free(s);
    }
    //SOLAMENTE LIBERO LA ESTRUCTURA. LOS CHAR * NO
    
    free(aux);
    fclose(text);
    free(s);
    return 0;
}










/*

char ** getData(FILE * text){
    char ** s = malloc(BLOCK * sizeof(char * ));
    unsigned int i = 0;
    while((s[i++] = getLine(text)) != NULL){
        if((i % BLOCK) == 0){
            s = realloc(s, (i + BLOCK) * sizeof(char *));
        }
        //printf("%s\n", s[i-1]);
    } 
    s = realloc(s, (i-1) * sizeof(char *));
    return s;
}

//hola i = 0 i = 1
//chau i = 1 i = 2
//EOF i = 2  i = 3
char ** getData(FILE * text){
    char ** lines;
    char ** linesAux;
    if((lines = malloc(BLOCK * sizeof(char *))) == NULL){
        return NULL;
    }
    unsigned int i = 0;
    while((lines[i++] = getLine(text)) != NULL){
        if((i % BLOCK) == 0){
            linesAux = realloc(lines, (i + BLOCK) * sizeof(char *));
            if(linesAux == NULL){
                //liberar todo
                return NULL;
            }
            lines = linesAux;
        }
        //printf("%s\n", line[i-1]);
    } 
    lines = realloc(lines, (i) * sizeof(char *));
    lines[i-1] = NULL;
    return lines;
}

void getTokens(char * line){
    char * lineAux = strtok(line, ";");
    while( lineAux != NULL ) {
        printf( " %s\n", lineAux );
        lineAux = strtok(NULL, ";");
    }
}
*/