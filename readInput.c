#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imdbADT.h"

enum content{PTITLE=1, SYEAR, GENRE = 4, ARATE, NVOT};

// Funcion que verifica si hay memoria disponible para guardar data.
static void checkMemory(void * text){
    if(errno == ENOMEM){
        free(text);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
}

/* Funcion que verifica memoria disponible para el char ** genre.
** Además libera la estructura antes de abortar.
*/
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
/* Funcion que se encarga de obtener los tokens del campo genero.
**
*/
static char * lineToTokens(char * line, char c, size_t * pos){
    int i, newDim = 0;
    char * ans = NULL;
    for(i = *pos; line[i] != c && line[i] != 0 ; i++){
        if(newDim % BLOCK == 0){
            ans = realloc(ans, (newDim + BLOCK) * sizeof(char));
            checkMemory(ans);
        }
        ans[newDim++] = line[i];
    }
    if(ans != NULL){
        *pos = line[i] == c ? i+1 : i ;
        ans = realloc(ans, (newDim+1) * sizeof(char));
        checkMemory(ans);
        ans[newDim] = 0;
    }
    return ans;
}

/* Funcion que obtiene una copia de una linea del archivo .csv
** y devuelve la dimension de la misma.
*/
static char * getLine(FILE * text, char * dest, unsigned int * dim, int * flag) {
    char * ans = dest;
    unsigned int i = 0 , idx = *dim;
    int c;
    while( (c = fgetc(text)) != '\n' && c != EOF){
        if(i == idx){
            ans = realloc(ans, (i + BLOCK) * sizeof(char));
            //errno == ENOMEM (PREGUNTAR
            checkMemory(ans);
            idx += BLOCK;
        }
        ans[i++] = c;
    }
    if(*flag){
        ans = realloc(ans, (i+1) * sizeof(char));
        checkMemory(ans);
        ans[i] = 0;
        *dim = i;
        if(c == EOF){
            *flag = FALSE;
        }
    }else{
        free(dest);
        ans = NULL;
    }
    return ans;
}
/* Funcion que crea una copia del string recibido. Además verifica si hay memoria disponible
** para poder llevar a cabo esta acción.
*/
static char * copy(char * source){
    char * ans = malloc((strlen(source)+1) * sizeof(char));
    checkMemory(ans);
    strcpy(ans, source);
    return ans;
}
/* Funcion que copia los generos en el vector de punteros a char que se encuentra en la
** estructura.
*/
static void copy_genres(TContent * aux , char * line){
    aux->genre = NULL;
    size_t pos = 0;
    char * token = lineToTokens(line, ',', &pos);
    int i = 0;
    for(; token != NULL ; i++,token = lineToTokens(line, ',', &pos)){
        if((i % BLOCK) == 0) {
            aux->genre = realloc(aux->genre, (BLOCK + i) * sizeof(char *));
            checkMemGenre(aux,BLOCK+i);
        }
        aux->genre[i] = copy(token);
        free(token);
    }
    aux->genre = realloc(aux->genre, (i+1) * sizeof(char *));
    checkMemGenre(aux,i+1);
    aux->genre[i] = NULL;
}

//VERIFICAR EL /N QUE PUEDE APARECER PREGUNTAR.
//enum para sumar punticos.
//CANTIDAD DE GENEROS TENGO QUE QUITAR
/* Funcion que divide una linea del archivo .csv en tokens separados por el ';'.
** Además guarda cada token en su correspondiente campo presente en la estructura que
** almacena toda la información de la película/serie.
*/
static int getTokens(char * line, TContent * aux){
    //ESTO FUNCION PERO DEBEMOS PROBAR QUE PASA SI EN LOS
    //STRTOK DE ABAJO SE PONE NULL O EL MISMO LINE.
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
                if(aux->titleType == SER){
                    aux->genre = calloc(1,sizeof(char *));
                    checkMemGenre(aux,1);
                }
                else{
                    copy_genres(aux,token);
                }
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
//PASAR SOLO UNA ESTRUCTURA
//NO LIBERAR LOS CHAR * DE LA ESTRUCTURA
/* Funcion que guarda toda la informacion de peliculas/series presentes en el archivo
** llamado file para su posterior análisis.
*/
void readInput(int argQty, char * file[], imdbADT imdb){
    if(argQty != 2){ //Verifica si la cantidad de argumentos al ejecutar es correcta.
        fprintf(stderr, "CANTIDAD DE ARGUMENTOS INVALIDA!"); //Mensaje de error.
        exit(1); //Aborta
    }
    FILE * text = fopen(file[1], "r"); //Se accede a los datos del archivo.
    TContent * aux = malloc(sizeof(TContent)); //Creacion de un puntero a la estructura que almacenara toda la informacion de cada linea del archivo.
    checkMemory(aux); //Chequea memoria disponible.
    unsigned int dim = 0; //Variable que guarda la dimension de cada linea actual que se esta procesando.
    char * s = NULL; //Inicializacion del string que guardara la linea actual.
    int flag = TRUE;
    s = getLine(text,s,&dim,&flag); //Obtencion de la linea actual y de su correspondiente dimension
    /* Ciclo que obtiene la información del archivo en su totalidad (a menos que existan errores).
    ** Además le da la informacion al ADT para que este mismo la procese.
    */
    while((s = getLine(text,s,&dim,&flag)) != NULL){ //Se obtiene la linea actual.
        if(getTokens(s, aux)){ //Se obtienen los tokens de la linea actual.
            //asignar al adt SOLAMENTE en este caso
            if(add(imdb, aux) == ERR){
                freeImdb(imdb);
                fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
                exit(1);
            }
        }
    }
    free(s);
    free(aux->genre);
    free(aux);
    fclose(text);
}