//
//  main.c
//  shadow_parse
//
//  Created by Richard Lancia on 12/4/14.
//  Copyright (c) 2014 Richard Lancia. All rights reserved.
//


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
int main(int argc, const char * argv[]) {
    FILE *fp;
    int i = 0;
    char* token;
    int size;
    void* p;
    char array[1000];
    char s[2] = ":";
    char *temp_userName, temp_pwHash;
    fp = fopen("/Users/rich/Desktop/shadow_example.txt", "rw");
    fgets(array, sizeof(array), fp);
    size = atoi(array);
    shadowData *myUsers = (*shadowData)(malloc(sizeof(shadowData)));
    
   
        temp_userName = strtok(array, ":");
        strcpy(myUsers[i].user, temp_userName);
        temp_pwHash = *(char*)strtok(NULL, ":");
        strcpy(myUsers[i].hash, temp_pwHash);
        
    
    
    
    
    
    return 0;
}
