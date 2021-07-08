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
    char** genres= malloc(4*sizeof (char*));
    char* title= malloc(30);
    strcpy(title,"UNO");
    char* genre=malloc(20);
    strcpy(genre,"Accion");
    genres[0]=genre;
    genre=malloc(20);
    strcpy(genre,"Aventura");
    genres[1]=genre;//es de accion y aventuras
    genres[2]=NULL;
    TContent movie={MOV,title,2021,genres,8.0,120000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"DOS(mejor rating)");
    genre=malloc(20);
    strcpy(genre,"Accion");
    genres[0]=genre;
    genre=malloc(20);
    strcpy(genre,"Drama");
    genres[1]=genre;
    genres[2]=NULL;//es solo de accion
    movie=(TContent){MOV,title,2021,genres,9.2,130000};
    add(m,&movie);
    //el puntero a last deberia quedar en esta a medida que se agregan
    title= malloc(30);
    strcpy(title,"TRES(peor rating)");
    genre=malloc(20);
    strcpy(genre,"Accion");
    genres[0]=genre;
    genres[1]=NULL;
    movie=(TContent){MOV,title,2021,genres,4.2,110000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"ARRIBA DE UNO");
    genre=malloc(20);
    strcpy(genre,"Accion");
    genres[0]=genre;
    genres[1]=NULL;
    movie=(TContent){MOV,title,2021,genres,8.0,130000};
    add(m,&movie);
    title= malloc(30);
    strcpy(title,"UNO");
    genre=malloc(20);
    strcpy(genre,"Drama");
    genres[0]=genre;
    genre=malloc(20);
    strcpy(genre,"Accion");
    genres[1]=genre;
    genres[2]=NULL;
    movie=(TContent){MOV,title,2020,genres,9.2,130000};
    add(m,&movie);
    //series no deberian aparecer en el ranking
    title= malloc(30);
    strcpy(title,"UNO_SERIE");
    genre=malloc(20);
    strcpy(genre,"Drama");
    genres[0]=genre;
    genre=malloc(20);
    strcpy(genre,"No deberia aparecer");
    genres[1]=genre;
    genres[2]=NULL;
    movie=(TContent){SER,title,2021,genres,10.0,140000};
    add(m,&movie);
    free(genres[0]);
    free(genres[1]);
    title= malloc(30);
    strcpy(title,"DOS_SERIE");
    genre=malloc(20);
    strcpy(genre,"Accion");
    genres[0]=genre;
    genres[1]=NULL;
    movie=(TContent){SER,title,2021,genres,9.2,130000};
    add(m,&movie);
    free(genres[0]);
    free(genres);
    toBeginYear(m);
    //2021
    TYear ans= nextYear(m);
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
    TGenre gen= nextGenres(m);
    assert(strcmp(gen.genre,"Accion")==0);
    assert(gen.cant==4);//solo se cuentan las películas
    assert(hasNextGenres(m)==1);//no deberia guardarse lo de las series
    gen= nextGenres(m);
    assert(strcmp(gen.genre,"Aventura")==0);
    assert(gen.cant==1);//solo se cuentan las películas
    assert(hasNextGenres(m)==1);
    gen= nextGenres(m);
    assert(strcmp(gen.genre,"Drama")==0);
    assert(gen.cant==1);//solo se cuentan las películas
    assert(hasNextGenres(m)==0);
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
    assert(strcmp(gen.genre,"Accion")==0);
    assert(gen.cant==1);
    assert(hasNextGenres(m)==1);
    gen= nextGenres(m);
    assert(strcmp(gen.genre,"Drama")==0);
    assert(gen.cant==1);
    assert(hasNextGenres(m)==0);
    goToNextYear(m);
    assert(hasNextYear(m)==0);
    toBeginRanking(m);
    //estoy liberando algo antes de tiempo, revisar
    while (hasNextRanking(m)){
        TRanking ansR= nextRanking(m);
        printf("año: %d, titulo: %s, cantVotos: %ld, rating: %f\n",ansR.startYear,ansR.primaryTitle,ansR.numVotes,ansR.averageRating);
    }
    assert(hasNextRanking(m)==0);
    freeImdb(m);
    puts("OK!");


}
