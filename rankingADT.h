//
// Created by Jose Menta on 06/07/2021.
//

#ifndef TPE_FINAL_PI_RANKINGADT_H
#define TPE_FINAL_PI_RANKINGADT_H
#include "structs.h"
/*
 * rankingADT es un TAD que permite guardar las 100 mejores películas ordenadas descendentemente segun su calificación y su cantidad de votos
 * en caso de tener la misma calificacion.
 */
typedef struct rankingCDT* rankingADT;
/*
 * Crea una nueva estructura vacía
 */
rankingADT newRankingADT();
/*
 * Libera todos los recursos utilizados por la estructura
 */
void freeRankingADT(rankingADT ranking);
/*
 * Agrega una película al ranking en el caso de estar dentro de las mejores 100.
 * Recibe un puntero a una estructura TContent de la cual solo utiliza el título, en año, la cantidad de votos y el raiting. El usuario debe
 * liberar la estructura en caso de ser necesario, pero no el char* con el título, ya que en el TAD se almacena la direccion en la estructura,
 * no se copia el string.
 */
int addRanking(rankingADT ranking, const TContent * content);
/*
 * Inicializa el iterador para recorrer a las 100 mejores películas con el orden mencionado.
 */
void toBeginRankingADT(rankingADT ranking);
/*
 * Retorna 1 si hay una película sigiente en el ranking, 0 si no.
 */
int hasNextRankingADT(rankingADT ranking);
/*
 * Devuelve una estructura tQ4 con la informacion del iterador en el ranking. El ususario no debe modificar el char* con el título en la estructura,
 * ya que el mismo se utiliza en el TAD. Avanza al iterador al siguiente elemento automáticamente.
 */
tQ4 nextRankingADT(rankingADT ranking);

#endif //TPE_FINAL_PI_RANKINGADT_H
