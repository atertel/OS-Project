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
#define shadowFile "shadow_example.txt"


typedef struct shadowDataNode{
    char *user;
    char pw_hash[50];
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

shadowDataNode *parse(shadowDataNode *head) {
	char *temp_str;
	int i;
	shadowDataNode *x;
	tempUser *users = create();
	head = malloc(sizeof(shadowDataNode));
	x = head;
	for (i = 0; i<33; i++) {
		temp_str = strtok(users[i].userinfo, ":");
		x->user = temp_str;

		temp_str = strtok(NULL, ":");
		strcpy(x->pw_hash, temp_str);

		temp_str = strtok(NULL, ":");
		x->numDays = atoi(temp_str);

		temp_str = strtok(NULL, ":");
		x->daysCanChange = atoi(temp_str);

		temp_str = strtok(NULL, ":");
		x->daysMustChange = atoi(temp_str);

		temp_str = strtok(NULL, ":");
		x->daysWarn = atoi(temp_str);

		if(i != 32) {
			x->next = malloc(sizeof(shadowDataNode));
			x = x->next;
		}
	}
	return head;
}
