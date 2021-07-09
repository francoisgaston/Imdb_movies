//
//
//

#ifndef TPE_FINAL_PI_YEARSADT_H
#define TPE_FINAL_PI_YEARSADT_H
#include "structs.h"
/*
 * yearsADT es una estructura que almacena películas y series en orden descendente por año, donde para cada uno guarda
 * la película y serie con mayor cantidad de votos, la cantidad de películas, la cantidad de series y una lista con la
 * cantidad de películas de cada género.
 */
typedef struct yearsCDT* yearsADT;
/*
 * crea una nueva estrucura vacía.
*/
yearsADT newYearsADT(void);
/*
 * Agrega la información de una película o serie al TAD. El usuario debe pasar la información como un puntero a una estructura
 * TContent. (ver imdbADT.h sobre como pasar los datos en la estructura).
 */
int addYear(yearsADT years,const TContent* content);
/*
 * Libera todos los recursos utilizados por la estructura.
 */
void freeYearsADT(yearsADT year);
/*
 * Inicializa un iterador para recorrer la lista de todos los años en orden descendente y otro iterador para recorrer la
 * lista de géneros de ese año en orden alfabético.
 */
void toBeginYearsADT(yearsADT years);
/*
 * Retorna 1 si hay un año siguiente, 0 si no.
 */
int hasNextYearsADT(yearsADT years);
/*
 * Retorna 1 si hay un género siguiente en la lista de géneros para el año donde esta el iterador por años, 0 si no
 * hay más géneros.
 */
int hasNextGenresYearsADT(yearsADT years);
/*
 * Devuelve una estructura de tipo TYear, donde la información será la del iterador por años.
 * A diferencia de otras funciones similares,esta funcion no mueve el iterador al siguiente elemento, para que el usuario
 * pueda iterar por los elementos de la lista de géneros de ese año.
 */
TYear nextYearADT(yearsADT years);
/*
 * Devuelve una estructura TGenre con la información del nodo de la lista de géneros del año donde se está iterando. Esta funcion
 * mueve el iterador por géneros al siguiente elemento luego de copiar los datos.
 */
TGenre nextGenresYearsADT(yearsADT years);
/*
 * Avanza el iterador por años al siguiente. En caso de existir, tambien deja al iterador por géneros en la lista de géneros del mismo.
 */
void goToNextYearsADT(yearsADT years);
#endif //TPE_FINAL_PI_YEARSADT_H
