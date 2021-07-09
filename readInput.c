#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imdbADT.h"

enum content{PTITLE=1, SYEAR, GENRE = 4, ARATE, NVOT};

// Funcion que verifica si hay memoria disponible para guardar data.
static void checkMemory(void * text){
    if( text==NULL ||errno == ENOMEM){
        free(text);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
}
static int checkMem(void* mem, int* ffree){
    if(mem==NULL || errno==ENOMEM){
        free(mem);
        //fprintf(stderr, "NO HAY MEMORIA DISPONIBLE"); Esto creo que no deberia ir aca
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
        //free(aux); creo que no debería estar, poruqe seguro se libera antes de terminar la ejecución
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
            auxp = realloc(ans, (newDim + BLOCK) * sizeof(char));
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
        auxp = realloc(ans, (newDim+1) * sizeof(char));
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
            auxp = realloc(ans, (i + BLOCK) * sizeof(char));
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
    auxp = realloc(ans, (i+1) * sizeof(char));
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
    char * ans = malloc((strlen(source)+1) * sizeof(char));
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
                return; //la funcion termina y la que lo llama tiene el mensaje de terminar
            }
        }
        aux->genre[i] = token;
    }
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
static int getTokens(char * line, TContent * aux, int* ffree){
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
    while((token = strtok(NULL, ";")) != NULL && *ffree!=ERR){
        switch(++counter){
            case PTITLE:
                aux->primaryTitle = copy(token,ffree);
                //en este caso tambíen debería liberar todos lo que tiene la estructura para ser consistentes
                checkMemGenre(aux, aux,0,ffree); //si ffree es ERR, libera todo
                break;
            case SYEAR:
                aux->startYear = (!strcmp(token, "\\N")) ? INVALID : atoi(token);
                counter++;
                strtok(NULL, ";"); //Porque sino me quedo en el TOKEN de FIN DE ANIO
                break;  //agregar aca el INVALID
            case GENRE: //si falla el malloc del genero liberar los demas
                if(aux->titleType == SER){
                    aux->genre = calloc(1,sizeof(char *));
                    checkMemGenre(aux,aux->genre,1,ffree); //si se reservó bien, sigue. Si no se libera toda la estructura.
                }
                else{
                    copyGenres(aux, token,ffree); //si hubo error, libera toda la estructura
                }
                break;
            case ARATE:
            //mandar 0 si es \N
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
    int ffree=OK;
    while((s = getLine(text,s,&dim,&flag)) != NULL && ffree!=ERR) { //Se obtiene la linea actual.
        if (getTokens(s, aux, &ffree) && ffree != ERR) { //si ffree==ERR, ya se libero toda la estructura
            //Se obtienen los tokens de la linea actual.
            //asignar al adt SOLAMENTE en este caso
            if (add(imdb, aux) == ERR) {
                freeImdb(imdb);
                free(aux->genre);
                free(aux);
                free(s);
                fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
                exit(1);
            }
        }
        if (ffree!=ERR) { //si no, ya fue liberado, no deberia hacer doble free
            free(aux->genre); //solo se debe hacer si no se libero la estructura antes
        }
    }
    free(s);
    free(aux);
    if(ffree==ERR){//la estructura ya fue liberada
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE");
        exit(1);
    }
    fclose(text);
}