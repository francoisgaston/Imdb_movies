//
// Created by Jose Menta on 06/07/2021.
//

#ifndef TPE_FINAL_PI_YEARSADT_H
#define TPE_FINAL_PI_YEARSADT_H
#include "structs.h"
typedef struct yearsCDT* yearsADT;
yearsADT newYearsADT(void);
int addYear(yearsADT years,const TContent* content);
void freeYearsADT(yearsADT year);
void toBeginYearsADT(yearsADT years);
int hasNextYearsADT(yearsADT years);
int hasNextGenresYearsADT(yearsADT years);
tYear nextYearADT(yearsADT years);
tGenres nextGenresYearsADT(yearsADT years);
void goToNextYearsADT(yearsADT years);
#endif //TPE_FINAL_PI_YEARSADT_H
