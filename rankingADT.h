//
// Created by Jose Menta on 06/07/2021.
//

#ifndef TPE_FINAL_PI_RANKINGADT_H
#define TPE_FINAL_PI_RANKINGADT_H
#include "structs.h"
typedef struct rankingCDT* rankingADT;
rankingADT newRankingADT();
void freeRankingADT(rankingADT ranking);
int addRanking(rankingADT ranking, const TContent * content);
void toBeginRankingADT(rankingADT ranking);
int hasNextRankingADT(rankingADT ranking);
tQ4 nextRankingADT(rankingADT ranking);

#endif //TPE_FINAL_PI_RANKINGADT_H
