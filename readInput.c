#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imdbADT.h"

#define CONTENTS 7

static int checkMem(void* mem, int* ffree){
    if(mem==NULL || errno==ENOMEM){
        free(mem);
        //fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n"); Esto creo que no deberia ir aca
        *ffree=ERR;
        return 0;
    }
    return 1;
}
/* Funcion que verifica memoria disponible para el char ** genre.
** Además libera la estructura antes de abortar.
*/
static int checkMemGenre(TContent * aux, void* auxp, int idx, int* ffree){
    if( auxp==NULL || errno == ENOMEM || *ffree==ERR) {
        for (int i = 0; i < idx; i++)
            free(aux->genre[i]);
        free(aux->genre);
        free(aux->primaryTitle);
        *ffree=ERR;
        return 0;
    }
    return 1;
}
/* Funcion que se encarga de obtener los tokens del campo genero.
**
*/
//funcion con sugerencias para el caso donde se queda sin memoria
static char * lineToTokens(const char * line, char c, int * pos, int* ffree){//despues
    int i, newDim = 0;
    char * ans = NULL;
    char* auxp;
    for(i = *pos; line[i] != c && line[i] != 0 && *ffree!=ERR ; i++){
        if(newDim % BLOCK == 0){
        //creo que deberiamos hacer algo asi en todos los realloc
            auxp = realloc(ans, (newDim + BLOCK));
            if(!checkMem(auxp,ffree)) {
                free(ans);//se libera lo que tenia hasta el momento para no tener leaks
                return NULL;
            }
            ans=auxp;
        }
        ans[newDim++] = line[i];
    }
    if(ans != NULL){
        *pos = line[i] == c ? i+1 : i ;
        auxp = realloc(ans, (newDim+1));
        if(!checkMem(auxp,ffree)){
            free(ans);
            return NULL;
        }
        ans=auxp;//pudo reservar bien la memoria
        ans[newDim] = 0;
    }
    return ans;
}

/* Funcion que obtiene una copia de una linea del archivo .csv
** y devuelve la dimension de la misma.
*/
static char * getLine(FILE * text, char * dest, unsigned int * dim, int * ffree) {
    char * ans = dest;
    unsigned int i = 0 , idx = *dim;
    char* auxp;
    int c= fgetc(text);
    if(c==EOF){
        return NULL;
    }
    while ( c != '\n' && c != EOF) {
        if (i == idx) {
            auxp = realloc(ans, (i + BLOCK));
            if(!checkMem(auxp,ffree)){
                return ans;//lo que tenía ans se libera en la funcion que lo llama
            }
            ans=auxp;//se amplió bien la memoria
            idx += BLOCK;
        }
        ans[i++] = c;
        c= fgetc(text);
    }
    //si hay \n despues de la ultima linea con informacion de contenido
    auxp = realloc(ans, (i+1));
    if(!checkMem(auxp,ffree)){
        return ans;
    }
    ans=auxp; //se aplió correctamente
    ans[i] = 0;
    *dim = i;
    return ans;
}
/* Funcion que crea una copia del string recibido. Además verifica si hay memoria disponible
** para poder llevar a cabo esta acción.
*/
static char * copy(char * source,int* ffree){
    char * ans = malloc(strlen(source)+1);
    if(checkMem(ans,ffree)){
        strcpy(ans, source);
        return ans;
    }
    return NULL;
}
/* Funcion que copia los generos en el vector de punteros a char que se encuentra en la
** estructura.
*/
static void copyGenres(TContent * aux , char * line, int* ffree){//revisar
    aux->genre = NULL;
    int pos = 0;
    char * token = lineToTokens(line, ',', &pos,ffree);
    int i = 0;
    char** auxp;
    for(; token != NULL && *ffree!=ERR ; i++,token = lineToTokens(line, ',', &pos,ffree)){
        if((i % BLOCK) == 0) {
            auxp = realloc(aux->genre, (BLOCK + i) * sizeof(char *));
            if(checkMemGenre(aux,auxp,i,ffree)){
                aux->genre=auxp; //recien ahi lo asigno, si no libero todos los campos
            }else{
                free(token);
                return; //la funcion termina y la que lo llama tiene el mensaje de terminar
            }
        }
        aux->genre[i] = token;
    }
    if(*ffree == ERR)//no se pudo reservar memoria para el/los generos
        return;
    auxp = realloc(aux->genre, (i+1) * sizeof(char *));
    if(checkMemGenre(aux,auxp,i,ffree)){
        aux->genre=auxp; //recien ahi lo asigno, si no libero todos los campos
    }else{
        return; //la funcion termina y la que lo llama tiene el mensaje de terminar
    }
    aux->genre[i] = NULL;
}

