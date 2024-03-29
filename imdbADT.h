//
//
//

#ifndef TPE_FINAL_PI_IMDBADT_H
#define TPE_FINAL_PI_IMDBADT_H
#include "structs.h"
#include <stddef.h>
/*
 * imdbADT es un TAD que permite guardar la información de películas y series para luego poder obtener las 100 mejores películas
 *  con al menos cien mil votos y la información de cada año, donde se puede iterar descendentemente por ellos y obtener
 *  la película y serie con mayor cantidad de votos, la cantidad de series, la cantidad de películas y una lista donde se cuenta la cantidad de
 *  películas de cada género, a la cual se puede acceder mediante otro iterador.
 *  En el caso de haber un error reservando memoria, deja en el TAD los datos agregados hasta el momento (incluyendo por ejemplo un año
 *  sin la mejor película/serie) y devuelve ERR en add (constante definida en structs.h)
 */
typedef struct imdbCDT* imdbADT;
/*
 * Crea una nueva estructura vacía. En caso de no poder reservar memoria devuelve NULL.
 */
imdbADT newImdb(void);
/*
 * Agrega la informacion de una película o serie al TAD. Recibe un puntero a una estructura TContent
 * (ver nota sobre las estructuras de entrada/salida). Devuelve ERR si hay un error reservando memoria
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
 * Devuelve una estructura TYear con la información del año donde se encuentra el iterador. A diferencia de otras funciones del mismo tipo, no
 * se mueve el iterador al año siguiente para que el usuario pueda recorrer la lista de géneros de ese año con la función hasNextGenre y nextGenre.
 */
TYear nextYear(imdbADT imdb);
/*
 * Mueve el iterador de años al siguiente y en caso de existir ubica al iterador por géneros en el primer elemento de la lista de ese año.
 */
void goToNextYear(imdbADT imdb);
/*
 * Retorna 1 si hay un genero siguiente en la lista del año donde esta el iterador por años, 0 si no.
 */
int hasNextGenres(imdbADT imdb);
/*
 * Devuelve una estructura TGenre con la informacion del iterador por géneros de un año. Ubica al iterador en el siguiente género automáticamente.
 */
TGenre nextGenres(imdbADT);
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
 * Devuelve una estructura TRanking con la información del iterador.
 */
TRanking nextRanking(imdbADT imdb);

/*
 * Sobre las estructuras de entrada/salida:
 * Entrada:
 * El puntero a TContent debe ser administrado por el usuario, el TAD no lo modificará. Para ser almacenado junto con la informacion de cada año,
 * el campo startYear debe ser positivo, si no hay un valor en la base de datos debe ingresarse INVALID. El vector de char* con los géneros del
 * contenido debe terminar en NULL y ser administrado por el usuario.
 * Los char* de la estructura, tanto primaryTitle como cada género en genre deben estar en una zona de memoria dinámica y NO deben ser
 * modificados o liberados una vez que son enviados al TAD, ya que se guarda una copia del puntero. No se deben pasar los géneros si se trata de una serie,
 * se debe pasar un vector de char* cuyo unico elemento sea NULL.
 * El campo titleType debe completarse con las constantes MOV o SER en caso de ser una película o una serie. No se deben enviar al TAD datos con otro tipo.
 * El raitig debe ser un valor entre 0 y 10 con una posicion decimal, no se verificará en el TAD si se pasan valores incorrectos.
 * Salida:
 * TYear: startYear se completa con un entero positivo, nunca con INVALID. En caso de no existir en ese año una mejor película o serie,
 * se deja su primaryTitle en NULL y tanto numVotes como averageRaiting en 0. Los punteros que se envían en primaryTitle no deben ser modificados o liberados
 * por el usuario, ya que los mismos se utilizan en el backend.
 * TGenre: el campo genre se completa con un char* con el nombre del género, que no debe ser modificado o liberado por el usuario.
 * TRanking: El campo startYear tendrá el mismo valor que se pasó con la película cuando se cargó: un entero positivo o INVALID. PrimaryTitle
 * tendra un char* que no debe ser liberado o modificado. AverageRating tendrá el módulo y signo del dato que se cargó, no se verificará que
 * sea positivo.
 */
#endif //TPE_FINAL_PI_IMDBADT_H
