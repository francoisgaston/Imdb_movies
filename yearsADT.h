//
// Created by Jose Menta on 06/07/2021.
//

#ifndef TPE_FINAL_PI_YEARSADT_H
#define TPE_FINAL_PI_YEARSADT_H
#include "structs.h"
/*
 * yearsADT es una estructura que almacena películas y series en orden descendente por año, donde para cada uno guarda
 * la mejor película, la mejor serie, la cantidad de películas, la cantidad de series y una lista donde
 * se gurada la cantidad de películas de cada género.
 */
typedef struct yearsCDT* yearsADT;
/*
 * crea una nueva estrucura vacía
*/
yearsADT newYearsADT(void);
/*
 * Agrega la información de una película o serie al TAD. El usuario debe pasar la información como un puntero a una estructura
 * TContent, que deberá liberar si es necesario luego de llamar a la funcion. (en este caso, el usuaro debería liberar tambien los
 * char* que envía en la estructura, pero como imdbADT los usa luego y se encarga de mantenerlos, se pide que no se liberen)
 */
int addYear(yearsADT years,const TContent* content);
/*
 * Libera todos los recursos utilizados por la estructura
 */
void freeYearsADT(yearsADT year);
/*
 * Inicializa un iterador para recorrer la lista de todos los años en orden descendente y otro iterador para recorrer la
 * lista de géneros de ese año en orden alfabético
 */
void toBeginYearsADT(yearsADT years);
/*
 * Retorna 1 si hay un año siguiente, 0 si no
 */
int hasNextYearsADT(yearsADT years);
/*
 * Retorna 1 si hay un género siguiente en la lista de géneros para el año donde esta el iterador por años, 0 si no
 * hay más géneros.
 */
int hasNextGenresYearsADT(yearsADT years);
/*
 * Devuelve una estructura de tipo tYear, donde la información será la del iterador por años. El usuario no debe modificar los char* que
 * devuelve la fucnion en la estructura, ya que unicamente se copian los que estan almacenados para ahorrar memoria.
 * A diferencia de otras funciones similares,esta funcion no mueve el iterador al siguiente elemento, ya que quizás el usuario
 * quiere iterar por los elementos de la lista de géneros de ese año.
 */
tYear nextYearADT(yearsADT years);
/*
 * Devuelve una estructura tGenres con la información del nodo de la lista de géneros del año donde se está iterando. Esta funcion
 * mueve el iterador por géneros al siguiente elemento luego de copiar los datos. Nuevamente, el usuario no debe modificar los char*
 * que devuelve en la estructura porque son los que usa el backend.
 */
tGenres nextGenresYearsADT(yearsADT years);
/*
 * Avanza el iterador por años al siguiente. En caso de existir, tambien deja al iterador por géneros en la lista de géneros del mismo.
 */
void goToNextYearsADT(yearsADT years);
#endif //TPE_FINAL_PI_YEARSADT_H
