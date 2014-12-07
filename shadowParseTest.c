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
#include "list.h"
#define shadowFile "/Users/rich/Desktop/shadow_example.txt"


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
    tempUser *myUsers = malloc(33*sizeof(tempUser));
    FILE *fp;
    char tempArray[100];
    int i;
    fp = fopen(shadowFile, "rw");
    
    for (i = 0; i<33; i++) {
        fgets(tempArray, 100, fp);
        strcpy(myUsers[i].userinfo, tempArray);
        fseek(fp, 0, SEEK_CUR);
    }
    return myUsers;
}




int main(int argc, const char * argv[]){
    tempUser *myUsers = malloc(33*sizeof(tempUser));
    myUsers = create();
    for (int i = 0; i<33; i++) {
        printf("%s", myUsers[i].userinfo);
    }
    return 0;
}
