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
#define shadowFile /Users/rich/Desktop/shadow_example.txt

typedef struct shadowDataNode{
    char *user;
    char hash[50];
    int numDays[10];
    int daysCanChange[10];
    int daysMustChange[10];
    int daysWarn[1];
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
    fp = fopen("/Users/rich/Desktop/shadow_example.txt", "rw");
    fgets(array, sizeof(array), fp);
    size = atoi(array);
    shadowData *myUsers = (*shadowData)(malloc(sizeof(shadowData)));
    
    for (i = 0; i<35; i++) {
        fgets(array, sizeof(array), fp);
        strtok(array, ":");
        strcpy(myUsers[i].user, array);
        p = (char*)strtok(NULL, ":");
        
        
    }
    
    
    
    
    return 0;
}
