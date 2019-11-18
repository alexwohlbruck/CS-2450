// Joel Swanson
// Programming Assignment 3 - Structures

#include <stdio.h>
#include <string.h>
#include "structures.h"

void getOneDVD(dvd_t *dvd) {
    
    char title[TEXT_SIZE];
    char director[TEXT_SIZE];
    int playTime;
    char newline;
    
    printf("Enter title: ");
    scanf("%80[^\n]s", title);
    scanf("%c", &newline);
    
    
    printf("Enter director: ");
    scanf("%80[^\n]s", director);
    scanf("%c", &newline);
    
    
    printf("Enter play time in minutes: ");
    scanf("%d", &playTime);
    scanf("%c", &newline);
    
    
    strncpy(dvd->title, title, TEXT_SIZE);
    strncpy(dvd->director, director, TEXT_SIZE);
    dvd->playTime = playTime;
}

void printOneDVD(dvd_t dvd) {
    printf("%s:%s:%d\n", dvd.title, dvd.director, dvd.playTime);
}

void getCollection(dvd_t dvds[], int count) {
    for (int i = 0; i < count; i++) {
        getOneDVD(&dvds[i]);
    }
}

void printCollection(dvd_t dvds[], int count) {
    for (int i = 0; i < count; i++) {
        printOneDVD(dvds[i]);
    }
}