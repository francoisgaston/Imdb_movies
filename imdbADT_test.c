//
// Created by Jose Menta on 06/07/2021.
//
#include "imdbADT.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    imdbADT m=newImdb();
    char* title= malloc(30);
    strcpy(title,"UNO");
    char* genre=malloc(20);
    strcpy(genre,"Accion");
    TContent movie={MOV,title,2021,genre,8.0,120000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"DOS(mejor rating)");
    genre=malloc(20);
    strcpy(genre,"Accion");
    movie=(TContent){MOV,title,2021,genre,9.2,130000};
    add(m,&movie);
    //el puntero a last deberia quedar en esta a medida que se agregan
    title= malloc(30);
    strcpy(title,"TRES(peor rating)");
    genre=malloc(20);
    strcpy(genre,"Accion");
    movie=(TContent){MOV,title,2021,genre,4.2,110000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"ARRIBA DE UNO");
    genre=malloc(20);
    strcpy(genre,"Accion");
    movie=(TContent){MOV,title,2021,genre,8.0,130000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"UNO");
    genre=malloc(20);
    strcpy(genre,"Drama");
    movie=(TContent){MOV,title,2020,genre,9.2,130000};
    add(m,&movie);
    //series no deberian aparecer en el ranking
    title= malloc(30);
    strcpy(title,"UNO_SERIE");
    genre=malloc(20);
    strcpy(genre,"Drama");
    movie=(TContent){SER,title,2021,genre,10.0,140000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"DOS_SERIE");
    genre=malloc(20);
    strcpy(genre,"Accion");
    movie=(TContent){SER,title,2021,genre,9.2,130000};
    add(m,&movie);
    toBeginYear(m);
    //2021
    tYear ans= nextYear(m);
    assert(ans.startYear==2021);
    assert(strcmp(ans.primaryTitleMovie,"DOS(mejor rating)")==0);
    assert(strcmp(ans.primaryTitleSerie,"UNO_SERIE")==0);
    assert(ans.numVotesMovie==130000);
    assert(ans.numVotesSerie==140000);
    assert(ans.dimMovie==4);
    assert(ans.dimSerie==2);
    printf("Rating movie: %f=9.2\n",ans.averageRatingMovie);
    printf("Rating serie: %f=10.0\n",ans.averageRatingSerie);
    assert(hasNextGenres(m)==1);
    tGenres gen= nextGenres(m);
    assert(strcmp(gen.genres,"Accion")==0);
    assert(gen.cant==4);//solo se cuentan las películas
    assert(hasNextGenres(m)==0);//no deberia guardarse lo de las series
    goToNextYear(m);
    //2020
    assert(hasNextYear(m)==1);
    ans= nextYear(m);
    assert(ans.dimMovie==1);
    assert(ans.dimSerie==0);
    assert(strcmp(ans.primaryTitleMovie,"UNO")==0);
    assert(ans.primaryTitleSerie==NULL);
    assert(hasNextGenres(m)==1);
    gen= nextGenres(m);
    assert(strcmp(gen.genres,"Drama")==0);
    assert(gen.cant==1);
    assert(hasNextGenres(m)==0);
    goToNextYear(m);
    assert(hasNextYear(m)==0);
    toBeginRanking(m);
    //estoy liberando algo antes de tiempo, revisar
    while (hasNextRanking(m)){
        tQ4 ansR= nextRanking(m);
        printf("año: %d, titulo: %s, cantVotos: %d, rating: %f\n",ansR.startYear,ansR.primaryTitle,ansR.numVotes,ansR.averageRating);
    }
    assert(hasNextRanking(m)==0);
    freeImdb(m);
    puts("OK!");


}
