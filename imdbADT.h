//
// Created by Jose Menta on 05/07/2021.
//

#ifndef TPE_FINAL_PI_IMDBADT_H
#define TPE_FINAL_PI_IMDBADT_H
#include "structs.h"
#include <stddef.h>
/*
 * imdbADT es un TAD que permite guardar la información de películas y series para luego poder obtener las 100 mejores películas
 *  con al menos cien mil votos y la información de cada año, donde se puede iterar descendentemente por ellos y obtener
 *  la mejor película, la mejor serie, la cantidad de series, la cantidad de películas y una lista donde se cuenta la cantidad de
 *  películas de cada género
 */
typedef struct imdbCDT* imdbADT;
/*
 * Crea una nueva estructura vacía. En caso de no poder reservar memoria devuelve NULL.
 */
imdbADT newImdb(void);
/*
 * Agrega la informacion de una película o serie al TAD. Recibe un puntero a una estructura TContent que el usuario deberá liberar luego
 * en caso de ser necesario. El usuario no debe modificar o liberar los char* que se pasan con la estructura (que deben estar en el Heap),
 * ya que en el TAD se guarda una referencia a los mismos y no se copia nuevamente, por lo que el TAD se encarga de liberarlos en caso de ser necesario.
 */
int add(imdbADT imdb, const TContent* content);
/*
 * Libera todos los recursos almacendados en el TAD
 */
void freeImdb(imdbADT imdb);
/*
 * Inicializa el iterador que permite recorrer los años en orden descendente y los géneros del año.
 */
void toBeginYear(imdbADT imdb);
/*
 * Retorna 1 si hay un año siguiente, 0 si no.
 */
int hasNextYear(imdbADT imdb);
/*
 * Devuelve una estructura tYear con la información del año donde se encuentra el iterador. A diferencia de otras funciones del mismo tipo, no
 * se mueve el iterador al año siguiente para que el usuario pueda recorrer la lista de géneros de ese año.
 */
tYear nextYear(imdbADT imdb);
/*
 * Mueve el iterador de años al siguiente y en caso de existir ubica al iterador por géneros en el primer elemento de la lista de ese año.
 */
void goToNextYear(imdbADT imdb);
/*
 * Retorna 1 si hay un genero siguiente en la lista del año donde esta el iterador por años, 0 si no.
 */
int hasNextGenres(imdbADT imdb);
/*
 * Devuelve una estructura tGenres con la informacion del iterador por géneros de un año. Ubica al iterador en el siguiente género automáticamente. El usuario
 * no debe modificar el char* con el genero, ya que esa misma zona de memoria se utiliza en el TAD.
 */
tGenres nextGenres(imdbADT);
/*
 * Inicializa el iterador para recorrer las 100 mejores películas en orden descendente por calificación (ordenadas segun su cantidad de votos con
 * el mismo orden en caso de tener la misma calificación)
 */
void toBeginRanking(imdbADT imdb);
/*
 * Retorna 1 si hay una película siguiente en la lista, 0 si no.
 */
int hasNextRanking(imdbADT imdb);
/*
 * Devuelve una estructura tQ4 con la información del iterador. El usuario no debe modificar el char* con el título de la estructura, ya que esa misma
 * zona de memoria se utiliza en el TAD.
 */
tQ4 nextRanking(imdbADT imdb);

#endif //TPE_FINAL_PI_IMDBADT_H
