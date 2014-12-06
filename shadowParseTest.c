//
//  main.c
//  shadow_parse
//
//  Created by Richard Lancia on 12/4/14.
//  Copyright (c) 2014 Richard Lancia. All rights reserved.
//
//test

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define shadowFile "/Users/rich/Desktop/shadow_example.txt"



char* createBigArray(FILE *fp){
    char* bigArray;
    fp = fopen(shadowFile, "rw");
    fgets(bigArray, sizeof(bigArray), fp);
    return bigArray;
}


typedef struct shadowDataNode{
    char *user;
    char hash[50];
    int numDays;
    int daysCanChange;
    int daysMustChange;
    int daysWarn;
    struct shadowDataNode *next, *prev;
    
}shadowDataNode;

typedef struct tempUser{
    char userinfo[100];
}tempUser;

tempUser* create(){
    FILE *fp;
    tempUser *users = malloc(35*sizeof(tempUser));
    char tempArray[100];
    int i;
    fp = fopen(shadowFile, "rw");
    
    for (i = 0; i<33; i++) {
        fgets(tempArray, 100, fp);
        strcpy(users[i].userinfo, tempArray);
        fseek(fp, 0, SEEK_CUR);
    }

    return users;
}

int main(int argc, const char * argv[]){
    
    
    
    return 0;
}
