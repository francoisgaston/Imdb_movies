#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imdbADT.h"

/* Funcion que chequea si hay memoria disponible luego del llamado de una funcion de la familia alloc. */
static int checkMem(void * mem, int * ffree){
    /* Si el void * ingresado es igual a NULL o errno es igual a ENOMEM quiere decir que no hay mas memoria
    ** disponible en el heap y debo liberar el mem y encender el flag con el fin de liberar toda la memoria
    ** reservada previamente antes de abortar.
    */
    if(mem == NULL || errno == ENOMEM){ 
        free(mem);
        *ffree=ERR;
        return 0;
    }
    return 1;
}
/* Funcion que verifica memoria disponible para el char ** genre.
** Además libera la estructura antes de abortar.
*/
static int checkMemGenre(TContent * aux, void * auxp, int idx, int * ffree){
    /* Si el void * auxiliar es igual a NULL o si el errno es igual a ENOMEM o el el flag de free es igual a ERR
    ** entonces no se pudo reservar memoria en el heap y por ende debo abortar. Sin embargo, previo a esto debo
    ** liberar toda la memoria que se reservo hasta ese momento.
    */
    if( auxp == NULL || errno == ENOMEM || *ffree == ERR) {
        for (int i = 0; i < idx; i++)
            free(aux->genre[i]);
        free(aux->genre);
        free(aux->primaryTitle);
        *ffree=ERR;
        return 0;
    }
    return 1;
}
/* Funcion que se encarga de obtener un genero del token que contiene a todos los generos.
** Recibe el token que posee todos los generos del contenido audiovisual, el caracter que separa
** los generos en el token de generos, un parametro de entrada/salida en donde se guarda la posicion
** en donde inicia el genero y un flag free que avisa si hay errores al querer reservar memeoria en el heap.
*/
static char * lineToTokens(const char * line, char c, int * pos, int* ffree){
    int i, newDim = 0; 
    char * ans = NULL; // String donde se guardara el genero del token de generos.
    char * auxp; // Puntero a char auxiliar para saber si hay memoria disponible en el heap.
    /* Ciclo que recorre el string donde estan todos los genero hasta que se encuentra con un '/0',
    ** o el caracter que separa los generos o si no se pudo reservar memoria en el heap.
    */
    for(i = *pos; line[i] != c && line[i] != 0 && *ffree!=ERR ; i++){
        if(newDim % BLOCK == 0){ // Si ya ocupe todos los lugares reservados agrando el string dinamico.
            auxp = realloc(ans, (newDim + BLOCK)); // Reservo memoria en el puntero auxiliar
            /* Verifico que se pudo reservar la memoria en el heap. Si no se pudo debo retorno NULL. */
            if(!checkMem(auxp,ffree)) {
                free(ans);
                return NULL;
            }
            ans = auxp; // Si se pudo reservar memoria entonces igualo el auxiliar al string ans.
        }
        ans[newDim++] = line[i];
    }
    /* Si ans es distinto de NULL entonces debo actualizar el pos para que sepa en donde iniciar el recorrido
    ** si existe otro genero. Achico el string para que quede en la longitud justa pero con el auxiliar para
    ** verificar que haya memoria disponible. Si hay, entonces lo igualo a ans y coloco el '\0' en ans.
    */
    if(ans != NULL){
        *pos = line[i] == c ? i+1 : i ;
        auxp = realloc(ans, (newDim+1));
        if(!checkMem(auxp,ffree)){
            free(ans);
            return NULL;
        }
        ans = auxp;
        ans[newDim] = 0;
    }
    return ans;
}