//VERIFICAR EL /N QUE PUEDE APARECER PREGUNTAR.
//enum para sumar punticos.
//CANTIDAD DE GENEROS TENGO QUE QUITAR
/* Funcion que divide una linea del archivo .csv en tokens separados por el ';'.
** Además guarda cada token en su correspondiente campo presente en la estructura que
** almacena toda la información de la película/serie.
*/

static void fPtitle (char * token, TContent * aux, int * ffree){
    aux->primaryTitle = copy(token,ffree);
    checkMemGenre(aux, aux->primaryTitle,0,ffree);
}
static void fYear(char * token, TContent * aux, int * ffree){
    aux->startYear = (!strcmp(token, "\\N")) ? INVALID : atoi(token);
}

static void pass(char * token, TContent * aux, int * ffree){
	return;
}

static void fGenre(char * token, TContent * aux, int * ffree){
    if(aux->titleType == SER || !strcmp(token,"\\N")){
        aux->genre = calloc(1,sizeof(char *));
        checkMemGenre(aux,aux->genre,1,ffree); //si se reservó bien, sigue. Si no se libera toda la estructura.
    }
    else{
        copyGenres(aux, token,ffree); //si hubo error, libera toda la estructura
    }
}

static void fRate(char * token, TContent * aux, int * ffree){
    aux->averageRating = (!strcmp(token, "\\N")) ? 0.0 : atof(token);
}

static void fVot(char * token, TContent * aux, int * ffree){
    aux->numVotes = (!strcmp(token, "\\N")) ? 0 : atoi(token);
}


static int getTokens(char * line, TContent * aux, int* ffree){
    void (*procesadores[CONTENTS]) (char *, TContent *, int *)={fPtitle, fYear, pass, fGenre, fRate, fVot, pass};
    char * token = strtok(line, ";");
    if(!strcmp(token, "movie"))
    	aux->titleType = MOV;
    else if(!strcmp(token, "tvSeries"))
    	aux->titleType = SER;
    else
        return FALSE; 
    for(int i=0; (token = strtok(NULL,";"))!=NULL && *ffree!=ERR; i++)
            procesadores[i] (token, aux, ffree);
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
        fprintf(stderr, "CANTIDAD DE ARGUMENTOS INVALIDA!\n");
        exit(1);
    }
    FILE * text = fopen(file[1], "r"); //Se accede a los datos del archivo.
    TContent * aux = malloc(sizeof(TContent)); //Creacion de un puntero a la estructura que almacenara toda la informacion de cada linea del archivo.
    aux->genre = NULL;
    int ffree=OK;
    if( !checkMem(aux,&ffree) ){//Chequea memoria disponible.
        freeImdb(imdb);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n");
        exit(1);
    } 
    unsigned int dim = 0; //Variable que guarda la dimension de cada linea actual que se esta procesando.
    char * s = NULL; //Inicializacion del string que guardara la linea actual.
    s = getLine(text,s,&dim,&ffree); //Obtencion de la linea actual y de su correspondiente dimension
    /* Ciclo que obtiene la información del archivo en su totalidad (a menos que existan errores).
    ** Además le da la informacion al ADT para que este mismo la procese.
    */
    if(ffree != ERR){//si no se pudo obtener la primera linea, liberara la memoria en la siguiente condicion porque ffree sera igual a ERR
        while((s = getLine(text,s,&dim,&ffree)) != NULL && ffree!=ERR) { //Se obtiene la linea actual.
            if (getTokens(s, aux, &ffree) && ffree != ERR) { //si ffree==ERR, ya se libero toda la estructura
                //Se obtienen los tokens de la linea actual.
                //asignar al adt SOLAMENTE en este caso
                if (add(imdb, aux) == ERR) {
                    freeImdb(imdb);
                    free(aux->genre);
                    free(aux);
                    free(s);
                    fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n");
                    exit(1);
                }
                free(aux->genre);
            }
        }
    }
    free(s);
    free(aux);
    if(ffree==ERR){//la estructura ya fue liberada
        freeImdb(imdb);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n");
        exit(1);
    }
    fclose(text);
}