/* Funcion que obtiene una copia de una linea del archivo .csv y devuelve la dimension de la misma.
** Se tiene de parametros al archivo que contiene la informacion de los contenidos audiovisuales, el string
** que se reutilizara para almacenar la informacion de la nueva linea y que previamente contenia la data de la 
** linea leida previamente. Ademas se tiene dos parametro de entrada/salida, el primero almacena la dimension de la
** linea leida previamente y el segundo es aquel que indica que hubo problemas al querer almacenar memoria en el heap.
*/
static char * getLine(FILE * text, char * dest, unsigned int * dim, int * ffree) {
    char * ans = dest;
    unsigned int i = 0 , idx = *dim;
    char * auxp;
    int c = fgetc(text);
    if(c == EOF){ // Si se ingresa y el primer caracter es EOF entonces debo retornar NULL.
        return NULL;
    }
    /* Ciclo que recorre la linea del archivo .csv hasta que se encuentra con un salto de linea o EOF. */
    while ( c != '\n' && c != EOF) {
        if (i == idx) {
            /* Reserva de memoria en el heap para guardar la linea actual del archivo en un puntero auxiliar
            ** con el fin de chequear si hay memoria disponible. En caso de que haya se iguala el auxiliar al
            ** que contendra la linea.
            */
            auxp = realloc(ans, (i + BLOCK));
            if(!checkMem(auxp,ffree)){
                return ans;
            }
            ans = auxp;
            idx += BLOCK;
        }
        ans[i++] = c;
        c = fgetc(text);
    }
    /* Achico el string a la longitud justo, utilizando la misma metodologia de lineas anteriores. */
    auxp = realloc(ans, (i+1));
    if(!checkMem(auxp,ffree)){
        return ans; // Si no hay memoria disponible.
    }
    /* Si hay memoria disponible completo el string con '\0' y igualo el valor del puntero a la cantidad de espacios
    ** reservados para que la siguiente vez que se ingrese a esta funcion se pueda reutilizar la memoria reservada previamente.
    */
    ans = auxp;
    ans[i] = 0;
    *dim = i;
    return ans;
}
/* Funcion que crea una copia del string recibido. Además verifica si hay memoria disponible en el heap.
** Si hay, realizo la copia, si no hay entonces retorno NULL.
*/
static char * copy(char * source,int* ffree){
    char * ans = malloc(strlen(source)+1);
    if(checkMem(ans,ffree)){
        strcpy(ans, source);
        return ans;
    }
    return NULL;
}
/* Funcion que copia los generos en el vector de punteros a char que se encuentra en la estructura.
** Recibe el puntero a la estructura que contiene la descripcion del contenido audiovisual, el token que contiene
** a todos los generos a los que pertenece el contenido audiovisual y el flag que indica que hubo problemas 
** al querer almacenar memoria en el heap.
*/
static void copyGenres(TContent * aux , char * line, int* ffree){//revisar
    aux->genre = NULL;
    int pos = 0; // Variable que guarda la posicion en la que me encuentro en el string donde estan todos los generos.
    char * token = lineToTokens(line, ',', &pos, ffree); // Obtengo el primer genero.
    int i = 0;
    char ** auxp; // Auxiliar que permite verificar si hay memoria disponible.
    /* Ciclo que recorre todo el vector que almacena todos los generos. */
    for(; token != NULL && *ffree!=ERR ; i++, token = lineToTokens(line, ',', &pos,ffree)){
        /* Si me quedo sin espacio reservado, agrando el vector y verifico memoria. */
        if((i % BLOCK) == 0) {
            auxp = realloc(aux->genre, (BLOCK + i) * sizeof(char *));
            if(checkMemGenre(aux,auxp,i,ffree)){
                aux->genre=auxp;
            }else{
                free(token);
                return;
            }
        }
        aux->genre[i] = token;
    }
    if(*ffree == ERR) // No se pudo reservar memoria para el/los generos
        return;
    /* Achico el vector a la longitud justo, utilizando la misma metodologia de lineas anteriores. */
    auxp = realloc(aux->genre, (i+1) * sizeof(char *));
    if(checkMemGenre(aux, auxp, i, ffree)){
        aux->genre = auxp;
    }else{
        return;
    }
    /* El ultimo elemento debe ser NULL. */
    aux->genre[i] = NULL;
}

/* Funcion que guarda el titulo del contenido audiovisual. */
static void fPtitle (char * token, TContent * aux, int * ffree){
    aux->primaryTitle = copy(token,ffree);
    checkMemGenre(aux, aux->primaryTitle,0,ffree);
}
/* Funcion que guarda el anio de inicio del contenido audiovisual. */
static void fYear(char * token, TContent * aux, int * ffree){
    aux->startYear = (!strcmp(token, "\\N")) ? INVALID : atoi(token);
}
/* Funcion que omite la informacion del contenido audiovisual. */
static void pass(char * token, TContent * aux, int * ffree){
	return;
}
/* Funcion que guarda los generos que representan al contenido audiovisual. */
static void fGenre(char * token, TContent * aux, int * ffree){
    /* Si es una serie o el genero es "\N" entonces debo guardar un vector con un unico elemento
    ** que es NULL. Ademas se verifica si hay memoria suficiente para llevar a cabo esta accion.
    */
    if(aux->titleType == SER || !strcmp(token,"\\N")){
        aux->genre = calloc(1,sizeof(char *));
        checkMemGenre(aux, aux->genre, 1, ffree);
    }
    /* En caso de ser movie, se obtienen todos los generos mediante la funcion copyGenres. */
    else{
        copyGenres(aux, token, ffree);
    }
}
/* Funcion que guarda el rating del contenido audiovisual. */
static void fRate(char * token, TContent * aux, int * ffree){
    aux->averageRating = (!strcmp(token, "\\N")) ? 0.0 : atof(token);
}
/* Funcion que guarda la cantidad de votos del contenido audiovisual. */
static void fVot(char * token, TContent * aux, int * ffree){
    aux->numVotes = (!strcmp(token, "\\N")) ? 0 : atoi(token);
}
/* Funcion que divide una linea del archivo .csv en tokens separados por el ';'.
** Además guarda cada token en su correspondiente campo presente en la estructura que
** almacena toda la información de la película/serie.
*/
static int getTokens(char * line, TContent * aux, int* ffree){
    /* Vector de funciones que se aplicaran a las diferentes campos que describen el contenido audiovisual.*/
    void (*procesadores[]) (char *, TContent *, int *)={fPtitle, fYear, pass, fGenre, fRate, fVot, pass};
    /* String que guarda el primer token. Si es "movie" o "tvSeries" guardo la info en la estructura. Sino
    ** retorno FALSE.
    */
    char * token = strtok(line, ";");
    if(!strcmp(token, "movie"))
    	aux->titleType = MOV;
    else if(!strcmp(token, "tvSeries"))
    	aux->titleType = SER;
    else
        return FALSE;
    /* Ciclo que recorre todos los tokens que aparecen en la linea que describe el contenido audiovisual. */
    for(int i=0; (token = strtok(NULL,";"))!=NULL && *ffree!=ERR; i++)
            procesadores[i] (token, aux, ffree);
    return TRUE;
}
/* Funcion que guarda toda la informacion de peliculas/series presentes en el archivo
** llamado file para su posterior analisis.
*/
void readInput(char * file[], imdbADT imdb){
    FILE * text = fopen(file[1], "r"); //Se accede a los datos del archivo.
    TContent * aux = malloc(sizeof(TContent)); //Creacion de un puntero a la estructura que almacenara toda la informacion de cada linea del archivo.
    aux->genre = NULL;
    int ffree = OK;
    /* Verifica que haya memoria disponible en el heap. */
    if( !checkMem(aux, &ffree) ){
        freeImdb(imdb);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n");
        exit(1);
    } 
    unsigned int dim = 0; //Variable que guarda la dimension de cada linea actual que se esta procesando.
    char * s = NULL; //Inicializacion del string que guardara la linea actual.
    s = getLine(text, s, &dim, &ffree); //Obtencion de la linea actual y de su correspondiente dimension
    /* Ciclo que obtiene la información del archivo en su totalidad (a menos que existan errores).
    ** Además le da la informacion al ADT para que este mismo la procese.
    */
    if(ffree != ERR){// Si no se pudo obtener la primera linea, liberara la memoria en la siguiente condicion porque ffree sera igual a ERR
        while((s = getLine(text, s, &dim, &ffree)) != NULL && ffree!=ERR) { //Se obtiene la linea actual.
            if (getTokens(s, aux, &ffree) && ffree != ERR) { //Si ffree==ERR, ya se libero toda la estructura
                //Se obtienen los tokens de la linea actual que describe a un contenido audiovisual.
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
    /* Si no hay memoria disponible en el heap debo liberar la memoria reservada para imdbADT y abortar. */
    if(ffree == ERR){
        freeImdb(imdb);
        fprintf(stderr, "NO HAY MEMORIA DISPONIBLE\n");
        exit(1);
    }
    fclose(text);
}

